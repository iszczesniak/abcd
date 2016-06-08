#include "connection.hpp"
#include "routing.hpp"
#include "utils.hpp"

#include <algorithm>
#include <iostream>
#include <utility>

#include <boost/none.hpp>

using namespace std;

int connection::counter = 0;

connection::re_t connection::m_re = connection::re_t::none;

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
  return m_p;
}

int
connection::get_length() const
{
  assert(is_established());

  boost::property_map<graph, boost::edge_weight_t>::type
    wm = get(boost::edge_weight_t(), m_g);

  int length = 0;
  const path &p = m_p.get().first;
  
  for(const edge &e: p)
    length += get(wm, e);

  return length;
}

int
connection::get_hops() const
{
  assert(is_established());
  return m_p.get().first.size();
}

int
connection::get_nsc() const
{
  assert(is_established());
  return m_p.get().second.size();
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

  // Route the demand.
  m_p = routing::route(m_g, m_d);

  return m_p;
}

std::pair<bool, int>
connection::reconfigure(vertex new_src)
{
  assert(is_established());

  std::pair<bool, int> result;

  switch(m_re)
    {
    case re_t::complete:
      // The complete reconfiguration.
      result = reconfigure_complete(new_src);
      break;

    default:
      assert(false);
    }

  // Remember the new source when the reconfiguration succeeds.
  if (result.first)
    m_d.first.first = new_src;

  return result;
}

std::pair<bool, int>
connection::reconfigure_complete(vertex new_src)
{
  std::pair<bool, int> result;

  // The old path.
  sscpath old_p = m_p.get();

  // First we need to tear down the existing path.  We might need its
  // subcarriers to establish a new connection.
  tear_down();

  // That's the new demand.
  demand nd = demand(npair(new_src, m_d.first.second), m_d.second);

  // Try to establish the connection.
  if (establish(nd))
    result = make_pair(true, calc_new_links(m_p.get(), old_p));

  return result;
}

void
connection::tear_down()
{
  assert(is_established());
  routing::tear_down(m_g, m_p.get());
  m_p = boost::none;
}
