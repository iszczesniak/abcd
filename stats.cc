#include "client.hpp"
#include "stats.hpp"
#include "utils_netgen.hpp"

#include <iostream>

using namespace std;

stats *stats::singleton;

stats::stats(const sdi_args &args, const graph &g, pqueue &q,
             const std::vector<client *> &vc):
  module(q), args(args), g(g), vc(vc)
{
  assert(!singleton);
  singleton = this;
  schedule(0);

  cout << "time seed hash "
    // The network load.
       << "load" << " "
    // The probability of establishing a connection.
       << "estab" << " "
    // The probability of completing a connection.
       << "compl" << " "
    // The number of currently supported connections.
       << "conns" << " "
    // The mean number of fragments on a link.
       << "frags" << " "
    // The mean number of links to configure in order to reconfigure a
    // connection
       << "nltoc" << " "
    // The mean length of an established connection.
       << "lofec"
    // That's it.  Thank you.
       << endl;
}

stats *
stats::get()
{
  return singleton;
}

void
stats::operator()(double t)
{
  cout << t << " " << args.seed << " " << args.hash << " ";

  // The network load.
  cout << calculate_load(g) << " ";

  // The probability of establishing a connection.
  cout << ba::mean(cea) << " ";
  // We reset the accumulator to get new means in the next interval.
  cea = dbl_acc();

  // The probability of completing a connection.
  cout << ba::mean(cca) << " ";
  // We reset the accumulator to get new means in the next interval.
  cca = dbl_acc();

  cout << calculate_conns() << " ";
  cout << calculate_frags() << " ";

  // The mean number of links to configure in order to reconfigure a
  // connection.
  cout << ba::mean(nla) << " ";
  // We reset the accumulator to get new means in the next interval.
  nla = dbl_acc();

  // The mean length of an established connection.
  cout << ba::mean(lea);
  // We reset the accumulator to get new means in the next interval.
  lea = dbl_acc();

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
stats::reconfigured_links(int nl)
{
  nla(nl);
}

void
stats::established_length(int len)
{
  lea(len);
}

int
stats::calculate_conns()
{
  int count = 0;

  for(std::vector<client *>::const_iterator i = vc.begin();
      i != vc.end(); ++i)
    {
      client *c = *i;
      if (!c->is_idle())
        ++count;
    }

  return count;
}

double
stats::calculate_frags()
{
  dbl_acc frags;

  // Iterate over all edges.
  boost::graph_traits<graph>::edge_iterator ei, ee;
  for (tie(ei, ee) = boost::edges(g); ei != ee; ++ei)
    {
      const edge e = *ei;
      const SSC &ssc = boost::get(boost::edge_ssc, g, e);
      int f = calculate_fragments(ssc);
      frags(f);
    }

  return ba::mean(frags);
}
