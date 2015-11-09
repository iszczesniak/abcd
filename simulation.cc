#include "simulation.hpp"

using namespace std;

// The singleton of the class.  The compiler initializes it to null.
simulation *simulation::m_s;

simulation::simulation(graph &g, boost::mt19937 &rng):
  m_g(g), m_rng(rng), m_t()
{
  assert(!m_s);
  m_s = this;
}

simulation
&simulation::get()
{
  assert(m_s);
  return *m_s;
}

graph
&simulation::g() const
{
  return m_g;
}

boost::mt19937
&simulation::rng() const
{
  return m_rng;
}

void
simulation::schedule(double t, module *m)
{
  assert(t >= m_t);
  m_q.push(event(t, m));
}

void
simulation::run(double sim_time)
{
  // The event loop.
  while(!m_q.empty())
    {
      const event &e = m_q.top();

      m_t = e.get_time();

      if (m_t > sim_time)
	break;

      e.process();
      m_q.pop();
    }
}

double
simulation::now()
{
  return m_t;
}
