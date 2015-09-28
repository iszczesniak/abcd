#include "module.hpp"

module::module() :
  sim(simulation::get()), g(sim.g()), rng(sim.rng())
{
}

module::~module()
{
}

void
module::schedule(double t)
{
  sim.schedule(t, this);
}
