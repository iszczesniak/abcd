#include "graph.hpp"
#include "utils_netgen.hpp"
#include <iostream>

#include <boost/random/linear_congruential.hpp>

using namespace std;

int
main (int argc, char* argv[])
{
  minstd_rand gen;
  Graph g;

  // Random graph: 100 nodes, 1000 edges.
  generate_graph(g, 100, 1000, gen);
  name_vertices(g);

  // The number of subcarriers for each edge.
  set_subcarriers(g, 800);

  return 0;
}
