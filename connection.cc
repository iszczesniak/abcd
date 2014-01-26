#include "connection.hpp"
#include "dijkstra.hpp"

#include <algorithm>
#include <iostream>
#include <utility>

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

std::pair<bool, int>
connection::set_up(const demand &d)
{
  std::pair<bool, int> result;

  // Make sure the connection is not established.
  assert(!is_established());

  this->d = d;

  // We allow to allocate the signal on any of the subcarriers.
  V2C2S r = dijkstra::search(g, d);
  p = dijkstra::trace(g, r, d);
  result.first = is_established();
  result.second = p.first.size();

  if (result.first)
    dijkstra::set_up_path(g, p);

  return result;
}

std::pair<bool, int>
connection::reconfigure(vertex new_src)
{
  std::pair<bool, int> result;

  switch(reconf)
    {
    case part:
      result = reconfigure_part(new_src);
      break;

    case anew:
      result = reconfigure_anew(new_src);
      break;

    case retrace:
      // First we do the retrace.
      result = reconfigure_retrace(new_src);
      if (!result.first)
        // And if that fails we do reconfiguration anew.
        result = reconfigure_anew(new_src);
      break;

    case retrace2:
      // First we do the retrace2.
      result = reconfigure_retrace2(new_src);
      if (!result.first)
        // And if that fails we do reconfiguration anew.
        result = reconfigure_anew(new_src);
      break;

    default:
      assert(false);
    }

  return result;
}

std::pair<bool, int>
connection::reconfigure_part(vertex new_src)
{
  std::pair<bool, int> result;

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
  V2C2S r = dijkstra::search(g, nd, p.second);
  // Additional path.
  sscpath ap = dijkstra::trace(g, r, nd);
  // If the path is not empty, then we succeeded in our search.
  result.first = !ap.first.empty();
  // The number of links to reconfigure.
  result.second = ap.first.size();

  // Set up the extra part and modify the list.
  if (result.first)
    {
      // We want the SSC in the additional path to be the same as in
      // the existing path.
      assert(p.second == ap.second);
      d.first.first = new_src;
      p.first.insert(p.first.begin(),
                     ap.first.begin(), ap.first.end());
      dijkstra::set_up_path(g, ap);
    }

  return result;
}

std::pair<bool, int>
connection::reconfigure_retrace(vertex new_src)
{
  std::pair<bool, int> result;

  // Store the existing sscpath, because we'll need it in case we fail
  // to establish a new connection.
  sscpath tmp = p;

  // That's the intermediate node, from where the old path is reused.
  vertex int_src = d.first.first;

  // True if a path has been found.
  result.first = false;

  // The additional path.
  sscpath ap;

  // In every iteration of the loop we search for the shortest path
  // from new_src to int_src.  And we ask for exactly the very same
  // subcarriers that are already used by the existing connection.  We
  // break the loop if we reached the destination.
  while(int_src != d.first.second)
    {
      // This is the new demand.  Here we state only the number of
      // subcarriers required.
      demand nd(npair(new_src, int_src), d.second);

      // It can happen that the client got back to some node
      // previously visited, and so we just cut the path.
      if (new_src == int_src)
        {
          result.first = true;
          result.second = 0;
          ap.first = path();
          ap.second = p.second;
        }
      else
        {
          // When searching a path for a new demand, we also state
          // exactly what SSC is available at the start, which is the
          // SSC of an existing path.  Together with the number of
          // required subcarriers, we search the path that has exactly
          // the required SSC.
          V2C2S r = dijkstra::search(g, nd, p.second);
          // Additional path.
          ap = dijkstra::trace(g, r, nd);
          result.first = !ap.first.empty();
          result.second = ap.first.size();
        }

      if (result.first)
        break;

      // If no success, take down the leading edge in the path, and
      // search again.
      edge ettd = p.first.front();
      p.first.pop_front();
      // Retrace one node.
      assert(source(ettd, g) == int_src);
      int_src = target(ettd, g);
      // Take down that edge!
      sscpath sscpathttd;
      sscpathttd.first.push_back(ettd);
      sscpathttd.second = p.second;
      dijkstra::tear_down_path(g, sscpathttd);
    }

  // Tear down the connection or the rest of it, becasue we'll be
  // establishing it again.
  if (is_established())
    dijkstra::tear_down_path(g, p);

  if (result.first)
    {
      // We want the SSC in the additional path to be the same as
      // in the existing path.
      assert(p.second == ap.second);
      d.first.first = new_src;
      p.first.insert(p.first.begin(),
                     ap.first.begin(), ap.first.end());
    }
  else
    p = tmp;

  dijkstra::set_up_path(g, p);

  assert(d.first.first == source(p.first.front(), g));

  return result;
}

