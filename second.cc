#include <iostream>
#include <list>
#include <queue>
#include <utility>
#include <boost/tuple/tuple.hpp>

#include "Event.hpp"
#include "Source.hpp"

using namespace std;
using namespace boost;

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
