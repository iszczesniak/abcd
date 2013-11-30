#ifndef STATS_HPP
#define STATS_HPP

#include "event.hpp"
#include "connection.hpp"
#include "graph.hpp"
#include "module.hpp"

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>

namespace ba = boost::accumulators;

class stats: public module
{
  // The singleton of the class.
  static stats *singleton;
  
  // The graph the stats operates on.
  graph &g;

  // The accumulator with double values.
  typedef ba::accumulator_set<double, ba::stats<ba::tag::mean> > dbl_acc;

  // The network load accumulator.
  dbl_acc nla;

  // The probability of establishing a connection in the interval.
  dbl_acc cea;

  // The total probability of establishing a connection.
  dbl_acc tcea;

  // The probability of completing a connection in the interval.
  dbl_acc cca;

  // The total probability of completing a connection.
  dbl_acc tcca;

public:
  stats(graph &g, pqueue &q);
  ~stats();
  static stats *get();
  void schedule(double t);
  void operator()(double t);

  // Reporting functions.

  // True if the connection was established successfully.
  void established(bool status);

  // True if the connection was completed successfully.
  void completed(bool status);
};

#endif
