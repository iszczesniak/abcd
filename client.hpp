#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "event.hpp"
#include "connection.hpp"
#include "graph.hpp"
#include "module.hpp"
#include "stats.hpp"

#include <boost/random.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>

namespace ba = boost::accumulators;

class client: public module
{
  // The graph the client operates on.
  graph &g;

  // The client ID.
  int id;

  // The client random number generator.
  boost::mt19937 &rng;

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

  // The distribution for the number of changes.
  double mnc;
  boost::poisson_distribution<> ncd;
  boost::variate_generator<boost::mt19937 &,
                           boost::poisson_distribution<> > ncdg;

  // The distribution for the number of subcarriers.
  double mnsc;
  boost::poisson_distribution<> nscd;
  boost::variate_generator<boost::mt19937 &,
                           boost::poisson_distribution<> > nscdg;

  // The connection.
  connection conn;

  // The statistics object.
  stats *st;

public:
  /**
   * q is the DES queue, id is the id of the client, r is the random
   * number generator, l_sleep is the lambda of the exponential
   * distribution with mean (1/lambda), mnc is the mean number (for
   * the Poisson distribution) of cell changes that a client makes,
   * l_change is the lambda of the exponential distribution of time
   * between changes.
   */
  client(graph &g, pqueue &q, int id, boost::mt19937 &r,
         double l_sleep, double mnc, double l_change, double mnsc);
  ~client();

  // Schedule the new event based on the current state of the client.
  // This function doesn't change the state of the client.
  void schedule(double t);

  // Processes the event and changes the state of the client.
  void operator()(double t);

private:
  bool set_up();
  bool reconfigure();
  void tear_down();
};

#endif /* CLIENT_HPP */
