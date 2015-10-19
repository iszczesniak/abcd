#include "graph.hpp"
#include "edge_disjoint_ksp.hpp"

#include <map>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>

int main()
{
  // Build the graph.
  graph g;

  vertex s = 0, t = 1;

  boost::edge_disjoint_ksp(g, s, t);

  return 0;
}
