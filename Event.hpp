#ifndef IJS_EVENT
#define IJS_EVENT

#include <queue>

class Source;

class Event
{
  double t;
  Source *s;

public:
  Event(double t, Source *s);
  void process() const;
  bool operator<(const Event &e) const;
};

typedef std::priority_queue<Event> pqueue;

#endif
