#include "simulation.hpp"

using namespace std;

simulation::simulation(graph &g, boost::mt19937 &rng):
  g(g), rng(rng)
{
  assert(!singleton);
  singleton = this;
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
