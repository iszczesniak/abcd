#include "graph.hpp"

namespace std {

  bool operator <(const Edge &e1, const Edge &e2)
  {
    if (e1.m_source == e2.m_source)
      return e1.m_target < e2.m_target;

    return e1.m_source < e2.m_source;
  }

  bool operator ==(const Edge &e1, const Edge &e2)
  {
    return e1.m_source == e2.m_source && e1.m_target == e2.m_target;
  }

}
