#include "traffic.hpp"

#include "custom_dijkstra_call.hpp"
#include <boost/optional.hpp>
#include <list>

using namespace std;

traffic::traffic(double mcat, double mht, double mnsc):
  mcat(mcat), catd(1 / mcat), catg(m_rng, catd),
  mht(mht), mnsc(mnsc), idc()
{
  schedule(0);
}

traffic::~traffic()
{
  for(auto c: cs)
    delete c;

  delete_clients();
}

int
traffic::nr_clients() const
{
  return cs.size();
}

void
traffic::operator()(double t)
{
  // Before we create new clients, we delete those clients that
  // requested deletion.
  delete_clients();
  
  // We are creating a client, but we ain't doing anything with the
  // pointer we get!  It's so, because it's up to the client to
  // register itself with the traffic.
  new client(mht, mnsc, *this);
  schedule_next(t);
}

void
traffic::schedule_next(double t)
{
  double dt = catg();
  schedule(t + dt);
}

void
traffic::insert(client *c)
{
  cs.insert(c);
}

void
traffic::erase(client *c)
{
  cs.erase(c);
}

void
traffic::delete_me_later(client *c)
{
  dl.push(c);
}

int
traffic::capacity_served() const
{
  int capacity = 0;

  // Iterate over all clients.
  for(const client *cli: cs)
    {
      const connection &c = cli->get_connection();
      const demand &d = c.get_demand();
      const npair &p = d.first;
      int nsc = c.get_nsc();

      // Did we compute the shortest path before?
      auto i = sd.find(d.first);
      // If not, do it now
      if (i == sd.end())
        {
          boost::optional<pair<int, list<edge>>> osp;
          osp = custom_dijkstra_call(m_mdl, p.first, p.second,
                                     get(boost::edge_weight_t(), m_mdl),
                                     get(boost::vertex_index_t(), m_mdl));
          // Make sure the shortest path was found.
          assert(osp);
          // This is the length of the shortest path.
          int len = osp.get().first;
          bool s;
          // Insert the results into the cache.
          tie(i, s) = sd.insert(make_pair(p, len));
          // Make sure there was no problem inserting the result.
          assert(s);
        }

      capacity += i->second * nsc;
    }

  return capacity;
}

void
traffic::delete_clients()
{
  while(!dl.empty())
    {
      delete dl.front();
      dl.pop();
    }
}
