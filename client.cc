#include "client.hpp"
#include <iostream>

using namespace std;

client::client(pqueue &q, int id, boost::mt19937 &r,
               double l_sleep, double mnc, double l_change):
  module(q), id(id), r(r),
  idle(true), nc_left(0),
  l_sleep(l_sleep), sd(l_sleep), sdg(r, sd),
  l_change(l_change), cd(l_change), cdg(r, cd),
  mnc(mnc)
{
}

client::~client()
{
}

void client::operator()(double t)
{
  schedule(t);
}

// Schedule the next event based on the current time 0.
void client::schedule(double t)
{
  //  double dt = g();
  //  t += dt;
  module::schedule(t);
}
