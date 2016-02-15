#ifndef EVENT_HPP
#define EVENT_HPP

#include <queue>

class module;

class event
{
  double t;
  module *m;

public:
  event(double t, module *m);
  void process() const;
  bool operator<(const event &e) const;
  double get_time() const;
};

typedef std::priority_queue<event> pqueue;

#endif /* EVENT_HPP */
