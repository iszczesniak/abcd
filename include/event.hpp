#ifndef EVENT_HPP
#define EVENT_HPP

#include <queue>

class module;

template<typename S>
class event
{
  S::time_type t;
  module<S> *m;

public:
  event(double t, module *m);
  void process() const;
  bool operator<(const event<S> &e) const;
  double get_time() const;
};

typedef std::priority_queue<event> pqueue;

#endif /* EVENT_HPP */
