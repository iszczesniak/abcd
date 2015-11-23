#define BOOST_TEST_MODULE edge_disjoint_ksp

#include "edge_disjoint_ksp.hpp"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/filtered_graph.hpp>
#include <boost/test/unit_test.hpp>

#include <list>

typedef
boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS,
                      boost::no_property,
                      boost::property<boost::edge_weight_t, int> >
Graph;

typedef Graph::edge_descriptor Edge;
typedef Graph::vertex_descriptor Vertex;
typedef std::list<Edge> Path;

using namespace std;

// Add an edge, test it, and set weight.
Edge
ade(Graph &g, Vertex s, Vertex d, int w)
{
  Edge e;
  bool success;

  boost::tie(e, success) = add_edge(s, d, g);
  assert(success);

  boost::get(boost::edge_weight, g, e) = w;

  return e;
}

pair<Edge, Edge>
aue(Graph &g, Vertex s, Vertex d, int w)
{
  return make_pair(ade(g, s, d, w), ade(g, d, s, w));  
}

bool
check_path(const std::list<std::pair<int, Path>> &r, int w, const Path &p)
{
  for(auto const &ele: r)
    if (ele.first == w && ele.second == p)
      return true;

  return false;
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

BOOST_AUTO_TEST_CASE(edge_disjoint_ksp_test)
{
  Graph g(5);

  // Vertexes
  Vertex a = *(vertices(g).first + 0);
  Vertex b = *(vertices(g).first + 1);
  Vertex c = *(vertices(g).first + 2);
  Vertex d = *(vertices(g).first + 3);
  Vertex e = *(vertices(g).first + 4);

  // Edges
  Edge ab, ba;
  Edge ae, ea;
  Edge ac, ca;
  Edge be, eb;
  Edge cd1, dc1;
  Edge cd2, dc2;
  Edge ce, ec;

  tie(ab, ba) = aue(g, a, b, 4);
  tie(ae, ea) = aue(g, a, e, 2);
  tie(ac, ca) = aue(g, a, c, 6);
  tie(be, eb) = aue(g, b, e, 1);
  tie(cd1, dc1) = aue(g, c, d, 5);
  tie(cd2, dc2) = aue(g, c, d, 7);
  tie(ce, ec) = aue(g, c, e, 3);

  std::list<std::pair<int, Path>> r;
  r = boost::edge_disjoint_ksp(g, c, d);
  BOOST_CHECK(r.size() == 2);
  BOOST_CHECK(check_path(r, 5, Path{cd1}));
  BOOST_CHECK(check_path(r, 7, Path{cd2}));

  r = boost::edge_disjoint_ksp(g, b, d);
  BOOST_CHECK(r.size() == 2);
  BOOST_CHECK(check_path(r, 9, Path{be, ec, cd1}));
  BOOST_CHECK(check_path(r, 17, Path{ba, ac, cd2}));

  r = boost::edge_disjoint_ksp(g, a, e);
  BOOST_CHECK(r.size() == 3);
  BOOST_CHECK(check_path(r, 2, Path{ae}));
  BOOST_CHECK(check_path(r, 5, Path{ab, be}));
  BOOST_CHECK(check_path(r, 9, Path{ac, ce}));
}

BOOST_AUTO_TEST_CASE(edksp_filter_test)
{
  Graph g(3);
  Vertex a = *(vertices(g).first);
  Vertex b = *(vertices(g).first + 1);
  Vertex c = *(vertices(g).first + 2);

  set<Edge> x;
  boost::edksp_filter<Graph> f(&x);
  typedef boost::filtered_graph<Graph, boost::edksp_filter<Graph> > fg_t;
  fg_t fg(g, f);
  
  Edge e1, e2, e3;
  bool s;
  tie(e1, s) = add_edge(a, b, g);
  tie(e2, s) = add_edge(a, b, g);
  tie(e3, s) = add_edge(b, c, g);

  fg_t::edge_iterator i, ie;

  // Exclude e1 and make sure it's not one of the edges.
  x.insert(e1);
  for(tie(i, ie) = boost::edges(fg); i != ie; ++i)
    BOOST_CHECK(*i != e1);

  // Exclude e2 and make sure it's not one of the edges.
  x.insert(e2);
  for(tie(i, ie) = boost::edges(fg); i != ie; ++i)
    BOOST_CHECK(*i != e2);

  // Exclude e3 and make sure there are no edges left.
  x.insert(e3);
  // Now the edge set should be empty.
  tie(i, ie) = boost::edges(fg);
  BOOST_CHECK(i == ie);
}
