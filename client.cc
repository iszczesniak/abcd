#include "client.hpp"

#include <utility>

#include "dijkstra.hpp"
#include "simulation.hpp"
#include "stats.hpp"
#include "utils_netgen.hpp"

using namespace std;

client::client(double mht, double mbst, double mdct, double mnsc):
  mht(mht), htd(mht), htg(rng, htd),
  mbst(mbst), bstd(mbst), bstg(rng, bstd), 
  mdct(mdct), dctd(mdct), dctg(rng, dctd),
  mnsc(mnsc - 1), nscd(mnsc), nscdg(rng, nscd),
  conn(g), st(stats::get())
{
}

client::~client()
{
  if (conn.is_established())
    conn.tear_down();
}

void client::operator()(double t)
{
  /*
  if (idle)
    {
      // The client is now idle, and should get busy now.
      pair<bool, int> result = set_up();
      st->established(result.first);

      if (result.first)
        {
          idle = false;
          nc_left = ncdg();
          st->established_length(result.second);
        }
    }
  else
    {
      if (nc_left)
        {
          // It's time now to reconfigure.
          pair<bool, int> result = reconfigure();
          if (result.first)
            {
              --nc_left;
              st->reconfigured_links(result.second);
            }
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
  */
}

// Schedule the next event based on the current time t.
void client::schedule_next(double t)
{
  /*
  double dt;

  // The time to sleep.  The time to next event depends on the current
  // state of the client.
  dt = idle ? sdg() : cdg();

  module::schedule(t + dt);
  */
}

pair<bool, int>
client::set_up()
{
  // The new demand.
  demand d;
  // The demand end nodes.
  d.first = random_node_pair(g, rng);
  // The number of subcarriers the signal requires.  It's Poisson + 1.
  d.second = nscdg() + 1;

  return conn.set_up(d);
}

std::pair<bool, int>
client::reconfigure()
{
  /*
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
  */
}
