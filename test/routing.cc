#include "cdijkstra.hpp"

#include "graph.hpp"
#include "utils.hpp"
#include "utils_netgen.hpp"

#include <iostream>

#define BOOST_TEST_MODULE routing
#include <boost/test/unit_test.hpp>

// This class lets us test the protected functions of routing.
class routing_test: public routing
{
public:
  using routing::select_ssc;
};

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
    routing::set_st("first");
    SSC ssc = routing_test::select_ssc(sssc, 2);
    BOOST_CHECK(ssc.size() == 2);
    BOOST_CHECK(*(ssc.begin()) == 0);
    BOOST_CHECK(*(++ssc.begin()) == 1);
  }

  {
    routing::set_st("fittest");
    SSC ssc = routing_test::select_ssc(sssc, 2);
    BOOST_CHECK(ssc.size() == 2);
    BOOST_CHECK(*(ssc.begin()) == 7);
    BOOST_CHECK(*(++ssc.begin()) == 8);
  }
}
