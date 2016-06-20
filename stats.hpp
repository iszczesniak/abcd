#ifndef STATS_HPP
#define STATS_HPP

#include "event.hpp"
#include "connection.hpp"
#include "graph.hpp"
#include "module.hpp"
#include "sdi_args.hpp"
#include "sim.hpp"
#include "traffic.hpp"

#include <vector>

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>
#include <boost/timer/timer.hpp>

namespace ba = boost::accumulators;
using boost::timer::cpu_timer;
using boost::timer::cpu_times;

class client;

class stats: public module<sim>
{
  // The singleton of the class.
  static stats *singleton;

  // The traffic of the run.
  const traffic &tra;

  // The accumulator type with double values.
  typedef ba::accumulator_set<double, ba::features<ba::tag::count,
                                                   ba::tag::mean>> dbl_acc;

  // The probability of establishing a connection.
  dbl_acc pec;
  // The length of the established connection.
  dbl_acc lenec;
  // The number of hops of the established connection.
  dbl_acc hopec;
  // The number of slices of the established connection.
  dbl_acc nscec;

  // The probability of reconfiguring a connection.
  dbl_acc m_prc;
  // The number of links to reconfigure a connection.
  dbl_acc m_nolrc;

  // The arguments of the run.
  sdi_args args;
  // The total timer, i.e., keeps track from the beginning of the run.
  cpu_timer ttimer;
  // The delta timer, i.e., keeps track from the last report time.
  cpu_timer dtimer;
  
public:
  stats(const sdi_args &, const traffic &);

  static stats &
  get();

  void
  schedule(double t);

  void
  operator()(double t);

  // Report the establishment status.
  void
  established(bool status);

  // Report the established connection.
  void
  established_conn(const connection &conn);

  // Report the reconfiguration status.
  void
  reconfigured(bool status);

  // Report the reconfigured connection.
  void
  reconfigured_conn(int nolrc);

private:
  // Calculate the average number of fragments on a link.
  double
  calculate_frags();
};

#endif
