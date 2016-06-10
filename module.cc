#include "module.hpp"

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
