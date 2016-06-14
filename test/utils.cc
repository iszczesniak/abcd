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

  SSSC sssc = split(ssc, 1);
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

BOOST_AUTO_TEST_CASE(find_path_ssc_test)
{
  graph g(3);
  vertex src = *(boost::vertices(g).first);
  vertex mid = *(boost::vertices(g).first + 1);
  vertex dst = *(boost::vertices(g).first + 2);
  edge e1 = boost::add_edge(src, mid, g).first;
  edge e2 = boost::add_edge(mid, dst, g).first;

  // Props of edge e1.
  boost::get(boost::edge_weight, g, e1) = 1;
  boost::get(boost::edge_ssc, g, e1).insert(0);
  boost::get(boost::edge_ssc, g, e1).insert(1);

  // Props of edge e2.
  boost::get(boost::edge_weight, g, e2) = 2;
  boost::get(boost::edge_ssc, g, e2).insert(1);
  boost::get(boost::edge_ssc, g, e2).insert(2);

  // The ssc of an empty path.
  SSC ssc1 = find_path_ssc(g, path());
  BOOST_CHECK(ssc1.empty());

  // The ssc of e1.
  SSC ssc2 = find_path_ssc(g, path{e1});
  BOOST_CHECK(ssc2.size() == 2);
  BOOST_CHECK(ssc2.count(0) == 1);
  BOOST_CHECK(ssc2.count(1) == 1);

  // The ssc of {e1, e2}.
  SSC ssc3 = find_path_ssc(g, path{e1, e2});
  BOOST_CHECK(ssc3.size() == 1);
  BOOST_CHECK(ssc3.count(1) == 1);
}
