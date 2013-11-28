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
  // The graph the stats operates on.
  graph &g;

public:
  stats(graph &g, pqueue &q);
  void schedule(double t);
  void operator()(double t);
};

#endif
