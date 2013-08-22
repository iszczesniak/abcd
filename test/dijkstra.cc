#include "utils.hpp"
#include "utils_netgen.hpp"
#include "dijkstra.hpp"

#include <iostream>

#define BOOST_TEST_MODULE Utils

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(dijkstra_test_1)
{
  Graph g(2);
  Vertex src = *(vertices(g).first);
  Vertex dst = *(vertices(g).first + 1);
  add_edge(src, dst, g);
  set_subcarriers(g, 2);

  SSC ssc(counting_iterator<int>(0), counting_iterator<int>(3));

  V2C2S result = dijkstra(g, src, dst, 3, ssc);

  BOOST_CHECK(result[dst].empty());
}

BOOST_AUTO_TEST_CASE(dijkstra_test_2)
{
  Graph g(2);
  Vertex src = *(vertices(g).first);
  Vertex dst = *(vertices(g).first + 1);
  add_edge(src, dst, g);
  set_subcarriers(g, 3);

  SSC ssc(counting_iterator<int>(0), counting_iterator<int>(3));

  V2C2S result = dijkstra(g, src, dst, 3, ssc);

  BOOST_CHECK(!result[dst].empty());
  BOOST_CHECK(result[dst].begin()->second.size() == 3);
}

BOOST_AUTO_TEST_CASE(dijkstra_test_3)
{
  Graph g(3);
  Vertex src = *(vertices(g).first);
  Vertex mid = *(vertices(g).first + 1);
  Vertex dst = *(vertices(g).first + 2);
  Edge e1 = add_edge(src, mid, g).first;
  Edge e2 = add_edge(src, mid, g).first;
  Edge e3 = add_edge(mid, dst, g).first;

  // Make sure we can represent multi-graphs.
  BOOST_CHECK(e1 != e2);

  // Props of edge e1.
  get(edge_weight, g, e1) = 1;
  get(edge_subcarriers, g, e1).insert(0);
  get(edge_subcarriers, g, e1).insert(1);

  // Props of edge e2.
  get(edge_weight, g, e2) = 2;
  get(edge_subcarriers, g, e2).insert(2);
  get(edge_subcarriers, g, e2).insert(3);

  // Props of edge e3.
  get(edge_weight, g, e3) = 1;
  get(edge_subcarriers, g, e3).insert(2);
  get(edge_subcarriers, g, e3).insert(3);

  SSC ssc(counting_iterator<int>(0), counting_iterator<int>(4));

  V2C2S result = dijkstra(g, src, dst, 2, ssc);

  // We found the path.
  BOOST_CHECK(!result[dst].empty());
  // The cost of the path is 3.
  BOOST_CHECK(result[dst].begin()->first.first == 3);
  // We reach the destination by edge e3.
  BOOST_CHECK(result[dst].begin()->first.second == e3);
  // There are two subcarriers in the solution.
  BOOST_CHECK(result[dst].begin()->second.size() == 2);
  BOOST_CHECK(*(result[dst].begin()->second.begin()) == 2);
  BOOST_CHECK(*(++(result[dst].begin()->second.begin())) == 3);
}
