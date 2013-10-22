#include "graph.hpp"

namespace std {

  bool operator <(const edge &e1, const edge &e2)
  {
    if (e1.m_source == e2.m_source)
      return e1.m_target < e2.m_target;

    return e1.m_source < e2.m_source;
  }

  bool operator ==(const edge &e1, const edge &e2)
  {
    return e1.m_source == e2.m_source && e1.m_target == e2.m_target;
  }

}
