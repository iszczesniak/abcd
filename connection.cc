#include "connection.hpp"
#include "routing.hpp"

#include <algorithm>
#include <iostream>
#include <utility>

connection::reconf_t connection::reconf;

int
calc_new_links(const sscpath &new_path, const sscpath &old_path)
{
  int number = 0;

  // Now we need to calculate the number of new links to configure,
  // which depends on the SSC.  Check whether the SSC of the new path
  // is the same as the SSC of the old path.
  if (new_path.second == old_path.second)
    {
      // Calculate the number of links to configure, i.e. those links
      // that are in the new path, but are missing in the old path.
      // Iterate over the new path, and calculate those links that are
      // not present in the old path.
      for(path::const_iterator i = new_path.first.begin();
          i != new_path.first.end(); ++i)
        {
          edge e = *i;
          path::const_iterator j = std::find(old_path.first.begin(),
                                             old_path.first.end(), e);
          if (j == old_path.first.end())
            ++number;
        }
    }
  else
    // Since it's a different SSC, we have to configure all links.
    number = new_path.first.size();

  return number;
}

connection::connection(graph &g): g(g)
{
}

connection::~connection()
{
  if (is_established())
    tear_down();
}

const demand &
connection::get_demand() const
{
  return d;
}

bool
connection::is_established() const
{
  return p.first;
}

std::pair<bool, int>
connection::set_up(const demand &d)
{
  std::pair<bool, int> result;

  // Make sure the connection is not established.
  assert(!is_established());

  this->d = d;

  if (d.first.first != d.first.second)
    {
      sscpath sp = routing::set_up(g, d);
      result.first = !p.second.first.empty();
      result.second = p.second.first.size();
      p.first = result.first;
      p.second = sp;
    }
  else
    {
      // We allow to establish a path between the same source and
      // destination nodes.  In this case the path is empty.
      p.first = true;
      p.second = sscpath();
    }
    
  return result;
}

std::pair<bool, int>
connection::reconfigure(vertex new_src)
{
  assert(is_established());

  std::pair<bool, int> result;

  switch(reconf)
    {
    case complete:
      // The complete reconfiguration.
      result = reconfigure_complete(new_src);
      break;

    case incremental:
      // First we do the incremental reconfiguration.
      result = reconfigure_incremental(new_src);
      if (!result.first)
        // And if that fails we do the complete reconfiguration.
        result = reconfigure_complete(new_src);
      break;

    case curtailing:
      // First we do the curtailing reconfiguration.
      result = reconfigure_curtailing(new_src);
      if (!result.first)
        // And if that fails we do the complete reconfiguration.
        result = reconfigure_complete(new_src);
      break;

    default:
      assert(false);
    }

  // Remember the new source when the reconfiguration succeeded.
  if (result.first)
    d.first.first = new_src;

  return result;
}

std::pair<bool, int>
connection::reconfigure_complete(vertex new_src)
{
  std::pair<bool, int> result;

  // Store the existing sscpath, because we'll need it in case we fail
  // to establish a new connection.
  sscpathws tmp = p;

  // First we need to tear down the existing path.  We might need its
  // subcarriers to establish a new connection.
  routing::tear_down_path(g, p.second);

  // That's the new demand.
  vertex dst = d.first.second;
  demand nd(npair(new_src, dst), d.second);

  // When searching for the new path, we allow all available
  // subcarriers.
  V2C2S r = dijkstra::search(g, nd);
  p.second = dijkstra::trace(g, r, nd);
  result.first = !p.second.first.empty();

  if (result.first)
    result.second = calc_new_links(p.second, tmp.second);
  else
    // If no new path has been found, revert to the old path.
    p = tmp;

  dijkstra::set_up_path(g, p.second);

  return result;
}

// We search for the shortest path from the new source node to the
// previous source node.
std::pair<bool, int>
connection::reconfigure_incremental(vertex new_src)
{
  std::pair<bool, int> result;

  // This is the new demand.  Here we state only the number of
  // subcarriers required.
  vertex old_src = d.first.first;
  demand nd(npair(new_src, old_src), d.second);

  // When searching a path for a new demand, we also state exactly
  // what SSC is available at the start, which is the SSC of an
  // existing path.  Together with the number of required subcarriers,
  // we search the path that has exactly the required SSC.
  V2C2S r = dijkstra::search(g, nd, p.second.second);
  // Bridging path.
  sscpath bp = dijkstra::trace(g, r, nd);
  // If the path is not empty, then we succeeded in our search.
  result.first = !bp.first.empty();
  // The number of links to reconfigure.
  result.second = bp.first.size();

  // Set up the extra part and modify the list.
  if (result.first)
    {
      // We want the SSC in the additional path to be the same as in
      // the existing path.
      assert(p.second.second == bp.second);
      p.second.first.insert(p.second.first.begin(),
                            bp.first.begin(), bp.first.end());
      dijkstra::set_up_path(g, bp);
    }

  return result;
}

