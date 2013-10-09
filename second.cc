#include <functional>
#include <iostream>
#include <list>
#include <queue>
#include <utility>
#include <boost/tuple/tuple.hpp>

using namespace std;
using namespace boost;

typedef std::function<void(double)> cback;

class Event
{
  double t;
  cback cb;

public:
  Event(double t, cback cb): t(t), cb(cb)
  {
  }

  void process() const
  {
    cb(t);
  }

  bool operator<(const Event &e) const
  {
    return t > e.t;
  }
};

typedef priority_queue<Event> pqueue;

class Source
{
public:
  pqueue &q;
  int id;

  Source(pqueue &q, int id) : q(q), id(id)
  {
  }

  void operator()(double t)
  {
    schedule(t);
  }

  // Schedule the next event based on the current time 0.
  void schedule(double t)
  {
    t += 1;
    cout << "Push t = " << t
	 << ", id = " << id
	 << ", this = " << this << endl;
    q.push(Event(t, *this));
  }
};

int
main()
{
  pqueue q;

  list<Source> ls;

  for(int i = 1; i <= 2; ++i)
    {
      ls.push_back(Source(q, i));
      ls.back().schedule(0);
    }

  // The event loop.
  while(!q.empty())
    {
      q.top().process();
      q.pop();
    }
}
