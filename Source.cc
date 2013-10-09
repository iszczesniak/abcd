#include "Source.hpp"
#include <iostream>

using namespace std;

Source::Source(pqueue &q, int id) : q(q), id(id), count(0)
{
  cout << "Constructor id = " << id << ", this = " << this << endl;
}

Source::Source(const Source &s): q(s.q), id(s.id), count(s.count)
{
  cout << "Copying id = " << id << ", this = " << this << endl;
}

void Source::operator()(double t)
{
  if (++count < 10)
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
