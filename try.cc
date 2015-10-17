#include <map>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>

int main()
{
  typedef
    boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                          boost::no_property,
                          boost::property<boost::edge_weight_t, int>>
    graph;

  typedef graph::edge_descriptor edge;
  typedef graph::vertex_descriptor vertex;

  // Build the graph.
  graph g;

  // Fill in the maps.
  std::map<edge, edge> e2e;
  std::map<edge, int> e2c;
  std::map<edge, int> e2r;
  std::map<edge, int> e2w;

  // Set the source and destination.
  vertex s, t;

  boost::associative_property_map<std::map<edge, edge>> rev(e2e);
  boost::associative_property_map<std::map<edge, int>> cap(e2c);
  boost::associative_property_map<std::map<edge, int>> res(e2r);
  boost::associative_property_map<std::map<edge, int>> wgt(e2w);
  boost::successive_shortest_path_nonnegative_weights(g, s, t,
                                                      capacity_map(cap).
                                                      residual_capacity_map(res).
                                                      reverse_edge_map(rev).
                                                      weight_map(wgt));

  return 0;
}
