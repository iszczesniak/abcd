#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <memory>

#include <boost/random.hpp>

#include "client.hpp"
#include "graph.hpp"
#include "module.hpp"

class simulation
{
  // The DES priority queue.
  pqueue q;

  // The graph we're working on.
  graph &g;

  // The random number generator.
  boost::mt19937 &rng;

  // The singleton for the class.
  static std::unique_ptr<simulation> s;
  
public:
  traffic(graph &g);

  static simulation &get();
};

#endif /* SIMULATION_HPP */
