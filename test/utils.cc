#include "utils.hpp"

#define BOOST_TEST_MODULE Utils

#include <boost/test/unit_test.hpp>
#include <boost/random/linear_congruential.hpp>

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

BOOST_AUTO_TEST_CASE(exclude2_test)
{
  SSC ssc, e;

  ssc.insert(0);
  ssc.insert(1);
  ssc.insert(2);
  ssc.insert(3);

  e.insert(0);
  e.insert(3);

  exclude(ssc, e);

  BOOST_CHECK(ssc.size() == 2);
  BOOST_CHECK(ssc.find(1) != ssc.end());
  BOOST_CHECK(ssc.find(2) != ssc.end());
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

BOOST_AUTO_TEST_CASE(get_random_int_test)
{
  boost::minstd_rand gen;
  for(int i = 0; i < 10; ++i)
    for(int j = 0; j < 10; ++j)
      {
	int n = get_random_int(0, j, gen);
	BOOST_CHECK(0 <= n && n <= j);
      }
}
