#include "utils.hpp"

#define BOOST_TEST_MODULE graph

#include <boost/test/unit_test.hpp>
#include <boost/random/linear_congruential.hpp>

BOOST_AUTO_TEST_CASE(parallel_edge_test)
{
  graph g(2);
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
