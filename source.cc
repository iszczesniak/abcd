#include "source.hpp"
#include <iostream>

using namespace std;

source::source(pqueue &q, int id, boost::mt19937 &r, double lambda):
  q(q), id(id), r(r), ed(lambda), g(r, ed)
{
}

source::~source()
{
  cout << "id = " << id << ", mean = "
       << ba::mean(acc) << endl;
}

void source::operator()(double t)
{
  schedule(t);
}

// Schedule the next event based on the current time 0.
void source::schedule(double t)
{
  double dt = g();
  acc(dt);
  t += dt;
  q.push(event(t, this));
}
