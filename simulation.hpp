#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <boost/random.hpp>

#include "event.hpp"
#include "graph.hpp"

// We need to forward-declare the module class.  We cannot include its
// header file, because that header file includes this file
// (simulation.hpp), which will not finally, define the simulation
// class, and the compilation fails.  At this moment the declaration
// of the module class is enough, because we only use a pointer to it.
class module;

class simulation
{
  // The DES priority queue.
  pqueue _q;

  // The graph we're working on.
  graph &_g;

  // The random number generator.
  boost::mt19937 &_rng;

  // The singleton for the class.
  static simulation *_s;

  // Keeps track of the simulation time.
  double _t;
  
public:
  simulation(graph &, boost::mt19937 &);

  // Run the simulation with the given simulation time limit.
  void run(double);

  // Get the singleton.
  static simulation &get();

  // Get the graph
  graph &g() const;

  // Get the rng.
  boost::mt19937 &rng() const;

  // Schedule an event at the given time for the given module.
  void
  schedule(double, module *);
};

#endif /* SIMULATION_HPP */
