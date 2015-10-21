#include "graph.hpp"
#include "edge_disjoint_ksp.hpp"

int main()
{
  graph g;

  assert(add_edge(0, 1, g).second);
  assert(add_edge(0, 1, g).second);
  assert(add_edge(1, 0, g).second);
  assert(add_edge(1, 0, g).second);

  vertex s = 0, t = 1;

  plist = boost::edge_disjoint_ksp(g, s, t);

  return 0;
}
