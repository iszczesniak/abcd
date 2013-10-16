#ifndef IJS_MODULE
#define IJS_MODULE

#include "event.hpp"

class module
{
  pqueue &q;

public:
  module(pqueue &);
  virtual void operator()(double) = 0;
  void schedule(double);
};

#endif
