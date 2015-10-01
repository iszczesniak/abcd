#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "connection.hpp"
#include "module.hpp"

#include <boost/random.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>

#include <utility>

namespace ba = boost::accumulators;

class stats;
class traffic;

class client: public module
{
  // The client ID.
  int id;

  // The tear down time.
  double tdt;

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

  // The traffic object the client belongs to.
  traffic &tra;
  
  // The statistics object.
  stats &st;

public:
  client(double mht, double mbst, double mdct, double mnsc, traffic &tra);
  
  // Processes the event and changes the state of the client.
  void operator()(double t);

private:
  void set_up();
  void reconfigure();
  void tear_down();

  // Schedule the next event.
  void schedule_next();

  // Self_destruction: report this to the traffic, and delete itself.
  void destroy();
};

#endif /* CLIENT_HPP */
