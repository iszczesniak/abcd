#ifndef EVENT_HPP
#define EVENT_HPP

#include "module.hpp"

template <typename S>
class event
{
  typename S::time_type m_t;
  module<S> *m_m;

public:
  event(typename S::time_type t, module<S> *m): m_t(t), m_m(m)
  {
  }

  void process() const
  {
    (*m_m)(m_t);
  }

  bool operator<(const event<S> &e) const
  {
    return m_t > e.m_t;
  }

  typename S::time_type get_time() const
  {
    return m_t;
  }
};

#endif /* EVENT_HPP */
