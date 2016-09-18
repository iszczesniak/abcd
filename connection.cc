#include "connection.hpp"
#include "routing.hpp"
#include "utils.hpp"

#include <algorithm>
#include <iostream>
#include <utility>

#include <boost/none.hpp>

using namespace std;

int connection::counter = 0;

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
  return m_p != boost::none;
}

int
connection::get_len() const
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
connection::get_nol() const
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

bool
connection::establish(const demand &d)
{
  // Make sure the connection is not established.
  assert(!is_established());

  // Route the demand.
  m_p = routing::route(m_g, d);

  // If successful, remember the demand.
  if (m_p)
    m_d = d;

  return is_established();
}

void
connection::tear_down()
{
  assert(is_established());
  routing::tear_down(m_g, m_p.get());
  m_p = boost::none;
}
