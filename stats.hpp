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

  // The network load accumulator.
  ba::accumulator_set<double, ba::stats<ba::tag::mean> > nla;

public:
  stats(graph &g, pqueue &q);
  ~stats();
  static stats *get();
  void schedule(double t);
  void operator()(double t);

  // Reporting functions.
  
};

#endif
