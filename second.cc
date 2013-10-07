#include <iostream>
#include <list>
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
  int id;

public:
  Source(pqueue &_q, int _id) : q(_q), counter(0), id(_id)
  {
    cout << "Constructor id = " << id << endl;
    schedule(0);
  }

  // Handles the event.
  void operator()(double t)
  {
    cout << "Event: t = " << t << ", id = " << id
	 << ", this = " << this << endl;

    //    if (counter < 10)
    //      {
    // Schedule the next event.
    //	schedule(t);
    //	++counter;
    //      }
  }

  // Schedule the next event based on the current time 0.
  void schedule(double t)
  {
    t += 1;
    cout << "Push t = " << t
	 << ", id = " << id
	 << ", this = " << this << endl;
    q.push(make_pair(t, this));
  }
};

int
main()
{
  pqueue q;

  list<Source> ls;

  for(int i = 1; i <= 10; ++i)
    ls.push_back(Source(q, i));

  // The event loop.
  while(!q.empty())
    {
      double t;
      Source *s;
      tie(t, s) = q.top();
      q.pop();
      (*s)(t);
    }
}
