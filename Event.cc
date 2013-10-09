#include "Event.hpp"
#include "Source.hpp"

Event::Event(double t, Source *s): t(t), s(s)
{
}

void Event::process() const
{
  (*s)(t);
}

bool Event::operator<(const Event &e) const
{
  return t > e.t;
}
