#include "simulation.hpp"

using namespace std;

simulation::simulation(graph &g, boost::mt19937 &rng):
  g(g), rng(rng), t()
{
  assert(!singleton);
  singleton = this;
}

void
simulation::schedule(const event &e)
{
  assert(e.get_time() >= t);
  q.push(e);
}

void
simulation::run(double sim_time)
{
  // The event loop.
  while(!q.empty())
    {
      if (q.top().get_time() > sim_time)
	break;

      q.top().process();
      q.pop();
    }
}
