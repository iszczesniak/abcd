#include "client.hpp"
#include "dijkstra.hpp"
#include "utils_netgen.hpp"

using namespace std;

client::client(graph &g, pqueue &q, int id, boost::mt19937 &rng,
               double l_sleep, double mnc, double l_change,
               double mnsc):
  g(g), module(q), id(id), rng(rng),
  idle(true), nc_left(0),
  l_sleep(l_sleep), sd(l_sleep), sdg(rng, sd),
  l_change(l_change), cd(l_change), cdg(rng, cd),
  mnc(mnc), ncd(mnc), ncdg(rng, ncd),
  mnsc(mnsc), nscd(mnsc), nscdg(rng, nscd),
  conn(g), st(stats::get())
{
}

client::~client()
{
  if (!idle)
    conn.tear_down();
}

void client::operator()(double t)
{
  if (idle)
    {
      // The client is now idle, and should get busy now.
      bool success = set_up();
      st->established(success);

      if (success)
        {
          idle = false;
          nc_left = ncdg();
        }
    }
  else
    {
      if (nc_left)
        {
          // It's time now to reconfigure.
          bool status = reconfigure();
          if (status)
            --nc_left;
          else
            {
              nc_left = 0;
              idle = true;
              conn.tear_down();
              st->completed(false);
            }
        }
      else
        {
          // It's time now to turn the connection down.
          idle = true;
          conn.tear_down();
          st->completed(true);
        }
    }

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
  // The new demand.
  demand d;
  // The demand end nodes.
  d.first = random_node_pair(g, rng);
  // The number of subcarriers the signal requires.  It's Poisson + 1.
  d.second = nscdg() + 1;

  return conn.set_up(d);
}

bool client::reconfigure()
{
  // We change the source node, and the destination node stays
  // unchanged.  We choose the new source node from one of the
  // neighbours of the current source node.
  vertex old_src = conn.get_demand().first.first;

  // From these vertexes we can reach the current source node.
  set<vertex> sov;
  boost::graph_traits<graph>::in_edge_iterator ei, eei;
  for(boost::tie(ei, eei) = boost::in_edges(old_src, g); ei != eei; ++ei)
    sov.insert(source(*ei, g));

  // Now we chose at random from one of the found nodes.
  vertex new_src = get_random_element(sov, rng);

  return conn.reconfigure(new_src);
}

bool
client::is_idle()
{
  return idle;
}
