#include "utils.hpp"
#include "graph.hpp"

#include <list>
#include <set>

#include <boost/graph/connected_components.hpp>

using namespace std;

bool
includes(const SSC &a, const SSC &b)
{
  includes(a.begin(), a.end(), b.begin(), b.end());
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
exclude(const SSC &ssc, int p)
{
  SSC result;

  SSC::const_iterator i = ssc.begin();

  while(i != ssc.end())
    {
      SSC partial;

      // This loop builds the "partial" set with contiguous
      // subcarriers.  In an interation we examine a single
      // subcarrier.
      while(i != ssc.end())
        {
          int sc = *i;

          // The break condition for this loop, i.e. the condition to
          // stop building the partial set.  If we break the loop, we
          // don't add the sc element and don't increment i.
          if (!partial.empty() && ((*partial.rbegin() + 1) != sc))
            break;

          partial.insert(sc);
          ++i;
        }

      // Add the partial set only if has at least p subcarriers.
      // Otherwise just ignore it.
      if (partial.size() >= p)
        result.insert(partial.begin(), partial.end());
    }

  return result;
}

/**
 * Exclude e from ssc.
 */
void
exclude(SSC &ssc, const SSC &e)
{
  for(SSC::const_iterator i = e.begin(); i != e.end(); ++i)
    ssc.erase(*i);
}

// The function for sorting the list of sets.
static bool stlos(const set<Vertex> &s1, const set<Vertex> &s2)
{
  return s1.size() > s2.size();
}

list<set<Vertex> >
get_components(const Graph &g)
{
  // An element of this vector corresponds to a vertex, i.e. c[v] is
  // the component number that vertex v belongs to.
  std::vector<int> c(num_vertices(g));
  // "num" is the number of connected components.
  int num = connected_components(g, &c[0]);

  // Each element of the list is a set that contains vertexes
  // belonging to a component.
  list<set<Vertex> > l(num);
  // In every iteration add one vertex to the appropriate set.
  for (std::vector<int>::iterator i = c.begin(); i != c.end(); ++i)
    {
      list<set<Vertex> >::iterator li = l.begin();
      advance(li, *i);

      // Iterator i refers to the element for vertex v.
      Vertex v = distance(c.begin(), i);
      li->insert(v);
    }

  l.sort(stlos);

  return l;
}
