#include "graph.hpp"
#include "edge_disjoint_ksp.hpp"

#include <map>

int main()
{
  // Build the graph.
  graph g;

  vertex s = 0, t = 1;

  boost::edge_disjoint_ksp(g, s, t);

  return 0;
}
