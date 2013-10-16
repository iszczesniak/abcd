#include "module.hpp"
#include "event.hpp"

module::module(pqueue &q) : q(q)
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
