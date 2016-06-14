#include "client.hpp"

#include "simulation.hpp"
#include "stats.hpp"
#include "traffic.hpp"
#include "utils_netgen.hpp"

#include <boost/property_map/property_map.hpp>

#include <map>
#include <utility>

using namespace std;

client::client(double mht, double mnsc, traffic &tra):
  mht(mht), htd(1 / mht), htg(m_rng, htd),
  mnsc(mnsc), nscd(mnsc - 1), nscdg(m_rng, nscd),
  nohd(mnoh - 1), nohdg(m_rng, nohd),
  conn(m_mdl), st(stats::get()), tra(tra)
{
  // We try to setup the connection and reconfigure it.  We schedule
  // the connection to be torn down only when it was setup and
  // reconfigured.
  if (set_up() && reconfigure())
    {
      // Register the client with the traffic.
      tra.insert(this);
      // Holding time.
      double ht = htg();
      // Tear down time.
      tdt = now() + ht;
      schedule(tdt);
    }
  else
    tra.delete_me_later(this);
}

void client::operator()(double t)
{
  tear_down();
}

bool client::set_up()
{
  // The new demand.
  demand d;
  // The demand end nodes.
  d.first = random_node_pair(m_mdl, m_rng);
  // The number of slices the signal requires.  It's Poisson + 1.
  d.second = nscdg() + 1;

  // Set up the connection.
  bool status = conn.establish(d);

  // Report whether the connection was established or not.
  st.established(status);

  // If established, report the connection.
  if (status)
    st.established_conn(conn);

  return status;
}

bool
client::reconfigure()
{
  assert(conn.is_established());

  // Choose the next vertex.
  vertex new_src = get_new_src();
  
  // Reconfigure the connection for the new source vertex.
  auto result = conn.reconfigure(new_src);
  st.reconfigured(result.first);

  if (result.first)
    st.reconfigured(result.second);

  return result.first;
}

const connection &
client::get_connection() const
{
  return conn;
}

void
client::tear_down()
{
  assert(conn.is_established());
  conn.tear_down();
  tra.erase(this);
  tra.delete_me_later(this);
}

// The type of the exception thrown when we're done searching.
struct gns_exception {};

struct gns_visitor
{
  gns_visitor(set<vertex> &vs, vector<int> &hv, int hops):
    m_vs(vs), m_hv(hv), m_hops(hops) {}
  void operator()(vertex v, const graph&)
  {
    // Here we stop.  We will not find more candidate vertexes.
    if (m_hv[v] == m_hops + 1)
      throw gns_exception();

    // These are the candidate vertexes.
    if (m_hv[v] == m_hops)
      m_vs.insert(v);
  }

  set<vertex> &m_vs;
  vector<int> &m_hv;
  int m_hops;
};

vertex
client::get_new_src()
{
  // The new source should be this number of nodes away.
  int hops = nohdg() + 1;

  // Find the vertexes which are the given number of hops away.
  set<vertex> candidates;

  // We keep track of the number of hops for vertexes (using the
  // record_distances visitor), we record the candidate vertexes
  // (which are m_hops away from src), and quit as soon as we can.
  std::vector<int> hv(num_vertices(m_mdl));
  auto hm = make_iterator_property_map(hv.begin(),
                                       get(boost::vertex_index_t(), m_mdl));
  auto gnsv = gns_visitor(candidates, hv, hops);
  auto rdv = boost::record_distances(hm, boost::on_tree_edge());
  auto vstr = boost::visitor(boost::make_bfs_visitor(rdv));
  vertex src = conn.get_demand().first.first;

  try
    {
      boost::breadth_first_search (m_mdl, src, vstr);
    }
  catch (gns_exception) {}

  // Choose one of these vertexes at random.
  assert(!candidates.empty());
  return get_random_element(candidates, m_rng);
}
