#include <iostream>
#include <list>
#include <queue>

#include "client.hpp"
#include "event.hpp"
#include "graph.hpp"
#include "utils_netgen.hpp"

#include <boost/random.hpp>

using namespace std;

int
main()
{
  // The number of subcarriers.
  const int sc = 800;

  // Simulation time.
  const double sim_limit = 100;

  // The number of nodes.
  const int nn = 100;

  // The number of edges.
  const int ne = 300;

  // The lambda for each module.
  const double lambda = 2;

  // Random number generator
  boost::mt19937 gen(1);
  Graph g;

  // 100 nodes, 300 links.
  int res = generate_graph(g, nn, ne, gen);
  // We expect there to be exactly 3000 edges.
  assert(res == ne);

  // Make sure there is only one component.
  assert(check_components(g));
  name_vertices(g);

  // The DES priority queue.
  pqueue q;

  // The list of nodes.
  std::vector<module *> vc;

  // Create the modules for the simulation.
  for(int i = 0; i < nn; ++i)
    {
      client *c = new client(q, i, gen, 2);
      c->schedule(0);
      vc.push_back(c);
    }

  // The event loop.
  while(!q.empty())
    {
      if (q.top().get_time() > sim_limit)
	break;

      q.top().process();
      q.pop();
    }

  // Delete the clients.
  for(module *c: vc)
    delete c;
}
