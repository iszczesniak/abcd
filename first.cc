#include "graph.hpp"
#include "utils_netgen.hpp"
#include <iostream>

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

  return 0;
}
