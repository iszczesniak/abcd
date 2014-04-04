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
  for(int i = 0; i < 100; ++i)
    for(int j = 0; j < 10; ++j)
      {
	int n = get_random_int(0, j, gen);
	BOOST_CHECK(0 <= n && n <= j);
      }
}

BOOST_AUTO_TEST_CASE(calculate_fragments_test)
{
  SSC ssc;
  BOOST_CHECK(calculate_fragments(ssc) == 0);

  ssc.insert(0);
  BOOST_CHECK(calculate_fragments(ssc) == 1);

  ssc.insert(1);
  BOOST_CHECK(calculate_fragments(ssc) == 1);

  ssc.insert(3);
  BOOST_CHECK(calculate_fragments(ssc) == 2);

  ssc.insert(5);
  BOOST_CHECK(calculate_fragments(ssc) == 3);

  ssc.insert(6);
  BOOST_CHECK(calculate_fragments(ssc) == 3);

  SSSC sssc = split(ssc);
  BOOST_CHECK(sssc.size() == 3);
  SSSC::iterator i = sssc.begin();
  BOOST_CHECK(i->size() == 2);
  BOOST_CHECK(*(i->begin()) == 0);
  BOOST_CHECK(*(++(i->begin())) == 1);
  ++i;
  BOOST_CHECK(i->size() == 1);
  BOOST_CHECK(*(i->begin()) == 3);
  BOOST_CHECK(*(++(i->begin())) == 1);
  ++i;
  BOOST_CHECK(i->size() == 2);
  BOOST_CHECK(*(i->begin()) == 5);
  BOOST_CHECK(*(++(i->begin())) == 6);
}

// A trivial test.

BOOST_AUTO_TEST_CASE(includes_test2)
{
  SSSC a, b;

  BOOST_CHECK(includes(a, b));

  SSC ssc1;
  ssc1.insert(1);
  a.insert(ssc1);

  BOOST_CHECK(includes(a, b));

  b.insert(ssc1);

  BOOST_CHECK(includes(a, b));
}

// SSSC b is a subset of SSSC a.

BOOST_AUTO_TEST_CASE(includes_test3)
{
  SSSC a, b;

  SSC ssc1, ssc2;
  ssc1.insert(1);
  ssc1.insert(2);
  ssc2.insert(1);

  a.insert(ssc1);
  b.insert(ssc2);

  BOOST_CHECK(includes(a, b));
}

// SSSC b is not a subset of SSSC a.

BOOST_AUTO_TEST_CASE(includes_test4)
{
  SSSC a, b;

  SSC ssc1, ssc2;
  ssc1.insert(1);
  ssc2.insert(1);
  ssc2.insert(2);

  a.insert(ssc1);
  b.insert(ssc2);

  BOOST_CHECK(!includes(a, b));
}

// SSSC b is not a subset of SSSC a.

BOOST_AUTO_TEST_CASE(includes_test5)
{
  SSSC a, b;

  SSC ssc1, ssc2, ssc3;
  ssc1.insert(1);
  ssc2.insert(1);
  ssc2.insert(2);
  ssc3.insert(3);

  a.insert(ssc1);
  a.insert(ssc2);
  b.insert(ssc3);

  BOOST_CHECK(!includes(a, b));
}
