#include "sample_graphs.hpp"

using namespace std;

void
sample_graph1(graph &g, vector<vertex> &vs, vector<edge> &ve)
{
  g = graph(3);
  vertex src = *(boost::vertices(g).first);
  vertex mid = *(boost::vertices(g).first + 1);
  vertex dst = *(boost::vertices(g).first + 2);
  edge e1 = boost::add_edge(src, mid, g).first;
  edge e2 = boost::add_edge(mid, dst, g).first;

  // Props of edge e1.
  boost::get(boost::edge_weight, g, e1) = 1;
  boost::get(boost::edge_ssc, g, e1).insert(0);
  boost::get(boost::edge_ssc, g, e1).insert(1);

  // Props of edge e2.
  boost::get(boost::edge_weight, g, e2) = 2;
  boost::get(boost::edge_ssc, g, e2).insert(1);
  boost::get(boost::edge_ssc, g, e2).insert(2);

  vs = vector<vertex>{src, mid, dst};
  ve = vector<edge>{e1, e2};
}
