#include "module.hpp"

module::module() :
  sim(simulation::get()), g(sim.g()), rng(sim.rng())
{
}

module::~module()
{
}

double
module::now()
{
  return sim.now();
}

void
module::schedule()
{
  sim.schedule(now(), this);
}

void
module::schedule(double t)
{
  sim.schedule(t, this);
}
