#ifndef IJS_EVENT
#define IJS_EVENT

#include <queue>

class source;

class event
{
  double t;
  source *s;

public:
  event(double t, source *s);
  void process() const;
  bool operator<(const event &e) const;
};

typedef std::priority_queue<event> pqueue;

#endif
