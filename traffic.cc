#include "traffic.hpp"

using namespace std;

traffic::traffic(double mcat, double mht, double mbst,
                 double mdct, double mnsc):
  mcat(mcat), mht(mht), mbst(mbst), mdct(mdct), mnsc(mnsc), idc(0)
{
  schedule(0);
}

int
traffic::nr_clients() const
{
  return lc.size();
}

void
traffic::operator()(double t)
{
  //  client *c = new client(g, q, i, gen,
  //                         args.l_sleep, args.mnc, args.l_change,
  //                       args.mnsc);
  //c->schedule(0);
  //vc.push_back(c);
}
