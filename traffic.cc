#include "traffic.hpp"

using namespace std;

traffic::traffic(double mcat, double mht, double mnsc):
  mcat(mcat), catd(1 / mcat), catg(rng, catd),
  mht(mht), mnsc(mnsc), idc()
{
  schedule(0);
}

traffic::~traffic()
{
  for(auto c: cs)
    delete c;
}

int
traffic::nr_clients() const
{
  return cs.size();
}

void
traffic::operator()(double t)
{
  client *c = new client(mht, mnsc, *this);
  cs.insert(c);
  schedule_next(t);
}

void
traffic::schedule_next(double t)
{
  double dt = catg();
  schedule(t + dt);
}

void
traffic::erase(client *c)
{
  cs.erase(c);
}
