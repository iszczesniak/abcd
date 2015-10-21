#include "graph.hpp"
#include "ed_ksp.hpp"
#include <iostream>

using namespace std;

// Add edge, test, and set weight.
edge
foo(graph &g, vertex s, vertex d, int w)
{
  edge e;
  bool success;

  boost::tie(e, success) = add_edge(s, d, g);
  assert(success);

  boost::get(boost::edge_weight, g, e) = w;

  return e;
}

int main()
{
  graph g;

  foo(g, 0, 1, 1);
  foo(g, 0, 1, 1);
  foo(g, 1, 0, 1);
  foo(g, 1, 0, 1);

  plist l = boost::ed_ksp(g, 0, 1);

  cout << l.size() << endl;
  
  return 0;
}
