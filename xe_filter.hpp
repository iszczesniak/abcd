#ifndef XE_FILTER
#define XE_FILTER

#include <set>

using namespace std;

// Exclude edge filter
template <typename Graph>
struct xe_filter
{
  typedef typename Graph::edge_descriptor edge_descriptor;
  typedef typename std::set<edge_descriptor> edge_set;

  // The filter must be default-constructible, so it is.
  xe_filter() {};

  xe_filter(const edge_set *excluded): m_excluded(excluded) {};

  inline bool operator()(const edge &e) const
  {
    return m_excluded->count(e) == 0;
  }

  const edge_set *m_excluded;
};

#endif /* XE_FILTER */
