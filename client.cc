#include "client.hpp"
#include <iostream>

using namespace std;

client::client(pqueue &q, int id, boost::mt19937 &rng,
               double l_sleep, double mnc, double l_change):
  module(q), id(id), rng(rng),
  idle(true), nc_left(0),
  l_sleep(l_sleep), sd(l_sleep), sdg(rng, sd),
  l_change(l_change), cd(l_change), cdg(rng, cd),
  mnc(mnc), nd(mnc), ndg(rng, nd)
{
}

client::~client()
{
}

void client::operator()(double t)
{
  cout << "Event t = " << t << ", id = " << id << ": ";

  if (idle)
    {
      // Now the client gets busy.
      idle = false;
      nc_left = ndg();
      set_up();
    }
  else
    if (nc_left)
      {
        --nc_left;
        reconfigure();
      }
    else
      {
        // Now the client gets idle.
        idle = true;
        tear_down();
      }

  cout << endl;

  schedule(t);
}

// Schedule the next event based on the current time 0.
void client::schedule(double t)
{
  double dt;

  // The time to sleep.
  dt = idle ? sdg() : cdg();

  module::schedule(t + dt);
}

void client::set_up()
{
  // Establish a new connection.
  cout << "establishing";
}

void client::reconfigure()
{
  // Reconfigure the connection.
  cout << "reconfiguring";
}

void client::tear_down()
{
  // Take the connection down.
  cout << "tear down";
}
