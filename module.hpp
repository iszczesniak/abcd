#ifndef MODULE_HPP
#define MODULE_HPP

#include "simulation.hpp"

// This is the abstract class for modules.  A module processes events
// with the operator() function, which takes only the simulation time,
// and so the module is supposed to know why it's called.  An event is
// scheduled with the schedule function.
class module
{
protected:
  // The simulation object.
  simulation &sim;

  // The graph of the simulation.
  graph &g;
  
  // The random number generator.
  boost::mt19937 &rng;
  
public:
  module();
  // We need the destructor to be virtual.
  virtual ~module();

  // This function processes an event for this module.
  virtual void operator()(double t) = 0;

  // Return the current time of the simulation.
  double now();

  // Schedule an event for this module at the current time.
  void schedule();

  // Schedule an event for this module at time t.
  void schedule(double t);
};

#endif /* MODULE_HPP */
