#include "module.hpp"
#include <iostream>

using namespace std;

module::module(pqueue &q, int id, boost::mt19937 &r, double lambda):
  q(q), id(id), r(r), ed(lambda), g(r, ed)
{
}

module::~module()
{
}

void module::operator()(double t)
{
  schedule(t);
}

// Schedule the next event based on the current time 0.
void module::schedule(double t)
{
  double dt = g();
  acc(dt);
  t += dt;
  q.push(event(t, this));
}
