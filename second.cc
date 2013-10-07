#include <iostream>
#include <queue>
#include <utility>
#include <boost/tuple/tuple.hpp>

using namespace std;
using namespace boost;

class Source;

typedef priority_queue<pair<double, Source *> > pqueue;

class Source
{
  pqueue &q;
  int counter;

public:
  Source(pqueue &_q) : q(_q), counter(0)
  {
    schedule(0);
  }

  // Handles the event.
  void operator()(double t)
  {
    cout << "Event: t = " << t << endl;

    if (counter < 10)
      {
	// Schedule the next event.
	schedule(t);
	++counter;
      }
  }

  // Schedule the next event based on the current time 0.
  void schedule(double t)
  {
    t += 1;
    q.push(make_pair(t, this));
  }
};

int
main()
{
  pqueue q;

  Source s1(q);

  while(!q.empty())
    {
      double t;
      Source *s;
      tie(t, s) = q.top();
      q.pop();
      (*s)(t);
    }
}
