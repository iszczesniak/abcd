#ifndef IJS_SOURCE
#define IJS_SOURCE

#include "Event.hpp"

class Source
{
  pqueue &q;
  int id;

public:
  Source(pqueue &q, int id);
  void operator()(double t);
  void schedule(double t);
};

#endif
