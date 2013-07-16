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

  generate_graph(g, 100, 1000, gen);

  return 0;
}
