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
//     4 | 6   --5--
//    /  |  \ /     \
//   b   2   c       d
//    \  |  / \     /
//     1 | 3   --7--
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

  tie(ab, ba) = aue(g, a, b, 4);
  tie(ae, ea) = aue(g, a, e, 2);
  tie(ac, ca) = aue(g, a, c, 6);
  tie(be, eb) = aue(g, b, e, 1);
  tie(cd1, dc1) = aue(g, c, d, 5);
  tie(cd2, dc2) = aue(g, c, d, 7);
  tie(ce, ec) = aue(g, c, e, 3);

  std::list<std::pair<int, Path>> r;

  r = boost::yen_ksp(g, c, d);
  BOOST_CHECK(r.size() == 6);
}
