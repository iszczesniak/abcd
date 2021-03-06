#define BOOST_TEST_MODULE graph

#include "graph.hpp"

#include <boost/graph/connected_components.hpp>
#include <boost/test/unit_test.hpp>
#include <set>

using namespace std;

BOOST_AUTO_TEST_CASE(undirected_edge_test)
{
  graph g(2);
  vertex a = *(vertices(g).first);
  vertex b = *(vertices(g).first + 1);
  edge e1, e2;
  bool r1, r2;

  // Create the edge.  This is an undirected edge.
  add_edge(a, b, g);

  tie(e1, r1) = boost::edge(a, b, g);
  tie(e2, r2) = boost::edge(b, a, g);
  BOOST_CHECK(r1);
  BOOST_CHECK(r2);
  // Edges e1 and e2 are the same.
  BOOST_CHECK(e1 == e2);
  // Yet, BGL can tell what the source and target nodes are are, and
  // they are different for e1 and e2.
  BOOST_CHECK(boost::source(e1, g) == a);
  BOOST_CHECK(boost::target(e1, g) == b);
  BOOST_CHECK(boost::source(e2, g) == b);
  BOOST_CHECK(boost::target(e2, g) == a);
}

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

  // Make sure that parallel edges are differently identified by a
  // map.
  std::map<edge, int> e2i;
  e2i[e1] = 1;
  e2i[e2] = 2;
  BOOST_CHECK(e2i[e1] == 1);
  BOOST_CHECK(e2i[e2] == 2);
  BOOST_CHECK(e2i.size() == 2);
}

BOOST_AUTO_TEST_CASE(connected_components_test)
{
  graph g(2);
  vertex a = *(vertices(g).first);
  vertex b = *(vertices(g).first + 1);
  edge e;
  bool r;
  tie(e, r) = add_edge(a, b, g);

  std::vector<int> c(num_vertices(g));
  // "num" is the number of connected components.
  int num = boost::connected_components(g, &c[0]);
  BOOST_CHECK(num == 1);

  tie(e, r) = add_edge(b, a, g);

  // "num" is the number of connected components.
  num = boost::connected_components(g, &c[0]);
  BOOST_CHECK(num == 1);
}

BOOST_AUTO_TEST_CASE(connected_components_test2)
{
  graph g(4);
  vertex a = *(vertices(g).first);
  vertex b = *(vertices(g).first + 1);
  vertex c = *(vertices(g).first + 2);
  vertex d = *(vertices(g).first + 3);
      
  add_edge(b, d, g);
  add_edge(c, d, g);

  std::vector<int> comp(num_vertices(g));
  // "num" is the number of connected components.
  int num = boost::connected_components(g, &comp[0]);
  BOOST_CHECK(num == 2);
}
