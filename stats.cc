#include "stats.hpp"
#include "utils_netgen.hpp"

#include <iostream>

using namespace std;

stats *stats::singleton;

stats::stats(const sdi_args &args, const graph &g, pqueue &q):
  args(args), g(g), module(q)
{
  assert(!singleton);
  singleton = this;
  schedule(0);

  cout << "time seed "
    // network parameters
       << "nodes edges subcarriers clients "
    // load parameters
       << "l_sleep mnc l_change "
    // the network load
       << "load "
    // the probability of establishing a connection
       << "estab "
    // the probability of completing a connection
       << "compl" << endl;
}

stats *
stats::get()
{
  return singleton;
}

void stats::operator()(double t)
{
  cout << t << " " << args.seed << " "
       << args.nr_nodes << " " << args.nr_edges << " "
       << args.nr_sc << " " << args.nr_clients << " "
       << args.l_sleep << " " << args.mnc << " " << args.l_change << " ";

  // The network load.
  cout << calculate_load(g) << " ";

  // The probability of establishing a connection.
  cout << ba::mean(cea) << " ";

  // The probability of completing a connection.
  cout << ba::mean(cca) << endl;

  schedule(t);
}

// Schedule the next event based on the current time 0.
void stats::schedule(double t)
{
  // We call the stats every second.
  module::schedule(t + 1);
}

void stats::established(bool status)
{
  cea(status);
}

void stats::completed(bool status)
{
  cca(status);
}
