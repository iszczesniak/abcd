#include "module.hpp"

module::module() :
  m_sim(simulation::get()), g(m_sim.g()), rng(m_sim.rng())
{
}

module::~module()
{
}

double
module::now()
{
  return m_sim.now();
}

void
module::schedule()
{
  m_sim.schedule(now(), this);
}

void
module::schedule(double t)
{
  m_sim.schedule(t, this);
}
