#include "client.hpp"

#include "simulation.hpp"
#include "stats.hpp"
#include "traffic.hpp"
#include "utils_netgen.hpp"
#include "utils.hpp"

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
  // Try to setup the connection.
  if (set_up())
    {
      // Try to reconfigure the connection.
      if (reconfigure())
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
        // We managed to establish the connection, but failed to
        // reconfigure it.  That's the end of the game.
        destroy();
    }
  else
    // We didn't manage to establish the connection, and so the client
    // should be deleted.
    tra.delete_me_later(this);
}

void client::operator()(double t)
{
  destroy();
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
  vertex new_dst = get_new_dst();

  // Reconfigure the connection for the new source vertex.
  auto result = conn.reconfigure(new_dst);
  st.reconfigured(result != boost::none);

  if (result != boost::none)
    st.reconfigured_conn(conn, result.get().first, result.get().second);

  return result != boost::none;
}

const connection &
client::get_connection() const
{
  return conn;
}

void
client::destroy()
{
  assert(conn.is_established());
  conn.tear_down();
  tra.erase(this);
  tra.delete_me_later(this);
}

vertex
client::get_new_dst()
{
  set<vertex> candidates;
  
  do
    {
      // The new source should be this number of nodes away.
      int hops = nohdg() + 1;

      // Find the vertexes which are the given number of hops away.
      vertex dst = conn.get_demand().first.second;
      candidates = find_vertexes(m_mdl, dst, hops);
    }
  while (candidates.empty());

  return get_random_element(candidates, m_rng);
}
