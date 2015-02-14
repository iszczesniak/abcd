#include "utils.hpp"
#include "utils_netgen.hpp"
#include "dijkstra.hpp"

#include <iostream>

#define BOOST_TEST_MODULE Dijkstra

#include <boost/test/unit_test.hpp>

/*
 * Make sure that we can't find a path if there are no subcarriers.
 * The graph is a simple case of two nodes and a single link.
 */
BOOST_AUTO_TEST_CASE(dijkstra_test_1)
{
  graph g(2);
  vertex src = *(boost::vertices(g).first);
  vertex dst = *(boost::vertices(g).first + 1);
  boost::add_edge(src, dst, g);
  set_subcarriers(g, 2);

  demand d = demand(npair(src, dst), 3);
  V2C2S result = dijkstra::search(g, d);
  // There are no results for dst.
  BOOST_CHECK(result.find(dst) == result.end());
}

/*
 * Make sure that we can find a path if there are subcarriers.  The
 * graph is a simple case of two nodes and a single link.
 */
BOOST_AUTO_TEST_CASE(dijkstra_test_2)
{
  graph g(2);
  vertex src = *(boost::vertices(g).first);
  vertex dst = *(boost::vertices(g).first + 1);
  edge e = boost::add_edge(src, dst, g).first;
  set_subcarriers(g, 3);

  demand d = demand(npair(src, dst), 3);
  V2C2S result = dijkstra::search(g, d);

  BOOST_CHECK(result.find(dst) != result.end());
  // There is one set in SSSC.
  BOOST_CHECK(result[dst].begin()->second.size() == 1);
  // And that one set has three subcarriers.
  BOOST_CHECK(result[dst].begin()->second.begin()->size() == 3);

  sscpath p = dijkstra::trace(g, result, d);
  // The path has one edge.
  BOOST_CHECK(p.first.size() == 1);
  // That one edge is e.
  BOOST_CHECK(*(p.first.begin()) == e);
  // The path uses SSC with three subcarriers.
  BOOST_CHECK(p.second.size() == 3);

  // There are three subcarriers available.
  BOOST_CHECK(boost::get(boost::edge_ssc, g, e).size() == 3);
  // Set up the path.
  dijkstra::set_up_path(g, p);
  // The subcarriers have been taken.
  BOOST_CHECK(boost::get(boost::edge_ssc, g, e).empty());
  // Release the subcarriers taken by the path.
  dijkstra::tear_down_path(g, p);
  // There are three subcarriers available.
  BOOST_CHECK(boost::get(boost::edge_ssc, g, e).size() == 3);
}

/*
 * Make sure we can establish a path with a worse cost then initially
 * it seemed.
 */
