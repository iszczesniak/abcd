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
  minstd_rand gen;
  Graph g;

  generate_graph(g, 10, 20, gen);
  name_vertices(g);
  complete_graph(g);

  // The number of subcarriers for each edge.
  set_subcarriers(g, 2);

  print_graph(g);
  print_subcarriers(g);
  print_sp(g, cout);

  for(int i = 0; i < 100; ++i)
    {
      pair<Vertex, Vertex> p = random_node_pair(g, gen);
      cout << get(vertex_name, g, p.first)
           << " "
           << get(vertex_name, g, p.second)
           << endl;
    }

  SSC all(counting_iterator<int>(0),
          counting_iterator<int>(10));
  dijkstra(g, 0, 5, 2, all);

  return 0;
}
