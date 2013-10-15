#include "event.hpp"
#include "module.hpp"

event::event(double t, module *s): t(t), s(s)
{
}

void event::process() const
{
  (*s)(t);
}

bool event::operator<(const event &e) const
{
  return t > e.t;
}

double event::get_time() const
{
  return t;
}
