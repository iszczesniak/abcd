#include "graph.hpp"
#include "dijkstra.hpp"
#include "utils_netgen.hpp"
#include <iostream>
#include <utility>

#include <boost/random/linear_congruential.hpp>
#include <boost/graph/graph_utility.hpp>

using namespace std;

int
main (int argc, char* argv[])
{
  // The number of subcarriers.
  int sc = 800;

  minstd_rand gen;
  Graph g;

  // 1000 nodes, 3000 links.
  int res = generate_graph(g, 1000, 3000, gen);
  // We expect there to be exactly 3000 edges.
  assert(res == 3000);
  // Make sure there is only one component.
  assert(check_components(g));
  name_vertices(g);

  // The number of subcarriers for each edge.
  set_subcarriers(g, 800);

  // Load the network.
  for(int i = 0; i < 100; ++i)
    {
      cout << "Allocating..." << endl;

      // Get a pair of different nodes.
      pair<Vertex, Vertex> p = random_node_pair(g, gen);

      // The number of subcarriers the signal requires.
      int n = get_random_int(1, 10, gen);

      cout << n << endl;

      // We allow to allocate the signal on any of the subcarriers.
      SSC all(counting_iterator<int>(0), counting_iterator<int>(sc));
      V2C2S r = dijkstra(g, p.first, p.second, n, all);
    }

  return 0;
}
