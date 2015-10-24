#define BOOST_TEST_MODULE ksp

#include "graph.hpp"
#include "ed_ksp.hpp"
#include <boost/test/unit_test.hpp>

using namespace std;

// Add an edge, test it, and set weight.
edge
ade(graph &g, vertex s, vertex d, int w)
{
  edge e;
  bool success;

  boost::tie(e, success) = add_edge(s, d, g);
  assert(success);

  boost::get(boost::edge_weight, g, e) = w;

  return e;
}

pair<edge, edge>
aue(graph &g, vertex s, vertex d, int w)
{
  return make_pair(ade(g, s, d, w), ade(g, d, s, w));  
}

//      1
//     /|\
//    / | \
//   0  |  3--4
//    \ | /
//     \|/
//      2

BOOST_AUTO_TEST_CASE(ksp_1)
{
  graph g(5);

  aue(g, 0, 1, 1);
  aue(g, 0, 2, 1);
  aue(g, 1, 2, 1);
  aue(g, 1, 3, 1);
  aue(g, 2, 3, 1);
  aue(g, 3, 4, 1);

  plist l;

  l = boost::ed_ksp(g, 3, 4);
  BOOST_CHECK(l.size() == 1);

  // There are two paths of length 2.
  l = boost::ed_ksp(g, 0, 3);
  BOOST_CHECK(l.size() == 2);
  BOOST_CHECK(l.front().size() == 2);
  BOOST_CHECK((++l.begin())->size() == 2);

  l = boost::ed_ksp(g, 0, 4);
  BOOST_CHECK(l.size() == 1);

  l = boost::ed_ksp(g, 1, 2);
  BOOST_CHECK(l.size() == 3);
}
