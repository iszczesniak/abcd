#include "utils.hpp"

#define BOOST_TEST_MODULE Utils

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(exclude_test)
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

BOOST_AUTO_TEST_CASE(includes_test)
{
  SSC a, b;

  a.insert(0);

  BOOST_CHECK(includes(a, b));

  b.insert(1);

  BOOST_CHECK(!includes(a, b));

  a.insert(1);

  BOOST_CHECK(includes(a, b));
}
