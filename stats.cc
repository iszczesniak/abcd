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

  // The user part of the wall clock time.
  cout << "usertime" << " ";
  // The time elapsed in the simulation.
  cout << "simtime" << " ";
  // The seed of the simulation.
  cout << "seed" << " ";
  // The hash of the simulation.
  cout << "hash" << " ";

  // The mean connection arrival time.
  cout << "mcat" << " ";
  // The offered load.
  cout << "oload" << " ";
  // The network utilization.
  cout << "utilization" << " ";

  // The probability of establishing a connection.
  cout << "pec" << " ";
  // The mean length of an established connection.
  cout << "lenec" << " ";
  // The mean number of links of an established connection.
  cout << "nolec" << " ";
  // The mean number of slices of an established connection.
  cout << "nscec" << " ";

  // The probability of reconfiguring a connection.
  cout << "prc" << " ";
  // The mean length of a reconfigured connection.
  cout << "lenrc" << " ";
  // The mean number of links of a reconfigured connection.
  cout << "nolrc" << " ";
  // The mean number of slices of a reconfigured connection.
  cout << "nscrc" << " ";
  // The mean number of new links used in reconfiguration.
  cout << "newrc" << " ";
  // The mean number of old links used in reconfiguration.
  cout << "oldrc" << " ";

  // The number of currently active connections.
  cout << "conns" << " ";
  // The capacity served.
  cout << "capser" << " ";
  // The mean number of fragments on links.
  cout << "frags" << " ";
  // The average time a shortest path search (successful or not) takes.
  cout << "spsat";
  // That's it.  Thank you.
  cout << endl;
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

  // Total user time.
  double tut = ttime.user / 1e+9;
  // Delta of the user time.
  double dut = dtime.user / 1e+9;

  // The user part of the wall clock time.
  cout << tut << " ";
  // The time elapsed in the simulation.
  cout << st << " ";
  // The seed of the simulation.
  cout << args.seed << " ";
  // The hash of the simulation.
  cout << args.hash << " ";

  // The mean connection arrival time.
  cout << args.mcat << " ";
  // The offered load.
  cout << args.ol << " ";
  // The network utilization.
  cout << calculate_utilization(m_mdl) << " ";

  // The probability of establishing a connection.
  cout << ba::mean(m_pec) << " ";
  // The mean length of an established connection.
  cout << ba::mean(m_lenec) << " ";
  // The mean number of links of an established connection.
  cout << ba::mean(m_nolec) << " ";
  // The mean number of slices of an established connection.
  cout << ba::mean(m_nscec) << " ";

  // The mean probability of reconfiguring a connection.
  cout << ba::mean(m_prc) << " ";
  // The mean length of a reconfigured connection.
  cout << ba::mean(m_lenrc) << " ";
  // The mean number of links of a reconfigured connection.
  cout << ba::mean(m_nolrc) << " ";
  // The mean number of slices of a reconfigured connection.
  cout << ba::mean(m_nscrc) << " ";
  // The mean number of new links used in reconfiguration.
  cout << ba::mean(m_newrc) << " ";
  // The mean number of old links used in reconfiguration.
  cout << ba::mean(m_oldrc) << " ";

  // The number of currently active connections.
  cout << tra.nr_clients() << " ";
  // The capacity served.
  cout << tra.capacity_served() << " ";
  // The mean number of fragments of links.
  cout << calculate_frags() << " ";
  // The average time a shortest path search (successful or not) takes.
  cout << dut / ba::count(m_pec) << " ";
  // That's it.  Thanks!
  cout << endl;

  // We reset the accumulators to get new means in the next interval.
  m_pec = dbl_acc();
  m_lenec = dbl_acc();
  m_nolec = dbl_acc();
  m_nscec = dbl_acc();
  m_prc = dbl_acc();
  m_lenrc = dbl_acc();
  m_nolrc = dbl_acc();
  m_nscrc = dbl_acc();
  m_newrc = dbl_acc();
  m_oldrc = dbl_acc();

  // Start again to get next delta time.
  dtimer.start();

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
  m_pec(status);
}

void
stats::established_conn(const connection &conn)
{
  int len = conn.get_len();
  int nol = conn.get_nol();
  int nsc = conn.get_nsc();

  m_lenec(len);
  m_nolec(nol);
  m_nscec(nsc);
}

void
stats::reconfigured(bool status)
{
  m_prc(status);
}

void
stats::reconfigured_conn(const connection &conn, int newrc, int oldrc)
{
  int len = conn.get_len();
  int nol = conn.get_nol();
  int nsc = conn.get_nsc();

  m_lenrc(len);
  m_nolrc(nol);
  m_nscrc(nsc);
  m_newrc(newrc);
  m_oldrc(oldrc);
}

double
stats::calculate_frags()
{
  dbl_acc frags;

  // Iterate over all edges.
  graph::edge_iterator ei, ee;
  for (tie(ei, ee) = boost::edges(m_mdl); ei != ee; ++ei)
    {
      const edge e = *ei;
      const SSC &ssc = boost::get(boost::edge_ssc, m_mdl, e);
      int f = calculate_fragments(ssc);
      frags(f);
    }

  return ba::mean(frags);
}
