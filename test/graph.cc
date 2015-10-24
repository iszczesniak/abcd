#define BOOST_TEST_MODULE graph

#include "graph.hpp"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(parallel_edge_test)
{
  graph g(2);
  vertex a = *(vertices(g).first);
  vertex b = *(vertices(g).first + 1);
  edge e1, e2;
  bool r1, r2;
  tie(e1, r1) = add_edge(a, b, g);
  tie(e2, r2) = add_edge(a, b, g);

  // Make sure we can represent multi-graphs.  That we successfully
  // added the edges, and that the edges are different.
  BOOST_CHECK(r1);
  BOOST_CHECK(r2);
  BOOST_CHECK(e1 != e2);
  BOOST_CHECK(!(e1 == e2));

  // Make sure that parallel edges are differently identifies by a
  // map.
  std::map<edge, int> e2i;
  e2i[e1] = 1;
  e2i[e2] = 2;
  BOOST_CHECK(e2i[e1] == 1);
  BOOST_CHECK(e2i[e2] == 2);
  BOOST_CHECK(e2i.size() == 2);
}

BOOST_AUTO_TEST_CASE(parallel_reverse_edge_test)
{
  graph g(2);
  vertex a = *(vertices(g).first);
  vertex b = *(vertices(g).first + 1);
  edge e1, e2, e3;
  bool s1, s2, s3;
  tie(e1, s1) = add_edge(a, b, g);
  tie(e2, s2) = add_edge(a, b, g);

  // This is a free edge, i.e., it's not present in the graph.  We
  // still want this descriptor to be different from others.
  tie(e3, s3) = boost::edge(a, b, g);
  BOOST_CHECK(e3 != e1);
  BOOST_CHECK(e3 != e2);
}
