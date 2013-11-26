#include "connection.hpp"
#include "dijkstra.hpp"

connection::connection(graph &g): g(g)
{
}

const demand &
connection::get_demand() const
{
  return d;
}

bool
connection::is_established() const
{
  return !p.first.empty();
}

bool
connection::set_up(const demand &d)
{
  // Make sure the connection is not established.
  assert(!is_established());

  this->d = d;

  // We allow to allocate the signal on any of the subcarriers.
  V2C2S r = dijkstra(g, d);
  p = shortest_path(g, r, d);
  bool success = is_established();

  if (success)
    set_up_path(g, p);

  return success;
}

bool
connection::reconfigure(vertex new_src)
{
  // Make sure the connection is established.
  assert(is_established());

  vertex old_src = d.first.first;

  // We search for the shortest path from new_src to old_src.  And we
  // ask for exactly the very same subcarriers that are already used
  // by the existing connection.
  
  // This is the new demand.  Here we state only the number of
  // subcarriers required.
  demand nd(npair(new_src, old_src), d.second);

  // When searching a path for a new demand, we also state exactly
  // what SSC is available at the start, which is the SSC of an
  // existing path.  Together with the number of required subcarriers,
  // we search the path that has exactly the required SSC.
  V2C2S r = dijkstra(g, nd, p.second);
  // Additional path.
  sscpath ap = shortest_path(g, r, nd);
  bool status = !ap.first.empty();

  // Set up the extra part and modify the list.
  if (status)
    {
      // We want the SSC in the additional path to be the same as in
      // the existing path.
      assert(p.second == ap.second);
      d.first.first = new_src;
      p.first.insert(p.first.begin(),
                     ap.first.begin(), ap.first.end());
      set_up_path(g, ap);
    }

  return status;
}

void
connection::tear_down()
{
  assert(is_established());
  tear_down_path(g, p);
  p = sscpath();
}
