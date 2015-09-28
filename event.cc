#include "event.hpp"

#include "module.hpp"

event::event(double t, module *m): t(t), m(m)
{
}

void event::process() const
{
  s->(t);
}

bool event::operator<(const event &e) const
{
  return t > e.t;
}

double event::get_time() const
{
  return t;
}
