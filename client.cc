#include "client.hpp"
#include "dijkstra.hpp"
#include "utils_netgen.hpp"

#include <iostream>

using namespace std;

client::client(graph &g, pqueue &q, int id, boost::mt19937 &rng,
               double l_sleep, double mnc, double l_change):
  g(g), module(q), id(id), rng(rng),
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
      // The client is now idle, and should get busy now.
      bool success = set_up();
      if (success)
        {
          idle = false;
          nc_left = ndg();
          cout << "established";
        }
      else
        cout << "failed to establish";
    }
  else
    {
      // The device is busy now.
      if (nc_left)
        {
          // It's time now to reconfigure.
          bool status = reconfigure();
          --nc_left;
          cout << "reconfigured";
        }
      else
        {
          // It's time now to turn the connection down.
          idle = true;
          tear_down();
          cout << "torn down";
        }
    }

  cout << endl;

  schedule(t);
}

// Schedule the next event based on the current time 0.
void client::schedule(double t)
{
  double dt;

  // The time to sleep.  The time to next event depends on the current
  // state of the client.
  dt = idle ? sdg() : cdg();

  module::schedule(t + dt);
}

bool client::set_up()
{
  // Make sure the connection is not established.
  assert(conn.second.first.empty());

  // The demand end nodes.
  conn.first.first = random_node_pair(g, rng);
  // The number of subcarriers the signal requires.
  conn.first.second = get_random_int(1, 10, rng);

  // We allow to allocate the signal on any of the subcarriers.
  V2C2S r = dijkstra(g, conn.first);
  conn.second = shortest_path(g, r, conn.first);
  bool success = conn.second.first.size();

  if (success)
    set_up_path(g, conn.second);

  return success;
}

bool client::reconfigure()
{
  // Make sure the connection is established.
  assert(!conn.second.first.empty());

  return true;
}

void client::tear_down()
{
  // Make sure the connection is established.
  assert(!conn.second.first.empty());
  tear_down_path(g, conn.second);
  conn.second = cpath();
}
