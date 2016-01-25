#include "routing.hpp"

#include "cdijkstra.hpp"
#include "edksp.hpp"
#include "utils.hpp"
#include "yenksp.hpp"

#include <climits>
#include <iostream>
#include <list>

using namespace std;

routing::rt_t routing::m_rt = routing::rt_t::none;

routing::st_t routing::m_st = routing::st_t::none;

boost::optional<int> routing::m_ml;

boost::optional<unsigned> routing::m_K;

unique_ptr<routing> routing::singleton;

sscpath
routing::route(graph &g, const demand &d)
{
  assert(singleton);
  return singleton->route_w(g, d);
}

routing::st_t
routing::st_interpret (const string &st)
{
  map <string, routing::st_t> st_map;
  st_map["first"] = routing::st_t::first;
  st_map["fittest"] = routing::st_t::fittest;
  return interpret ("spectrum selection type", st, st_map);
}

routing::rt_t
routing::rt_interpret (const string &rt)
{
  map <string, routing::rt_t> rt_map;
  rt_map["cdijkstra"] = rt_t::cdijkstra;
  rt_map["edksp"] = rt_t::edksp;
  rt_map["yenksp"] = rt_t::yenksp;
  return interpret ("routing type", rt, rt_map);
}

void
routing::set_rt(const string &rt)
{
  m_rt = rt_interpret(rt);

  switch (m_rt)
    {
    case rt_t::cdijkstra:
      singleton.reset(new cdijkstra());
      break;

    case rt_t::edksp:
      singleton.reset(new edksp());
      break;

    case rt_t::yenksp:
      singleton.reset(new yenksp());
      break;

    default:
      abort();
    }
}

routing::rt_t
routing::get_rt()
{
  return m_rt;
}

void
routing::set_ml(boost::optional<int> ml)
{
  m_ml = ml;
}

boost::optional<int>
routing::get_ml()
{
  return m_ml;
}

void
routing::set_K(boost::optional<unsigned> K)
{
  m_K = K;
}

boost::optional<unsigned>
routing::get_K()
{
  return m_K;
}

void
routing::set_st(const string &st)
{
  m_st = st_interpret(st);
}

routing::st_t
routing::get_st()
{
  return m_st;
}

bool
routing::set_up_path(graph &g, const sscpath &p)
{
  // The SSC map.
  boost::property_map<graph, boost::edge_ssc_t>::type
    sm = get(boost::edge_ssc_t(), g);
  
  const path &l = p.first;
  const SSC &p_ssc = p.second;

  // The list of edges on which the p_ssc was allocated.
  list<edge> le;
  
  // Make sure the edges have the required SSC.
  for(const auto &e: l)
    if (includes(sm[e], p_ssc))
      {
        exclude(sm[e], p_ssc);
        le.push_back(e);
      }
    else
      {
        // We failed to allocate the SSC along the whole path, and so
        // we free up those SSCs already allocated.
        for(const auto &e: le)
          {
            // The SSC for edge e.
            SSC &e_ssc = sm[e];

            // Make sure that the edge has these slices taken.
            assert(excludes(e_ssc, p_ssc));

            // Put back the p_ssc slices to e_ssc.
            include(e_ssc, p_ssc);
          }

        return false;
      }

  return true;
}

void
routing::tear_down(graph &g, const sscpath &p)
{
  boost::property_map<graph, boost::edge_ssc_t>::type
    sscm = get(boost::edge_ssc_t(), g);

  const path &l = p.first;
  const SSC &p_ssc = p.second;

  // Iterate over the edges of the path.
  for(path::const_iterator i = l.begin(); i != l.end(); ++i)
    {
      const edge &e = *i;

      // The SSC for edge e.
      SSC &e_ssc = sscm[e];

      // Make sure that the edge has these slices taken.
      assert(excludes(e_ssc, p_ssc));

      // Put back the p_ssc slices to e_ssc.
      include(e_ssc, p_ssc);
    }
}

SSC
routing::select_first(const SSSC &sssc, int nsc)
{
  SSC ssc;

  for(SSSC::const_iterator i = sssc.begin(); i != sssc.end(); ++i)
    {
      SSC tmp_ssc = select_first(*i, nsc);

      if (ssc.empty())
        ssc = tmp_ssc;
      else
        if (*tmp_ssc.begin() < *ssc.begin())
          ssc = tmp_ssc;
    }

  return ssc;
}

SSC
routing::select_first(const SSC &ssc, int nsc)
{
  SSSC sssc = split(ssc);

  for(SSSC::const_iterator i = sssc.begin(); i != sssc.end(); ++i)
    {
      const SSC &tmp = *i;

      // We take the first SSC that can handle nsc.
      if (tmp.size() >= nsc)
        return tmp;
    }

  return SSC();
}

SSC
routing::select_fittest(const SSSC &sssc, int nsc)
{
  SSC ssc;

  for(SSSC::const_iterator i = sssc.begin(); i != sssc.end(); ++i)
    {
      SSC tmp_ssc = select_fittest(*i, nsc);

      if (ssc.empty())
        ssc = tmp_ssc;
      else
        if (tmp_ssc.size() < ssc.size())
          ssc = tmp_ssc;
    }

  return ssc;
}

SSC
routing::select_fittest(const SSC &ssc, int nsc)
{
  const SSC *result = NULL;

  SSSC sssc = split(ssc);

  for(SSSC::const_iterator i = sssc.begin(); i != sssc.end(); ++i)
    {
      const SSC &tmp = *i;

      // We only care about those fragments that can handle nsc.
      if (tmp.size() >= nsc)
        {
          // Take that, because it's the first we got.
          if (result == NULL)
            result = &tmp;
          else
            // Take tmp, only if it's tighter than the previous find.
            if (tmp.size() < result->size())
              result = &tmp;
        }
    }

  if (!result)
    return SSC();
  else
    return *result;
}
