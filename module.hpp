#ifndef IJS_MODULE
#define IJS_MODULE

#include "event.hpp"
#include <boost/random.hpp>

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>

namespace ba = boost::accumulators;

class module
{
  boost::mt19937 &r;
  boost::exponential_distribution<> ed;
  boost::variate_generator<boost::mt19937 &,
                           boost::exponential_distribution<> > g;

  ba::accumulator_set<double, ba::stats<ba::tag::mean> > acc;

  pqueue &q;
  int id;

public:
  module(pqueue &, int, boost::mt19937 &, double);
  ~module();
  void operator()(double);
  void schedule(double);
};

#endif