std::pair<bool, int>
connection::reconfigure_curtailing_worker(vertex new_src)
{
  std::pair<bool, int> result;

  // Store the existing sscpath, because we'll need it in case we fail
  // to establish a new connection.
  sscpathws tmp = p;

  // The new sscpathws -- the path after reconfiguration.
  sscpathws np;

  // The destination node of the connection.
  vertex dst = d.first.second;
  assert(new_src != dst);

  // That's the intermediate node, from where the old path is reused.
  // We start with the old source node.
  vertex int_src = d.first.first;

  // In every iteration of the loop we search for the shortest path
  // from new_src to int_src.  All nodes of the existing connection
  // are considered, including the destination node.  We need to
  // consider the destination node too, because we might be dealing
  // with an empty path, when the client roamed to its destination
  // node.  We retrace the whole path, and as we do it we dismantle
  // the path from the beginning because the dismantled links can be
  // used by the briding connection.
  while(true)
    {
      // The bridging path with status for the int_src node.
      sscpathws bp;

      // It can happen that the client got back to some node
      // previously visited, and so we just cut the path.
      if (new_src == int_src)
        bp.first = true;
      else
        {
          V2C2S r;

          // This is the new bridging demand.  Here we state only the
          // number of subcarriers required.
          demand bd(npair(new_src, int_src), d.second);

          // In bridging, do we need to use the same subcarriers?  We
          // have to care about the continuity constraint only when
          // the path is not empty and the ssc is not empty.
          if (!p.second.first.empty())
            {
              // SSC must not be empty for a non-empty path.
              assert(!p.second.second.empty());
              r = dijkstra::search(g, bd, p.second.second);
            }
          else
            r = dijkstra::search(g, bd);

          // Additional path.
          bp.second = dijkstra::trace(g, r, bd);
          bp.first = !bp.second.first.empty();
        }

      // Is this the best result?  First, we must have found the
      // result, and then it must be the first result ever or it must
      // be better than a previous best result.
      if (bp.first &&
          (!result.first || bp.second.first.size() < result.second))
        {
          result.first = true;
          // That's the shortest length of the briding path.
          result.second = bp.second.first.size();

          // Build the new path np from p and bp.
          np = p;
          // Take the SSC from bp if p is empty.  This can happen in
          // the case we examine the destination node as the end point
          // of the briding connection.
          if (np.second.first.empty())
            np.second.second = bp.second.second;

          // We want the SSC in the additional path to be the same as
          // in the existing path.
          assert(!np.second.second.empty());
          np.second.first.insert(np.second.first.begin(),
                                 bp.second.first.begin(),
                                 bp.second.first.end());

          assert(new_src == source(np.second.first.front(), g));
        }

      // This is the condition for breaking the loop which should go
      // exactly here.  This allows us to consider also the dst node
      // as the end node for the briding path, and prevents the
      // execution of the code below.
      if (p.second.first.empty())
        break;

      // Take down the leading edge in the path.
      edge ettd = p.second.first.front();
      p.second.first.pop_front();
      // Retrace one node.
      assert(source(ettd, g) == int_src);
      int_src = target(ettd, g);
      // Take down that edge!
      sscpath sscpathttd;
      sscpathttd.first.push_back(ettd);
      sscpathttd.second = p.second.second;
      dijkstra::tear_down_path(g, sscpathttd);
    }

  // We should have dismantled the path completely by now.
  assert(p.second.first.empty());

  p = (result.first ? np : tmp);
  dijkstra::set_up_path(g, p.second);

  return result;
}

std::pair<bool, int>
connection::reconfigure_curtailing(vertex new_src)
{
  // If result.first is true, a path has been found. The number of new
  // links to configure is result.second.
  std::pair<bool, int> result;

  // It could happen that the roaming of the client brought it back to
  // the destination node.
  vertex dst = d.first.second;
  if (new_src == dst)
    {
      // Yeah, we found it, and it's nothing: no path and no SSC.
      result.first = true;
      // Tear down the path.
      dijkstra::tear_down_path(g, p.second);
      p.second = sscpath();
    }
  else
    result = reconfigure_curtailing_worker(new_src);

  return result;
}

void
connection::tear_down()
{
  assert(is_established());
  dijkstra::tear_down_path(g, p.second);
  p = sscpathws();
}

connection::reconf_t &
connection::get_reconf()
{
  return reconf;
}