BOOST_AUTO_TEST_CASE(dijkstra_test_3)
{
  graph g(3);
  vertex src = *(boost::vertices(g).first);
  vertex mid = *(boost::vertices(g).first + 1);
  vertex dst = *(boost::vertices(g).first + 2);
  edge e1 = boost::add_edge(src, mid, g).first;
  edge e2 = boost::add_edge(src, mid, g).first;
  edge e3 = boost::add_edge(mid, dst, g).first;

  // Props of edge e1.
  boost::get(boost::edge_weight, g, e1) = 1;
  boost::get(boost::edge_ssc, g, e1).insert(0);
  boost::get(boost::edge_ssc, g, e1).insert(1);

  // Props of edge e2.
  boost::get(boost::edge_weight, g, e2) = 2;
  boost::get(boost::edge_ssc, g, e2).insert(2);
  boost::get(boost::edge_ssc, g, e2).insert(3);

  // Props of edge e3.
  boost::get(boost::edge_weight, g, e3) = 1;
  boost::get(boost::edge_ssc, g, e3).insert(2);
  boost::get(boost::edge_ssc, g, e3).insert(3);

  demand d = demand(npair(src, dst), 2);
  V2C2S result = dijkstra::search(g, d);

  V2C2S::const_iterator i = result.find(dst);
  assert(i != result.end());
  const C2S &c2s = i->second;

  // We found the path.
  BOOST_CHECK(!c2s.empty());
  // The cost of the path is 3.
  BOOST_CHECK(c2s.begin()->first.first.first == 3);
  // We reach the destination by edge e3.
  BOOST_CHECK(c2s.begin()->first.second == e3);
  // There are two subcarriers in the solution: 2 and 3.
  BOOST_CHECK(c2s.begin()->second.size() == 1);
  BOOST_CHECK(*(c2s.begin()->second.begin()->begin()) == 2);
  BOOST_CHECK(*(++(c2s.begin()->second.begin()->begin())) == 3);

  sscpath p = dijkstra::trace(g, result, d);
  // The path has two edges.
  BOOST_CHECK(p.first.size() == 2);
  // The first edge is e2, the second e3.
  BOOST_CHECK(*(p.first.begin()) == e2);
  BOOST_CHECK(*(++p.first.begin()) == e3);
  // The path uses SSC with three subcarriers.
  BOOST_CHECK(p.second.size() == 2);
  // The subcarriers used are 2 and 3.
  BOOST_CHECK(p.second.find(2) != p.second.end());
  BOOST_CHECK(p.second.find(3) != p.second.end());

  // The number of subcarriers befor the path is set up.
  BOOST_CHECK(boost::get(boost::edge_ssc, g, e1).size() == 2);
  BOOST_CHECK(boost::get(boost::edge_ssc, g, e2).size() == 2);
  BOOST_CHECK(boost::get(boost::edge_ssc, g, e3).size() == 2);

  // Set up the path.
  dijkstra::set_up_path(g, p);

  // The number of subcarriers after the path is set up.
  BOOST_CHECK(boost::get(boost::edge_ssc, g, e1).size() == 2);
  BOOST_CHECK(boost::get(boost::edge_ssc, g, e2).size() == 0);
  BOOST_CHECK(boost::get(boost::edge_ssc, g, e3).size() == 0);

  // Tear down the path.
  dijkstra::tear_down_path(g, p);

  // The number of subcarriers befor the path is set up.
  BOOST_CHECK(boost::get(boost::edge_ssc, g, e1).size() == 2);
  BOOST_CHECK(boost::get(boost::edge_ssc, g, e2).size() == 2);
  BOOST_CHECK(boost::get(boost::edge_ssc, g, e3).size() == 2);
}

/*
 * Make sure we don't remember at node mid the results for edge e1
 * that led to that node with a worse cost then edge e2 and with the
 * same subcarriers.
 */
BOOST_AUTO_TEST_CASE(dijkstra_test_4)
{
  graph g(3);
  vertex src = *(vertices(g).first);
  vertex mid = *(vertices(g).first + 1);
  vertex dst = *(vertices(g).first + 2);
  edge e1 = add_edge(src, mid, g).first;
  edge e2 = add_edge(src, mid, g).first;
  edge e3 = add_edge(mid, dst, g).first;

  // Props of edge e1.
  boost::get(boost::edge_weight, g, e1) = 2;
  boost::get(boost::edge_ssc, g, e1).insert(0);

  // Props of edge e2.
  boost::get(boost::edge_weight, g, e2) = 1;
  boost::get(boost::edge_ssc, g, e2).insert(0);

  // Props of edge e3.
  boost::get(boost::edge_weight, g, e3) = 2;
  boost::get(boost::edge_ssc, g, e3).insert(0);

  demand d = demand(npair(src, dst), 1);
  V2C2S result = dijkstra::search(g, d);

  // We found the path.
  BOOST_CHECK(!result[dst].empty());
  // The cost of the path is 3.
  BOOST_CHECK(result[dst].begin()->first.first.first == 3);
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
  graph g(3);
  vertex src = *(vertices(g).first);
  vertex mid = *(vertices(g).first + 1);
  vertex dst = *(vertices(g).first + 2);
  edge e1 = add_edge(src, mid, g).first;
  edge e2 = add_edge(src, mid, g).first;
  edge e3 = add_edge(mid, dst, g).first;

  // Props of edge e1.
  boost::get(boost::edge_weight, g, e1) = 2;
  boost::get(boost::edge_ssc, g, e1).insert(0);
  boost::get(boost::edge_ssc, g, e1).insert(1);

  // Props of edge e2.
  boost::get(boost::edge_weight, g, e2) = 1;
  boost::get(boost::edge_ssc, g, e2).insert(0);

  // Props of edge e3.
  boost::get(boost::edge_weight, g, e3) = 2;
  boost::get(boost::edge_ssc, g, e3).insert(0);

  demand d = demand(npair(src, dst), 1);
  V2C2S result = dijkstra::search(g, d);

  // We found the path.
  BOOST_CHECK(!result[dst].empty());
  // The cost of the path is 3.
  BOOST_CHECK(result[dst].begin()->first.first.first == 3);

  // We remember at node mid the results for edges e1 and e2.
  BOOST_CHECK(result[mid].size() == 2);
  // We reach node mid by edge e2 with cost 1.
  BOOST_CHECK(result[mid].begin()->first.first.first == 1);
  BOOST_CHECK(result[mid].begin()->first.second == e2);
  // The second way of reaching node mid is by edge e1 with cost 2.
  BOOST_CHECK((++result[mid].begin())->first.first.first == 2);
  BOOST_CHECK((++result[mid].begin())->first.second == e1);
}

