#include "module.hpp"

#include "event.hpp"
#include "simulation.hpp"

module::module() :
  sim(simulation::get())
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
