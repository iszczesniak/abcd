#include "utils.hpp"

#define BOOST_TEST_MODULE Graph

#include <boost/test/unit_test.hpp>
#include <boost/random/linear_congruential.hpp>

BOOST_AUTO_TEST_CASE(parallel_edge_test)
{
  Graph g(2);
  Vertex a = *(vertices(g).first);
  Vertex b = *(vertices(g).first + 1);
  Edge e1, e2;
  bool r1, r2;
  tie(e1, r1) = add_edge(a, b, g);
  tie(e2, r2) = add_edge(a, b, g);

  // Make sure we can represent multi-graphs.  That we successfully
  // added the edges, and that the edges are different.
  BOOST_CHECK(r1);
  BOOST_CHECK(r2);
  BOOST_CHECK(e1 != e2);
}

BOOST_AUTO_TEST_CASE(edge_comparison_test)
{
  Graph g(2);
  Vertex a = *(vertices(g).first);
  Vertex b = *(vertices(g).first + 1);
  add_edge(a, b, g).first;
  add_edge(a, b, g).first;

  // This is the default Edge.
  Edge e;

  // Make sure the default edge is the smallest of all edges.  We need
  // to test for this, because we use (in shortest path) the
  // lower_bound function on a map with the Edge key.
  graph_traits<Graph>::edge_iterator ei, ee;
  for (tie(ei, ee) = edges(g); ei != ee; ++ei)
    BOOST_CHECK(e < *ei);
}
