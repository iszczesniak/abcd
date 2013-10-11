#include <iostream>
#include <list>
#include <queue>

#include "event.hpp"
#include "source.hpp"

using namespace std;

int
main()
{
  const double sim_limit = 1000000;
  
  // Random number generator
  boost::mt19937 r(1);

  // The DES priority queue.
  pqueue q;

  // The list of nodes.
  list<source> ls;

  for(int i = 1; i <= 2; ++i)
    {
      ls.push_back(source(q, i, r, 2));
      ls.back().schedule(0);
    }

  // The event loop.
  while(!q.empty())
    {
      if (q.top().get_time() > sim_limit)
	break;

      q.top().process();
      q.pop();
    }
}
