#include "connection.hpp"
#include "routing.hpp"

#include <algorithm>
#include <iostream>
#include <utility>

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
connection::establish(const demand &d)
{
  std::pair<bool, int> result;

  // Make sure the connection is not established.
  assert(!is_established());

  m_d = d;

  if (m_d.first.first != m_d.first.second)
    {
      sscpath sp = routing::route(g, d);
      result.first = !sp.first.empty();
      result.second = sp.first.size();
      p.first = result.first;
      p.second = sp;
    }
  else
    // We allow to establish a path between the same source and
    // destination nodes.  In this case the path is empty.
    p = make_pair(true, sscpath());

  return set_up(d);
}

void
connection::tear_down()
{
  assert(is_established());
  dijkstra::tear_down_path(g, p.second);
  p = sscpathws();
}
