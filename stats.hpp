#ifndef STATS_HPP
#define STATS_HPP

#include "event.hpp"
#include "connection.hpp"
#include "graph.hpp"
#include "module.hpp"
#include "sdi_args.hpp"

#include <vector>

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>

namespace ba = boost::accumulators;

class client;

class stats: public module
{
  // The singleton of the class.
  static stats *singleton;
  
  // The graph the stats operates on.
  const graph &g;

  // Vector of clients.
  const std::vector<client *> &vc;

  // The accumulator with double values.
  typedef ba::accumulator_set<double, ba::stats<ba::tag::mean> > dbl_acc;

  // The probability of establishing a connection in the interval.
  dbl_acc cea;

  // The probability of completing a connection in the interval.
  dbl_acc cca;

  // The accummulator for the mean number of links to configure in
  // order to reconfigure a connection.
  dbl_acc nla;

  // The accumulator for the length of the established connection.
  dbl_acc lea;

  // The arguments of the run.
  sdi_args args;

public:
  stats(const sdi_args &args, const graph &g, pqueue &q,
        const std::vector<client *> &vc);

  static stats *
  get();

  void
  schedule(double t);

  void
  operator()(double t);

  // Report success if the connection was established successfully.
  void
  established(bool status);

  // Report succes if the connection was completed successfully.
  void
  completed(bool status);

  // Report the length of the established connection.
  void
  established_length(int len);

  // Report the number of links that have to be reconfigured when a
  // connection is to be reconfigured.
  void
  reconfigured_links(int nl);

private:
  // Calculate the number of currently active clients.
  int
  calculate_conns();

  // Calculate the average number of fragments on a link.
  double
  calculate_frags();
};

#endif
