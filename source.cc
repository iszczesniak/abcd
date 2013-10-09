#include "source.hpp"
#include <iostream>

using namespace std;

source::source(pqueue &q, int id) : q(q), id(id)
{
}

void source::operator()(double t)
{
  schedule(t);
}

// Schedule the next event based on the current time 0.
void source::schedule(double t)
{
  t += 1;
  cout << "Push t = " << t
       << ", id = " << id
       << ", this = " << this << endl;
  q.push(event(t, this));
}
