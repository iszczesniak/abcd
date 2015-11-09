#define BOOST_TEST_MODULE ksp

#include "graph.hpp"
#include "ed_ksp.hpp"

#include <boost/test/unit_test.hpp>

#include <map>

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

bool
irek(const std::multimap<int, path> &r, int w, const path &p)
{
  std::multimap<int, path>::const_iterator i = r.find(w);
  return i != r.end() && i->second == p;
}

//       a
//      /|\
//     4 | 6   --5--
//    /  |  \ /     \
//   b   2   c       d
//    \  |  / \     /
//     1 | 3   --7--
//      \|/
//       e

BOOST_AUTO_TEST_CASE(ksp_1)
{
  graph g(5);

  // Vertexes
  vertex a = *(vertices(g).first + 0);
  vertex b = *(vertices(g).first + 1);
  vertex c = *(vertices(g).first + 2);
  vertex d = *(vertices(g).first + 3);
  vertex e = *(vertices(g).first + 4);

  // Edges
  edge ab, ba;
  edge ae, ea;
  edge ac, ca;
  edge be, eb;
  edge cd1, dc1;
  edge cd2, dc2;
  edge ce, ec;

  tie(ab, ba) = aue(g, a, b, 4);
  tie(ae, ea) = aue(g, a, e, 2);
  tie(ac, ca) = aue(g, a, c, 6);
  tie(be, eb) = aue(g, b, e, 1);
  tie(cd1, dc1) = aue(g, c, d, 5);
  tie(cd2, dc2) = aue(g, c, d, 7);
  tie(ce, ec) = aue(g, c, e, 3);

  std::multimap<int, path> r;
  r = boost::ed_ksp(g, c, d);
  BOOST_CHECK(r.size() == 2);
  BOOST_CHECK(irek(r, 5, path{cd1}));
  BOOST_CHECK(irek(r, 7, path{cd2}));

  r = boost::ed_ksp(g, b, d);
  BOOST_CHECK(r.size() == 2);
  BOOST_CHECK(irek(r, 9, path{be, ec, cd1}));
  BOOST_CHECK(irek(r, 17, path{ba, ac, cd2}));

  r = boost::ed_ksp(g, a, e);
  BOOST_CHECK(r.size() == 3);
  BOOST_CHECK(irek(r, 2, path{ae}));
  BOOST_CHECK(irek(r, 5, path{ab, be}));
  BOOST_CHECK(irek(r, 9, path{ac, ce}));
}
