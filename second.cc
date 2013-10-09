#include <iostream>
#include <list>
#include <queue>

#include "event.hpp"
#include "source.hpp"

using namespace std;

int
main()
{
  pqueue q;

  list<source> ls;

  for(int i = 1; i <= 2; ++i)
    {
      ls.push_back(source(q, i));
      ls.back().schedule(0);
    }

  // The event loop.
  while(!q.empty())
    {
      q.top().process();
      q.pop();
    }
}
