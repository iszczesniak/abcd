#include "traffic.hpp"

using namespace std;

traffic::traffic(double mcat, double mht, double mbst,
                 double mdct, double mnsc):
  mcat(mcat), catd(mcat), catg(rng, catd),
  mht(mht), mbst(mbst), mdct(mdct), mnsc(mnsc), idc()
{
  schedule(0);
}

traffic::~traffic()
{
  for(auto c: lc)
    delete c;
}

int
traffic::nr_clients() const
{
  return lc.size();
}

void
traffic::operator()(double t)
{
  client *c = new client(mht, mbst, mdct, mnsc);
  lc.push_back(c);
}

void
traffic::schedule_next(double t)
{
  double dt = catg();
  schedule(t + dt);
}
