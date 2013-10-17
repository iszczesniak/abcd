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
  // The client ID.
  int id;

  // The client random number generator.
  boost::mt19937 &r;

  // Is the client idle?
  bool idle;

  // The number of changes left to make.
  int nc_left;

  // Sleep distribution.
  double l_sleep;
  boost::exponential_distribution<> sd;
  boost::variate_generator<boost::mt19937 &,
                           boost::exponential_distribution<> > sdg;

  // Change distribution.
  double l_change;
  boost::exponential_distribution<> cd;
  boost::variate_generator<boost::mt19937 &,
                           boost::exponential_distribution<> > cdg;

  // The distribution for the nubmer of changes.
  double mnc;

  // ba::accumulator_set<double, ba::stats<ba::tag::mean> > acc;

public:
  /**
   * q is the DES queue, id is the id of the client, r is the random
   * number generator, l_sleep is the lambda of the exponential
   * distribution with mean (1/lambda), mnc is the mean number (for
   * the Poisson distribution) of cell changes that a client makes,
   * l_change is the lambda of the exponential distribution of time
   * between changes.
   */
  client(pqueue &q, int id, boost::mt19937 &r,
         double l_sleep, double mnc, double l_change);
  ~client();
  void schedule(double t);
  void operator()(double t);
};

#endif
