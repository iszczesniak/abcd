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

BOOST_AUTO_TEST_CASE(select_test1)
{
  SSC ssc;

  ssc.insert(0);
  ssc.insert(1);
  ssc.insert(2);
  ssc.insert(3);

  ssc.insert(7);
  ssc.insert(8);
  ssc.insert(9);

  {
    routing::set_st("first");
    SSC found = routing_test::select_ssc(ssc, 2);
    BOOST_CHECK(found.size() == 2);
    BOOST_CHECK(*(found.begin()) == 0);
    BOOST_CHECK(*(++found.begin()) == 1);
  }

  {
    routing::set_st("fittest");
    SSC found = routing_test::select_ssc(ssc, 2);
    BOOST_CHECK(found.size() == 2);
    BOOST_CHECK(*(found.begin()) == 7);
    BOOST_CHECK(*(++found.begin()) == 8);
  }
}

BOOST_AUTO_TEST_CASE(select_test2)
{
  SSC ssc1(boost::counting_iterator<int>(0),
           boost::counting_iterator<int>(100));

  for (int i = 0; i < 1000; ++i)
    SSC ssc2 = exclude(ssc1, 2);
}
