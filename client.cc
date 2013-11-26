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
          if (status)
            {
              --nc_left;
              cout << "reconfigured";
            }
          else
            {
              cout << "connection lost";
              nc_left = 0;
              idle = true;
              tear_down();
            }
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
  conn.first.second = get_random_int(1, 3, rng);

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

  // We change the source node, and the destination node stays
  // unchanged.  We chose the new source node from one of the
  // neighbours of the current source node.
  vertex old_src = conn.first.first.first;

  // From these vertexes we can reach the current source node.
  set<vertex> sov;
  boost::graph_traits<graph>::in_edge_iterator ei, eei;
  for(boost::tie(ei, eei) = boost::in_edges(old_src, g); ei != eei; ++ei)
    sov.insert(source(*ei, g));

  // Now we chose at random from one of the found nodes.
  vertex new_src = get_random_element(sov, rng);

  // We search for the shortest path from new_src to old_src.  And we
  // ask for exactly the very same subcarriers that are already used
  // at by the existing connection.
  
  // This is the new demand.
  demand nd(npair(new_src, old_src), conn.first.second);

  V2C2S r = dijkstra(g, nd, conn.second.second);
  // Additional path.
  sscpath ap = shortest_path(g, r, nd);
  bool status = !ap.first.empty();

  // Set up the extra part and modify the list.
  if (status)
    {
      // We want the SSC in the additional path to be the same as in
      // the existing path.
      assert(conn.second.second == ap.second);
      set_up_path(g, ap);
      conn.second.first.insert(conn.second.first.begin(),
                               ap.first.begin(), ap.first.end());
    }

  return status;
}

void client::tear_down()
{
  // Make sure the connection is established.
  assert(!conn.second.first.empty());
  tear_down_path(g, conn.second);
  conn.second = sscpath();
}
