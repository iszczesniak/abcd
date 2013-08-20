#include "utils.hpp"

#define BOOST_TEST_MODULE Utils

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(exclude_test_1)
{
  SSC ssc, r;

  r = exclude(ssc, 2);
  BOOST_CHECK(r.empty());

  ssc.insert(0);

  r = exclude(ssc, 2);
  BOOST_CHECK(r.empty());

  ssc.insert(1);

  r = exclude(ssc, 2);
  BOOST_CHECK(r.size() == 2);

  ssc.insert(3);

  r = exclude(ssc, 2);
  BOOST_CHECK(r.size() == 2);

  ssc.insert(4);

  r = exclude(ssc, 2);
  BOOST_CHECK(r.size() == 4);

  ssc.insert(5);

  r = exclude(ssc, 2);
  BOOST_CHECK(r.size() == 5);
}
