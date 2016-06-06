#include "connection.hpp"
#include "routing.hpp"
#include "utils.hpp"

#include <algorithm>
#include <iostream>
#include <utility>

using namespace std;

int connection::counter = 0;

connection::re_t connection::m_re = connection::re_t::none;

connection::connection(graph &g): m_g(g), m_id(counter++)
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
  return m_d;
}

bool
connection::is_established() const
{
  return m_p.first;
}

int
connection::get_length() const
{
  assert(is_established());

  boost::property_map<graph, boost::edge_weight_t>::type
    wm = get(boost::edge_weight_t(), m_g);

  int length = 0;
  const path &p = m_p.second.first;
  
  for(const edge &e: p)
    length += get(wm, e);

  return length;
}

int
connection::get_hops() const
{
  assert(is_established());
  return m_p.second.first.size();
}

int
connection::get_nsc() const
{
  assert(is_established());
  return m_p.second.second.size();
}

// Set the reconfiguration type.
void
connection::set_re(const std::string &re)
{
  map <string, connection::re_t> re_map;
  re_map["complete"] = connection::re_t::complete;
  re_map["our"] = connection::re_t::our;
  m_re = interpret ("reconfiguration type", re, re_map);
}

// Get the reconfiguration type.
connection::re_t
connection::get_re()
{
  return m_re;
}

bool
connection::establish(const demand &d)
{
  // Make sure the connection is not established.
  assert(!is_established());

  // Remember the demand.
  m_d = d;

  // If the source and destination nodes are different, do real
  // routing.
  if (m_d.first.first != m_d.first.second)
    {
      sscpath sp = routing::route(m_g, m_d);
      m_p.first = !sp.first.empty();
      m_p.second = sp;
    }
  else
    // We allow to establish a path between the same source and
    // destination nodes.  In this case the path is empty.
    m_p = make_pair(true, sscpath());

  return m_p.first;
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
  dijkstra::tear_down_path(g, p.second);

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

void
connection::tear_down()
{
  assert(is_established());
  routing::tear_down(m_g, m_p.second);
  m_p = sscpathws();
}
