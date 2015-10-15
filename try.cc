#include<iostream>

#include "graph.hpp"

int main() {
  graph g0;
  graph &g1 = g0.create_subgraph();

  add_vertex(g1);
  add_vertex(g1);
  
  std::cout << num_vertices(g0) << std::endl;
  std::cout << num_vertices(g1) << std::endl;
  
  edge e1, e2;
  bool s1, s2;
  
  boost::tie(e1, s1) = boost::add_edge(0, 1, g0);
  assert(s1);
  
  boost::tie(e2, s2) = boost::add_edge(0, 1, g1);
  assert(s2);

  std::cout << num_edges(g0) << std::endl;
  std::cout << num_edges(g1) << std::endl;
  
  return 0;
}
