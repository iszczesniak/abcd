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

  delete_clients();
}

int
traffic::nr_clients() const
{
  return cs.size();
}

void
traffic::operator()(double t)
{
  // Before we create new clients, we delete those clients that
  // requested deletion.
  delete_clients();
  
  // We are creating a client, but we ain't doing anything with the
  // pointer we get!  It's so, because it's up to the client to
  // register itself with the traffic.
  new client(mht, mnsc, *this);
  schedule_next(t);
}

void
traffic::schedule_next(double t)
{
  double dt = catg();
  schedule(t + dt);
}

void
traffic::insert(client *c)
{
  cs.insert(c);
}

void
traffic::erase(client *c)
{
  cs.erase(c);
}

void
traffic::delete_me_later(client *c)
{
  dl.push(c);
}

void
traffic::delete_clients()
{
  while(!dl.empty())
    {
      delete dl.front();
      dl.pop();
    }
}
