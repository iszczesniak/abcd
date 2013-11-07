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

  boost::minstd_rand gen;
  graph g;

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

      boost::graph_traits<graph>::edge_iterator ei, ee;
      for (boost::tie(ei, ee) = boost::edges(g); ei != ee; ++ei)
        sc_free += boost::get(boost::edge_subcarriers, g, *ei).size();

      cout << "Total subcarriers available = " << sc_free << endl;

      // Demand
      demand d;

      // Get a pair of different nodes.
      d.first = random_node_pair(g, gen);

      // The number of subcarriers the signal requires.
      d.second = get_random_int(1, 10, gen);

      cout << "Demand subcarriers = " << d.second << endl;

      cout << "Allocating... ";

      V2C2S r = dijkstra(g, d);
      cpath p = shortest_path(g, r, d);

      if (p.first.empty())
	cout << "unable to service a new demand." << endl;
      else
        {
	  set_up_path(g, p);
          cout << "done." << endl;
          ++connections;
          sc_allocated += d.second * p.first.size();
        }

      cout << "Load = " << calculate_load(g, sc) << endl;
      cout << "Connections = " << connections << endl;
      cout << "Total subcarriers allocated = " << sc_allocated << endl;
      cout << "----------------------------------------------" << endl;
    };

  return 0;
}
