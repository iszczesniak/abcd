#ifndef MODULE_HPP
#define MODULE_HPP

#include "simulation.hpp"

// This is the abstract class for modules.  A module processes events
// with the operator() function, which takes only the simulation time,
// and so the module is supposed to know why it's called.  An event is
// scheduled with the schedule function.
class module
{
  simulation &sim;

public:
  module();
  // We need the destructor to be virtual.
  virtual ~module();

  // This function processes an event for this module.
  virtual void operator()(double t) = 0;

  // Schedule an event for this module at time t.
  void schedule(double t);
};

#endif /* MODULE_HPP */
