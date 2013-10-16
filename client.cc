#include "client.hpp"
#include <iostream>

using namespace std;

client::client(pqueue &q, int id, boost::mt19937 &r, double lambda):
  module(q), id(id), r(r), ed(lambda), g(r, ed)
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
  double dt = g();
  t += dt;
  module::schedule(t);
}