std::pair<bool, int>
connection::reconfigure_retrace2(vertex new_src)
{
  // If result.first is true, a path has been found. The number of new
  // links to configure is result.second.
  std::pair<bool, int> result;

  // Store the existing sscpath, because we'll need it in case we fail
  // to establish a new connection.
  sscpath tmp = p;

  // The new sscpath.
  sscpath np;

  // That's the intermediate node, from where the old path is reused.
  vertex int_src = d.first.first;

  // The destination node.
  vertex dst = d.first.second;

  if (new_src == dst)
    {
      result.first = true;
      d.first.first = new_src;
      // Tear down the existing path.
      dijkstra::tear_down_path(g, p);      
      // Clear the path, becasue we don't need it.
      p = sscpath();
    }
  else
  // In every iteration of the loop we search for the shortest path
  // from new_src to int_src.  And we ask for exactly the very same
  // subcarriers that are already used by the existing connection.  We
  // retrace the whole path.
  while(int_src != dst)
    {
      // This is the new demand.  Here we state only the number of
      // subcarriers required.
      demand nd(npair(new_src, int_src), d.second);

      // The additional path.
      sscpath ap;

      std::pair<bool, int> int_result;

      // It can happen that the client got back to some node
      // previously visited, and so we just cut the path.
      if (new_src == int_src)
        int_result.first = true;
      else
        {
          // When searching a path for a new demand, we also state
          // exactly what SSC is available at the start, which is the
          // SSC of an existing path.  Together with the number of
          // required subcarriers, we search the path that has exactly
          // the required SSC.
          V2C2S r;

          // Do we nee to use the same subcarriers?
          if (!p.second.empty())
            r = dijkstra::search(g, nd, p.second);
          else
            r = dijkstra::search(g, nd);

          // Additional path.
          ap = dijkstra::trace(g, r, nd);
          int_result = std::make_pair(!ap.first.empty(), ap.first.size());
        }

      // Is this the best result?  First, we must have found the
      // result, and then it must be the first result ever or it must
      // be better than a previous best result.
      if (int_result.first &&
          (!result.first || int_result.second < result.second))
        {
          result = int_result;
          np = p;
          d.first.first = new_src;
          if (p.second.empty())
            p.second = np.second;

          if (!ap.first.empty())
            {
              // We want the SSC in the additional path to be the same as
              // in the existing path.
              assert(p.second == ap.second);
              np.first.insert(np.first.begin(),
                              ap.first.begin(), ap.first.end());
            }

          assert(d.first.first == source(np.first.front(), g));
        }

      // Take down the leading edge in the path.
      edge ettd = p.first.front();
      p.first.pop_front();
      // Retrace one node.
      assert(source(ettd, g) == int_src);
      int_src = target(ettd, g);
      // Take down that edge!
      sscpath sscpathttd;
      sscpathttd.first.push_back(ettd);
      sscpathttd.second = p.second;
      dijkstra::tear_down_path(g, sscpathttd);
    }

  // Fix it: that's a short cut, debug it.
  if (!p.first.empty())
    tear_down();

  p = (result.first ? np : tmp);

  dijkstra::set_up_path(g, p);

  assert(p.first.empty() ||
         d.first.first == source(p.first.front(), g));

  return result;
}

std::pair<bool, int>
connection::reconfigure_anew(vertex new_src)
{
  std::pair<bool, int> result;

  // Store the existing sscpath, because we'll need it in case we fail
  // to establish a new connection.
  sscpath tmp = p;

  // First we need to tear down the existing path.  We might need its
  // subcarriers to establish a new connection.
  tear_down();

  // That's the new demand.
  vertex dst = d.first.second;
  demand nd(npair(new_src, dst), d.second);

  // When searching for the new path, we allow all available
  // subcarriers.
  V2C2S r = dijkstra::search(g, nd);
  p = dijkstra::trace(g, r, nd);
  result.first = !p.first.empty();

  if (result.first)
    {
      // Make it the new source.
      d.first.first = new_src;

      // The number of links to configure depends on the SSC.
      if (p.second == tmp.second)
        {
          // Calculate the number of links to configure, i.e. those
          // links that are in the new path, but are missing in the
          // old path.  Iterate over the new path, and calculate those
          // edges that are not present in the old path.
          result.second = 0;
          for(path::const_iterator i = p.first.begin();
              i != p.first.end(); ++i)
            {
              edge e = *i;
              path::const_iterator j = std::find(tmp.first.begin(),
                                                 tmp.first.end(), e);
              if (j == tmp.first.end())
                ++result.second;
            }
        }
      else
        // Since it's a different SSC, we have to configure all links.
        result.second = p.first.size();
   }
  else
    // If no new path has been found, revert to the old path.
    p = tmp;

  dijkstra::set_up_path(g, p);

  return result;
}

void
connection::tear_down()
{
  dijkstra::tear_down_path(g, p);
  p = sscpath();
}

connection::reconf_t &
connection::get_reconf()
{
  return reconf;
}
