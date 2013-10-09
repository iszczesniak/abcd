#include "Source.hpp"
#include <iostream>

using namespace std;

Source::Source(pqueue &q, int id) : q(q), id(id)
{
}

void Source::operator()(double t)
{
  schedule(t);
}

// Schedule the next event based on the current time 0.
void Source::schedule(double t)
{
  t += 1;
  cout << "Push t = " << t
       << ", id = " << id
       << ", this = " << this << endl;
  q.push(Event(t, this));
}