/*
 * Make sure that we don't remember the results for edge e2, because
 * it offers a worse path then edge e1.  This tests for the existance
 * of better old results.
 */
BOOST_AUTO_TEST_CASE(dijkstra_test_6)
{
  graph g(2);
  vertex src = *(vertices(g).first);
  vertex dst = *(vertices(g).first + 1);

  edge e1 = boost::add_edge(src, dst, g).first;
  edge e2 = boost::add_edge(src, dst, g).first;

  // Props of edge e1.
  boost::get(boost::edge_weight, g, e1) = 1;
  boost::get(boost::edge_ssc, g, e1).insert(0);

  // Props of edge e2.  During the Dijkstra::Search search, the result for
  // this edge won't be even added, because already the better result
  // for edge e1 will be in place.
  boost::get(boost::edge_weight, g, e2) = 2;
  boost::get(boost::edge_ssc, g, e2).insert(0);

  demand d = demand(npair(src, dst), 1);
  V2C2S result = dijkstra::search(g, d);

  BOOST_CHECK(result[dst].size() == 1);
  BOOST_CHECK(result[dst].begin()->first.first.first == 1);
  BOOST_CHECK(result[dst].begin()->first.second == e1);
}

/*
 * Make sure that we don't remember the results for edge e1, because
 * it offers a worse path then edge e2.  This tests the purging of the
 * old results.
 */
BOOST_AUTO_TEST_CASE(dijkstra_test_7)
{
  graph g(2);
  vertex src = *(vertices(g).first);
  vertex dst = *(vertices(g).first + 1);

  edge e1 = boost::add_edge(src, dst, g).first;
  edge e2 = boost::add_edge(src, dst, g).first;

  // Props of edge e1.
  boost::get(boost::edge_weight, g, e1) = 2;
  boost::get(boost::edge_ssc, g, e1).insert(0);

  // Props of edge e2.  During the Dijkstra::Search search, the result for
  // edge e1 will be removed, because edge e2 offers a better result.
  boost::get(boost::edge_weight, g, e2) = 1;
  boost::get(boost::edge_ssc, g, e2).insert(0);

  demand d = demand(npair(src, dst), 1);
  V2C2S result = dijkstra::search(g, d);

  BOOST_CHECK(result[dst].size() == 1);
  BOOST_CHECK(result[dst].begin()->first.first.first == 1);
  BOOST_CHECK(result[dst].begin()->first.second == e2);
}

BOOST_AUTO_TEST_CASE(select_test)
{
  SSC ssc1, ssc2;

  ssc1.insert(1);
  ssc1.insert(2);
  ssc1.insert(3);
  ssc1.insert(4);

  ssc1.insert(5);
  ssc1.insert(6);
  ssc1.insert(7);
  ssc1.insert(8);

  ssc2.insert(0);
  ssc2.insert(1);
  ssc2.insert(2);
  ssc2.insert(3);

  ssc2.insert(7);
  ssc2.insert(8);
  ssc2.insert(9);

  SSSC sssc;
  sssc.insert(ssc1);
  sssc.insert(ssc2);

  {
    dijkstra::get_select() = dijkstra::first;
    SSC ssc = dijkstra::select_ssc(sssc, 2);
    BOOST_CHECK(ssc.size() == 2);
    BOOST_CHECK(*(ssc.begin()) == 0);
    BOOST_CHECK(*(++ssc.begin()) == 1);
  }

  {
    dijkstra::get_select() = dijkstra::fittest;
    SSC ssc = dijkstra::select_ssc(sssc, 2);
    BOOST_CHECK(ssc.size() == 2);
    BOOST_CHECK(*(ssc.begin()) == 7);
    BOOST_CHECK(*(++ssc.begin()) == 8);
  }
}
