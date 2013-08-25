#include "utils.hpp"
#include "utils_netgen.hpp"
#include "dijkstra.hpp"

#include <iostream>

#define BOOST_TEST_MODULE Utils

#include <boost/test/unit_test.hpp>

/*
 * Make sure that we can't find a path if there are no subcarriers.
 * The graph is a simple case of two nodes and a single link.
 */
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

/*
 * Make sure that we can find a path if there are subcarriers.  The
 * graph is a simple case of two nodes and a single link.
 */
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

/*
 * Make sure we can establish a path with a worse cost then initially
 * it seemed.
 */
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

/*
 * Make sure we don't remember at node mid the results for edge e1
 * that led to that node with a worse cost then edge e2 and with the
 * same subcarriers.
 */
BOOST_AUTO_TEST_CASE(dijkstra_test_4)
{
  Graph g(3);
  Vertex src = *(vertices(g).first);
  Vertex mid = *(vertices(g).first + 1);
  Vertex dst = *(vertices(g).first + 2);
  Edge e1 = add_edge(src, mid, g).first;
  Edge e2 = add_edge(src, mid, g).first;
  Edge e3 = add_edge(mid, dst, g).first;

  // Props of edge e1.
  get(edge_weight, g, e1) = 2;
  get(edge_subcarriers, g, e1).insert(0);

  // Props of edge e2.
  get(edge_weight, g, e2) = 1;
  get(edge_subcarriers, g, e2).insert(0);

  // Props of edge e3.
  get(edge_weight, g, e3) = 2;
  get(edge_subcarriers, g, e3).insert(0);

  SSC ssc(counting_iterator<int>(0), counting_iterator<int>(1));

  V2C2S result = dijkstra(g, src, dst, 1, ssc);

  // We found the path.
  BOOST_CHECK(!result[dst].empty());
  // The cost of the path is 3.
  BOOST_CHECK(result[dst].begin()->first.first == 3);
  // We reach node mid by edge e2.
  BOOST_CHECK(result[mid].begin()->first.second == e2);
  // We don't remember the results for edge e1.
  BOOST_CHECK(result[mid].begin()->second.size() == 1);
}

/*
 * Make sure we remember at node mid the results for both edge e1 and
 * e2.  e2 offers the shortest path with subcarrier 0, and e1 offers
 * the two subcarriers: 0 and 1 albeit at a higher cost.
 */
BOOST_AUTO_TEST_CASE(dijkstra_test_5)
{
  Graph g(3);
  Vertex src = *(vertices(g).first);
  Vertex mid = *(vertices(g).first + 1);
  Vertex dst = *(vertices(g).first + 2);
  Edge e1 = add_edge(src, mid, g).first;
  Edge e2 = add_edge(src, mid, g).first;
  Edge e3 = add_edge(mid, dst, g).first;

  // Props of edge e1.
  get(edge_weight, g, e1) = 2;
  get(edge_subcarriers, g, e1).insert(0);
  get(edge_subcarriers, g, e1).insert(1);

  // Props of edge e2.
  get(edge_weight, g, e2) = 1;
  get(edge_subcarriers, g, e2).insert(0);

  // Props of edge e3.
  get(edge_weight, g, e3) = 2;
  get(edge_subcarriers, g, e3).insert(0);

  // These are the subcarriers that we can start with at src.
  SSC ssc(counting_iterator<int>(0), counting_iterator<int>(2));
  V2C2S result = dijkstra(g, src, dst, 1, ssc);

  // We found the path.
  BOOST_CHECK(!result[dst].empty());
  // The cost of the path is 3.
  BOOST_CHECK(result[dst].begin()->first.first == 3);

  // We remember at node mid the results for edges e1 and e2.
  BOOST_CHECK(result[mid].size() == 2);
  // We reach node mid by edge e2 with cost 1.
  BOOST_CHECK(result[mid].begin()->first.first == 1);
  BOOST_CHECK(result[mid].begin()->first.second == e2);
  // The second way of reaching node mid is by edge e1 with cost 2.
  BOOST_CHECK((++result[mid].begin())->first.first == 2);
  BOOST_CHECK((++result[mid].begin())->first.second == e1);
}
