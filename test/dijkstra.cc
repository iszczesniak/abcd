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

  SSC ssc(counting_iterator<int>(0),
          counting_iterator<int>(3));

  V2C2S result = dijkstra(g, src, dst, 3, ssc);

  cout << "size = "<< result[dst].size() << endl;
  cout << "cost = " << result[dst].begin()->first.first << endl;
  cout << "SSC = " << result[dst].begin()->second << endl;

  BOOST_CHECK(result[dst].empty());
}
