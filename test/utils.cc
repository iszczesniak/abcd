#include "utils.hpp"

#define BOOST_TEST_MODULE Utils

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(exclude_test_1)
{
  SSC ssc;
  ssc.insert(0);
  ssc.insert(1);

  SSC r = exclude(ssc, 3);
  BOOST_CHECK(r.empty());
}

BOOST_AUTO_TEST_CASE(exclude_test_2)
{
  BOOST_CHECK(true);
}
