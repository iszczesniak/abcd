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

#include <utility>

namespace ba = boost::accumulators;

class client: public module
{
  // The graph the client operates on.
  graph &g;

  // The client ID.
  int id;

  // The client random number generator.
  boost::mt19937 &rng;

  // The mean holding time.
  double mht;
  // The holding time distribution.
  boost::exponential_distribution<> htd;
  // The holding time generator.
  boost::variate_generator<boost::mt19937 &,
                           boost::exponential_distribution<> > htg;

  // The mean BS-change time.
  double mbst;
  // The BS-change time distribution.
  boost::exponential_distribution<> bstd;
  // The BS-change time generator.
  boost::variate_generator<boost::mt19937 &,
                           boost::exponential_distribution<> > bstg;
  
  // The mean DC-change time.
  double mdct;
  // The DC-change time distribution.
  boost::exponential_distribution<> dctd;
  // The DC-change time generator.
  boost::variate_generator<boost::mt19937 &,
                           boost::exponential_distribution<> > dctg;

  // The mean number of subcarriers.
  double mnsc;
  // The number of subcarriers distribution.
  boost::poisson_distribution<> nscd;
  // The number of subcarriers generator.
  boost::variate_generator<boost::mt19937 &,
                           boost::poisson_distribution<> > nscdg;

  // The connection.
  connection conn;

  // The statistics object.
  stats *st;

public:
  client(graph &g, pqueue &q, int id, boost::mt19937 &r,
         double mht, double mbst, double mdct, double mnsc);
  ~client();
  
  // Schedule the new event based on the current state of the client.
  // This function doesn't change the state of the client.
  void schedule(double t);

  // Processes the event and changes the state of the client.
  void operator()(double t);

private:
  std::pair<bool, int>
  set_up();

  std::pair<bool, int>
  reconfigure();

  void
  tear_down();
};

#endif /* CLIENT_HPP */
