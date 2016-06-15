#define BOOST_TEST_MODULE Utils

#include "utils.hpp"

#include "sample_graphs.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/random/linear_congruential.hpp>

#include <set>
#include <tuple>
#include <vector>

using namespace std;

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
  graph g;
  vector<vertex> vs;
  vector<edge> es;
  sample_graph1(g, vs, es);

  // The ssc of an empty path.
  SSC ssc1 = find_path_ssc(g, path());
  BOOST_CHECK(ssc1.empty());

  // The ssc of es[0].
  SSC ssc2 = find_path_ssc(g, path{es[0]});
  BOOST_CHECK(ssc2.size() == 2);
  BOOST_CHECK(ssc2.count(0) == 1);
  BOOST_CHECK(ssc2.count(1) == 1);

  // The ssc of {es[0], es[1]}.
  SSC ssc3 = find_path_ssc(g, path{es[0], es[1]});
  BOOST_CHECK(ssc3.size() == 1);
  BOOST_CHECK(ssc3.count(1) == 1);
}

BOOST_AUTO_TEST_CASE(find_vertexes_test)
{
  graph g;
  vector<vertex> vs;
  vector<edge> es;
  sample_graph1(g, vs, es);

  set<vertex> found0 = find_vertexes(g, vs[0], 0);
  BOOST_CHECK(found0.size() == 1);
  BOOST_CHECK(found0.count(vs[0]) == 1);

  set<vertex> found1 = find_vertexes(g, vs[0], 1);
  BOOST_CHECK(found1.size() == 1);
  BOOST_CHECK(found1.count(vs[1]) == 1);

  set<vertex> found2 = find_vertexes(g, vs[0], 2);
  BOOST_CHECK(found2.size() == 1);
  BOOST_CHECK(found2.count(vs[2]) == 1);

  set<vertex> found3 = find_vertexes(g, vs[0], 3);
  BOOST_CHECK(found3.empty());

  set<vertex> found4 = find_vertexes(g, vs[1], 1);
  BOOST_CHECK(found4.size() == 2);
  BOOST_CHECK(found4.count(vs[0]) == 1);
  BOOST_CHECK(found4.count(vs[2]) == 1);
}
