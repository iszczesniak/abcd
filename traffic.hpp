#ifndef TRAFFIC_HPP
#define TRAFFIC_HPP

#include <list>
#include <memory>

#include <boost/random.hpp>

#include "graph.hpp"
#include "module.hpp"

class client;

class traffic: public module
{
  // The graph we are working on.
  graph &g;

  // The list of clients.
  std::list<std::unique_ptr<client>> lc;

  // The random number generator.
  boost::mt19937 &rng;

  // The ID counter.
  int idc;

  // The constructor arguments.
  double mcat, mht, mbst, mdct, mnsc;

public:
  traffic(double mcat, double mht, double mbst,
          double mdct, double mnsc);

  // Processes the event and changes the state of the client.
  void operator()(double t);
};

#endif /* TRAFFIC_HPP */
