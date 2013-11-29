#include "stats.hpp"
#include "utils_netgen.hpp"

#include <iostream>

using namespace std;

stats *stats::singleton;

stats::stats(graph &g, pqueue &q):
  g(g), module(q)
{
  assert(!singleton);
  singleton = this;
  schedule(0);
}

stats::~stats()
{
  cout << "stats at exit: " << endl;
  cout << "mean network load = " << ba::mean(nla) << endl;
}

stats *
stats::get()
{
  return singleton;
}

void stats::operator()(double t)
{
  cout << "stats at t = " << t << ": ";

  double nl = calculate_load(g);
  nla(nl);
  cout << "load = " << nl;

  cout << endl;

  schedule(t);
}

// Schedule the next event based on the current time 0.
void stats::schedule(double t)
{
  // We call the stats every second.
  module::schedule(t + 1);
}

