#include <iostream>
#include <list>
#include <queue>

#include "event.hpp"
#include "graph.hpp"
#include "module.hpp"
#include "utils_netgen.hpp"

using namespace std;

int
main()
{
  // The number of subcarriers.
  int sc = 800;

  // Simulation time.
  const double sim_limit = 1000000;

  // Random number generator
  boost::mt19937 gen(1);
  Graph g;

  // 100 nodes, 300 links.
  int res = generate_graph(g, 100, 300, gen);
  // We expect there to be exactly 3000 edges.
  assert(res == 300);
  // Make sure there is only one component.
  assert(check_components(g));
  name_vertices(g);

  // The DES priority queue.
  pqueue q;

  // The list of nodes.
  std::list<module> ls;

  for(int i = 1; i <= 2; ++i)
    {
      ls.push_back(module(q, i, gen, 2));
      ls.back().schedule(0);
    }

  // The event loop.
  while(!q.empty())
    {
      if (q.top().get_time() > sim_limit)
	break;

      q.top().process();
      q.pop();
    }
}
