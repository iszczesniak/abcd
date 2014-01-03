#include "connection.hpp"
#include "dijkstra.hpp"

connection::reconf_t connection::reconf;

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
  bool status;

  // Make sure the connection is established.
  assert(is_established());

  switch(reconf)
    {
    case part:
      status = reconfigure_part(new_src);
      break;

    case anew:
      status = reconfigure_anew(new_src);
      break;

    default:
      assert(false);
    }

  return status;
}

bool
connection::reconfigure_part(vertex new_src)
{
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

bool
connection::reconfigure_anew(vertex new_src)
{
  // Store the existing sscpath, because we'll need it in case we fail
  // to establish a new connection.
  sscpath tmp = p;

  // First we need to tear down the existing path.
  tear_down();

  // That's the new demand.
  vertex dst = d.first.second;
  demand nd(npair(new_src, dst), d.second);

  // When searching for the new path, we allow all available
  // subcarriers.
  V2C2S r = dijkstra(g, nd);
  p = shortest_path(g, r, nd);
  bool status = !p.first.empty();

  // If no new path has been found, revert to the old path.
  if (!status)
    p = tmp;

  set_up_path(g, p);

  return status;
}

void
connection::tear_down()
{
  assert(is_established());
  tear_down_path(g, p);
  p = sscpath();
}

connection::reconf_t &
connection::get_reconf()
{
  return reconf;
}
