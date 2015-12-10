#define BOOST_TEST_MODULE yen_ksp

#include "yen_ksp.hpp"

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
typedef boost::exclude_filter<Edge> eef_type;

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
//     1 | 1   --1--
//    /  |  \ /     \
//   b   2   c       d
//    \  |  / \     /
//     2 | 1   --10--
//      \|/
//       e

BOOST_AUTO_TEST_CASE(yen_ksp_test)
{
  Graph g(5);

  // Vertexes
  auto i = vertices(g).first;
  Vertex a = *i++;
  Vertex b = *i++;
  Vertex c = *i++;
  Vertex d = *i++;
  Vertex e = *i;

  // Edges
  Edge ab, ba;
  Edge ae, ea;
  Edge ac, ca;
  Edge be, eb;
  Edge cd1, dc1;
  Edge cd2, dc2;
  Edge ce, ec;

  tie(ab, ba) = aue(g, a, b, 1);
  tie(ae, ea) = aue(g, a, e, 2);
  tie(ac, ca) = aue(g, a, c, 1);
  tie(be, eb) = aue(g, b, e, 2);
  tie(cd1, dc1) = aue(g, c, d, 1);
  tie(cd2, dc2) = aue(g, c, d, 10);
  tie(ce, ec) = aue(g, c, e, 1);

  std::list<std::pair<int, Path>> r;

  r = boost::yen_ksp(g, b, d);

  for(auto a: r)      
    cout << a.first << " " << a.second << endl;

  BOOST_CHECK(check_path(r, 3, Path{ba, ac, cd1}));
  BOOST_CHECK(check_path(r, 12, Path{ba, ac, cd2}));

  BOOST_CHECK(check_path(r, 4, Path{be, ec, cd1}));
  BOOST_CHECK(check_path(r, 13, Path{be, ec, cd2}));

  BOOST_CHECK(check_path(r, 5, Path{ba, ae, ec, cd1}));
  BOOST_CHECK(check_path(r, 14, Path{ba, ae, ec, cd2}));

  BOOST_CHECK(check_path(r, 6, Path{be, ea, ac, cd1}));
  BOOST_CHECK(check_path(r, 15, Path{be, ea, ac, cd2}));
}
