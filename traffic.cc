#include "traffic.hpp"

using namespace std;

traffic::traffic(graph &g, pqueue &q, boost::mt19937 &rng,
                 double mcat, double mht, double mbst,
                 double mdct, double mnsc):
  g(g), module(q), rng(rng), mcat(mcat), mht(mht),
  mbst(mbst), mdct(mdct), mnsc(mnsc), idc(0)
{
  schedule(0);
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
