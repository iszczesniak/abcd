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

  cout << "usertime simtime seed hash "
    // The network load.
       << "utilization" << " "
    // The probability of establishing a connection.
       << "pec" << " "
    // The mean length of an established connection.
       << "lenec" << " "
    // The mean number of hops of an established connection.
       << "hopec" << " "
    // The mean number of subcarriers of an established connection.
       << "nscec" << " "
    // The number of currently active connections.
       << "conns" << " "
    // The capacity served.
       << "capser" << " "
    // The mean number of fragments on links.
       << "frags" << " "
    // The average time a shortest path search takes.
       << "spsat"
    // That's it.  Thank you.
       << endl;
}

stats &
stats::get()
{
  return *singleton;
}

void
stats::operator()(double st)
{
  cpu_times ttime = ttimer.elapsed();
  cpu_times dtime = dtimer.elapsed();
  // Start again to get next delta time.
  dtimer.start();

  // Total user time.
  double tut = ttime.user / 1e+9;
  // Delta of the user time.
  double dut = dtime.user / 1e+9;

  cout << tut << " " << st << " "
       << args.seed << " " << args.hash << " ";

  // The network utilization.
  cout << calculate_utilization(g) << " ";

  // The probability of establishing a connection.
  cout << ba::mean(pec) << " ";
  // We reset the accumulator to get new means in the next interval.
  pec = dbl_acc();

  // The mean length of an established connection.
  cout << ba::mean(lenec) << " ";
  // We reset the accumulator to get new means in the next interval.
  lenec = dbl_acc();

  // The mean number of hops of an established connection.
  cout << ba::mean(hopec) << " ";
  // We reset the accumulator to get new means in the next interval.
  hopec = dbl_acc();

  // The mean numnber of subcarriers of an established connection.
  cout << ba::mean(nscec) << " ";
  // We reset the accumulator to get new means in the next interval.
  nscec = dbl_acc();

  // The number of active connections.
  cout << tra.nr_clients() << " ";

  // The capacity served.
  cout << tra.capacity_served() << " ";

  // The mean number of fragments of links.
  cout << calculate_frags() << " ";

  // The time spend per search, either successfull or nor.
  cout << dut / ba::count(pec);
      
  // That's it.
  cout << endl;

  schedule(st);
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
  pec(status);
}

void
stats::established_conn(const connection &conn)
{
  int length = conn.get_length();
  int hops = conn.get_hops();
  int nsc = conn.get_nsc();

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
