#ifndef IJS_CLIENT
#define IJS_CLIENT

#include "event.hpp"
#include "module.hpp"

#include <boost/random.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>

namespace ba = boost::accumulators;

class client: public module
{
  boost::mt19937 &r;
  boost::exponential_distribution<> ed;
  boost::variate_generator<boost::mt19937 &,
                           boost::exponential_distribution<> > g;

  // ba::accumulator_set<double, ba::stats<ba::tag::mean> > acc;
  int id;

public:
  client(pqueue &, int, boost::mt19937 &, double);
  ~client();
  void schedule(double t);
  void operator()(double t);
};

#endif
