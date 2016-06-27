#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "connection.hpp"
#include "module.hpp"
#include "sim.hpp"

#include <boost/random.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>

#include <utility>

namespace ba = boost::accumulators;

class stats;
class traffic;

/**
 * The client class.  This class takes care of reporting the stats.
 */
class client: public module<sim>
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
  boost::variate_generator<sim::rng_type &,
                           boost::exponential_distribution<> > htg;

  // The mean number of slices.
  double mnsc;
  // The number of slices distribution.
  boost::poisson_distribution<> nscd;
  // The number of slices generator.
  boost::variate_generator<sim::rng_type &,
                           boost::poisson_distribution<> > nscdg;

  // The distance between the old source and the new source is
  // measured in hops.  The distribution of the number of hops is:
  // Poisson(0.5) + 1, with the mean of 1.5.
  //
  // The mean number of hops.
  const double mnoh = 1.5;
  // The number of hops distribution.
  boost::poisson_distribution<> nohd;
  // The number of hops generator.
  boost::variate_generator<sim::rng_type &,
                           boost::poisson_distribution<> > nohdg;

  // The connection.
  connection conn;

  // The traffic object the client belongs to.
  traffic &tra;
  
  // The statistics object.
  stats &st;

public:
  client(double mht, double mnsc, traffic &tra);
  
  // Processes the event and changes the state of the client.
  void operator()(double t);

  const connection &
  get_connection() const;

private:
  bool set_up();
  bool reconfigure();
  void destroy();

  // Finds a new destination vertex based on the current destination
  // vertex.  This new source vertex will be used for connection
  // reconfiguration.
  vertex get_new_dst();
};

#endif /* CLIENT_HPP */
