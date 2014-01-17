#ifndef MODULE_HPP
#define MODULE_HPP

#include "event.hpp"

class module
{
  pqueue &q;

public:
  module(pqueue &);
  virtual ~module();
  virtual void operator()(double) = 0;
  void schedule(double);
};

#endif /* MODULE_HPP */
