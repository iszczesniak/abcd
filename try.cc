#include "graph.hpp"
#include "edge_disjoint_ksp.hpp"

#include <map>

int main()
{
  graph g;

  vertex s = 0, t = 1;

  boost::edge_disjoint_ksp(g, s, t, boost::get(boost::edge_weight_t(), g));

  return 0;
}
