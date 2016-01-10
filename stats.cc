#include "client.hpp"
#include "stats.hpp"
#include "utils_netgen.hpp"

#include <iostream>

using namespace std;

// The singleton of the class.  The compiler initializes it to null.
stats *stats::singleton;

stats::stats(const sdi_args &args, const traffic &tra):
  args(args), tra(tra)
{
  assert(!singleton);
  singleton = this;
  schedule(0);

  cout << "time seed hash "
    // The network load.
       << "utilization" << " "
    // The probability of establishing a connection.
       << "estab" << " "
    // The mean length of an established connection.
       << "lenec" << " "
    // The mean number of hops of an established connection.
       << "hopec" << " "
    // The mean number of subcarriers of an established connection.
       << "nscec" << " "
    // The number of currently active connections.
       << "conns" << " "
    // The mean number of fragments on links.
       << "frags"
    // That's it.  Thank you.
       << endl;
}

stats &
stats::get()
{
  return *singleton;
}

void
stats::operator()(double t)
{
  cout << t << " " << args.seed << " " << args.hash << " ";

  // The network utilization.
  cout << calculate_utilization(g) << " ";

  // The probability of establishing a connection.
  cout << ba::mean(cea) << " ";
  // We reset the accumulator to get new means in the next interval.
  cea = dbl_acc();

  // The mean length of an established connection.
  cout << ba::mean(lenec) << " ";
  // We reset the accumulator to get new means in the next interval.
  lea = dbl_acc();

  // The mean number of hops of an established connection.
  cout << ba::mean(hopec) << " ";
  // We reset the accumulator to get new means in the next interval.
  lea = dbl_acc();

  // The mean numnber of subcarriers of an established connection.
  cout << ba::mean(nscec) << " ";
  // We reset the accumulator to get new means in the next interval.
  lea = dbl_acc();

  // The number of active connections.
  cout << tra.nr_clients() << " ";

  // The mean number of fragments of links.
  cout << calculate_frags();

  // That's it.
  cout << endl;

  schedule(t);
}

// Schedule the next event based on the current time 0.
void
stats::schedule(double t)
{
  // We call the stats every second.
  module::schedule(t + 1);
}

void
stats::established(bool status)
{
  cea(status);
}

void
stats::completed(bool status)
{
  cca(status);
}

void
stats::established_info(int length, int hops, int nsc)
{
  lenec(length);
  hopec(hops);
  nscec(nsc);
}

double
stats::calculate_frags()
{
  dbl_acc frags;

  // Iterate over all edges.
  graph::edge_iterator ei, ee;
  for (tie(ei, ee) = boost::edges(g); ei != ee; ++ei)
    {
      const edge e = *ei;
      const SSC &ssc = boost::get(boost::edge_ssc, g, e);
      int f = calculate_fragments(ssc);
      frags(f);
    }

  return ba::mean(frags);
}
