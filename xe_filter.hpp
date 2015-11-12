#include <boost/graph/filtered_graph.hpp>
#include <set>

using namespace std;

// Exclude edge filter
template <typename Graph>
struct xe_filter
{
  typename Graph::edge_descriptor edge;
  typename std::set<edge> edge_set;

  // The filter must be default-constructible, so it is.
  xe_filter() {};

  xe_filter(edge_set &excluded): m_excluded(excluded) {};

  inline bool operator()(const edge& e) const
  {
    return m_excluded.count(e) == 0;
  }

  edge_set &m_excluded;
};
