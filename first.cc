#include "graph.hpp"
#include "dijkstra.hpp"
#include "utils_netgen.hpp"
#include <iostream>
#include <utility>

#include <boost/random/linear_congruential.hpp>
#include <boost/graph/graph_utility.hpp>

using namespace std;
using namespace boost;

int
main (int argc, char* argv[])
{
  // The number of subcarriers.
  int sc = 800;

  boost::minstd_rand gen;
  Graph g;

  // 1000 nodes, 3000 links.
  int res = generate_graph(g, 1000, 3000, gen);
  // We expect there to be exactly 3000 edges.
  assert(res == 3000);
  // Make sure there is only one component.
  assert(check_components(g));
  name_vertices(g);

  // The number of subcarriers for each edge.
  set_subcarriers(g, sc);

  // Set the distances for each edge.
  set_distances(g, 10, 100, gen);

  int connections = 0;
  int sc_allocated = 0;

  // Load the network until the network cannot service a new demand.
  while(true)
    {
      int sc_free = 0;

      graph_traits<Graph>::edge_iterator ei, ee;
      for (tie(ei, ee) = edges(g); ei != ee; ++ei)
        sc_free += get(edge_subcarriers, g, *ei).size();

      cout << "Available subcarriers = " << sc_free << endl;

      cout << "Allocating..." << endl;

      // Get a pair of different nodes.
      pair<Vertex, Vertex> pn = random_node_pair(g, gen);

      // The number of subcarriers the signal requires.
      int n = get_random_int(1, 10, gen);

      cout << "Subcarriers requested = " << n << endl;

      // We allow to allocate the signal on any of the subcarriers.
      SSC all(counting_iterator<int>(0), counting_iterator<int>(sc));
      V2C2S r = dijkstra(g, pn.first, pn.second, n, all);
      Path p = shortest_path(g, r, pn.first, pn.second);

      if (p.first.empty())
	cout << "Unable to service a new request." << endl;
      else
        {
	  set_up_path(g, p, n);
          ++connections;
          sc_allocated += n * p.first.size();
        }

      cout << "Load = " << calculate_load(g, sc) << endl;
      cout << "Connections = " << connections << endl;
      cout << "Subcarriers allocated = " << sc_allocated << endl;
    };

  return 0;
}
