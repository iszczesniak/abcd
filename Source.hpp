#ifndef IJS_SOURCE
#define IJS_SOURCE

#include "Event.hpp"

class Source
{
  pqueue &q;
  int id;
  int count;

public:
  Source(pqueue &q, int id);
  Source(const Source &s);
  void operator()(double t);
  void schedule(double t);
};

#endif
