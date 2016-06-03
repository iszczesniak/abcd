#include "utils.hpp"
#include "graph.hpp"

#include <algorithm>
#include <list>
#include <set>

#include <boost/graph/connected_components.hpp>

using namespace std;

bool
includes(const SSC &a, const SSC &b)
{
  return includes(a.begin(), a.end(), b.begin(), b.end());
}

bool
excludes(const SSC &a, const SSC &b)
{
  return intersection(a, b).empty();
}

SSC
intersection(const SSC &a, const SSC &b)
{
  SSC r;

  set_intersection(a.begin(), a.end(),
                   b.begin(), b.end(),
                   inserter(r, r.begin()));

  return r;
}

SSC
exclude(const SSC &ssc, int nsc)
{
  SSC result;

  SSC::const_iterator i = ssc.begin();

  while(i != ssc.end())
    {
      SSC partial;

      // This loop builds the "partial" set with contiguous
      // slices.  In an interation we examine a single
      // slice.
      while(i != ssc.end())
        {
          int sc = *i;

          // The break condition for this loop, i.e. the condition to
          // stop building the partial set, because the previously
          // inserted element is not one less small than sc.  If we
          // break the loop, we don't add the sc element and don't
          // increment i.  The element pointed to by i will be
          // inserted into a new partial set.
          if (!partial.empty() && ((*partial.rbegin() + 1) != sc))
            break;

          partial.insert(sc);
          ++i;
        }

      // Add the partial set only if has at least p slices.
      // Otherwise just ignore it.
      if (partial.size() >= nsc)
        result.insert(partial.begin(), partial.end());
    }

  return result;
}

void
exclude(SSC &ssc, const SSC &e)
{
  for(SSC::const_iterator i = e.begin(); i != e.end(); ++i)
    ssc.erase(*i);
}

void
include(SSC &ssc, const SSC &e)
{
  for(SSC::const_iterator i = e.begin(); i != e.end(); ++i)
    ssc.insert(*i);
}

SSC
find_path_ssc(const graph &g, const path &p)
{
  SSC ssc;
  
  path::const_iterator i = p.begin();

  if (i != p.end())
    {
      ssc = boost::get(boost::edge_ssc, g, *i);

      while(++i != p.end())
        ssc = intersection(ssc, boost::get(boost::edge_ssc, g, *i));
    }

  return ssc;
}

// The function for sorting the list of sets.
static bool stlos(const set<vertex> &s1, const set<vertex> &s2)
{
  return s1.size() > s2.size();
}

SSSC
split(const SSC &ssc, int nsc)
{
  SSSC sssc;

  for(SSC::const_iterator i = ssc.begin(); i != ssc.end();)
    {
      SSC tmp;
      int sc;

      do
        {
          sc = *i;
          tmp.insert(sc);
          ++i;
        } while (i != ssc.end() && sc + 1 == *i);

      // Return this SSC only if it has at least nsc slices.
      if (tmp.size() >= nsc)
        sssc.insert(tmp);
    }

  return sssc;
}

int
calculate_fragments(const SSC &ssc)
{
  // Split the SSC into fragments of size 1 at least.
  return split(ssc, 1).size();
}

/**
 * This is the << operator for a sscpath.
 */
std::ostream &operator << (std::ostream &os, const sscpath &p)
{
  os << "sscpath(" << p.first << ", " << p.second << ")";
}

/**
 * This is the << operator for a CEV.
 */
std::ostream &operator << (std::ostream &os, const CEV &cev)
{
  os << "CEV("
     << get<0>(cev) << ", "
     << get<1>(cev) << ", "
     << get<2>(cev) << ")";
}
