#define BOOST_TEST_MODULE graph

#include "graph.hpp"
#include "xe_filter.hpp"

#include <boost/graph/filtered_graph.hpp>
#include <boost/test/unit_test.hpp>
#include <set>

using namespace std;

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

BOOST_AUTO_TEST_CASE(filtered_graph_test)
{
  graph g(3);
  vertex a = *(vertices(g).first);
  vertex b = *(vertices(g).first + 1);
  vertex c = *(vertices(g).first + 2);

  set<edge> x;
  xe_filter<graph> f(&x);
  typedef boost::filtered_graph<graph, xe_filter<graph> > fg_t;
  fg_t fg(g, f);
  
  edge e1, e2, e3;
  bool s;
  tie(e1, s) = add_edge(a, b, g);
  tie(e2, s) = add_edge(a, b, g);
  tie(e3, s) = add_edge(b, c, g);

  x.insert(e1);

  fg_t::edge_iterator i, ie;
  tie(i, ie) = boost::edges(fg);
}
