#ifndef TRAFFIC_HPP
#define TRAFFIC_HPP

#include <list>
#include <memory>

#include <boost/random.hpp>

#include "client.hpp"
#include "module.hpp"

class traffic: public module
{
  // The list of clients.
  std::list<std::unique_ptr<client>> lc;

  // The random number generator.
  boost::mt19937 &rng;

  // The ID counter.
  int idc;

public:
  // Processes the event and changes the state of the client.
  void operator()(double t);
};

#endif /* TRAFFIC_HPP */
