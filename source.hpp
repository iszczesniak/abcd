#ifndef IJS_SOURCE
#define IJS_SOURCE

#include "event.hpp"

class source
{
  pqueue &q;
  int id;

public:
  source(pqueue &q, int id);
  void operator()(double t);
  void schedule(double t);
};

#endif
