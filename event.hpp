#ifndef IJS_EVENT
#define IJS_EVENT

#include <queue>

class module;

class event
{
  double t;
  module *s;

public:
  event(double t, module *s);
  void process() const;
  bool operator<(const event &e) const;
  double get_time() const;
};

typedef std::priority_queue<event> pqueue;

#endif
