#include "module.hpp"
#include "event.hpp"

module::module(pqueue &q) : q(q)
{
}

void
module::schedule(double t)
{
  q.push(event(t, this));
}
