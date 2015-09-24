#include "module.hpp"
#include "event.hpp"

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
  q.push(event(t, this));
}
