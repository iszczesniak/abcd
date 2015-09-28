#include "simulation.hpp"

using namespace std;

// The singleton of the class.  The compiler initializes it to null.
simulation *simulation::_s;

simulation::simulation(graph &g, boost::mt19937 &rng):
  _g(g), _rng(rng), _t()
{
  assert(!_s);
  _s = this;
}

simulation
&simulation::get()
{
  assert(_s);
  return *_s;
}

graph
&simulation::g() const
{
  return _g;
}

boost::mt19937
&simulation::rng() const
{
  return _rng;
}

void
simulation::schedule(double t, module *m)
{
  assert(t >= _t);
  _q.push(event(t, m));
}

void
simulation::run(double sim_time)
{
  // The event loop.
  while(!_q.empty())
    {
      const event &e = _q.top();

      _t = e.get_time();

      if (_t > sim_time)
	break;

      e.process();
      _q.pop();
    }
}
