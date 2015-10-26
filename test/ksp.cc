#define BOOST_TEST_MODULE ksp

#include <map>
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

bool
irek(const std::multimap<int, path> &r, int w, const path &p)
{
  std::multimap<int, path>::const_iterator i = r.find(w);
  return i != r.end() && i->second == p;
}

//       a
//      /|\
//     1 | 3 
//    /  |  \
//   b   2   c--5--d
//    \  |  /
//     4 | 6
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
  edge cd, dc;
  edge ce, ec;

  tie(ab, ba) = aue(g, a, b, 1);
  tie(ae, ea) = aue(g, a, e, 2);
  tie(ac, ca) = aue(g, a, c, 3);
  tie(be, eb) = aue(g, b, e, 4);
  tie(cd, dc) = aue(g, c, d, 5);
  tie(ce, ec) = aue(g, c, e, 6);

  std::multimap<int, path> r;
  r = boost::ed_ksp(g, c, d);
  BOOST_CHECK(r.size() == 1);
  BOOST_CHECK(irek(r, 5, path{cd}));

  r = boost::ed_ksp(g, b, d);
  BOOST_CHECK(r.size() == 1);
  BOOST_CHECK(irek(r, 9, path{ba, ac, cd}));

  r = boost::ed_ksp(g, a, e);
  BOOST_CHECK(r.size() == 3);
  BOOST_CHECK(irek(r, 2, path{ae}));
  BOOST_CHECK(irek(r, 5, path{ab, be}));
  BOOST_CHECK(irek(r, 9, path{ac, ce}));
}
