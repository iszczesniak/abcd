#include "routing.hpp"

#include "cdijkstra.hpp"
#include "utils.hpp"

#include <iostream>

using namespace std;

routing::select_t routing::m_st;

int routing::m_ml;

std::unique_ptr<routing> routing::singleton;

sscpath
routing::route(graph &g, const demand &d)
{
  assert(singleton);
  return singleton->route_w(g, d);
}

void
routing::set_rt(const std::string &rt)
{
  if (rt == "cdijkstra")
    singleton.reset(new cdijkstra());
  else
    {
      std::cerr << "routing types: cdijkstra" << std::endl;
      exit(1);
    }
}

void
routing::set_ml(int ml)
{
  m_ml = ml;
}

void
routing::set_st(const std::string &st)
{
}

void
routing::set_up_path(graph &g, const sscpath &p)
{
  boost::property_map<graph, boost::edge_ssc_t>::type
    sscm = get(boost::edge_ssc_t(), g);
  
  const path &l = p.first;
  const SSC &p_ssc = p.second;

  // Iterate over the edges of the path.
  for(path::const_iterator i = l.begin(); i != l.end(); ++i)
    {
      const edge e = *i;

      // The SSC for edge e.
      SSC &e_ssc = sscm[e];

      // Make sure that the edge has the required subcarriers.
      assert(includes(e_ssc, p_ssc));

      // Remove the p_ssc subcarriers from e_ssc.
      exclude(e_ssc, p_ssc);
    }
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

      // Make sure that the edge has these subcarriers taken.
      assert(excludes(e_ssc, p_ssc));

      // Put back the p_ssc subcarriers to e_ssc.
      include(e_ssc, p_ssc);
    }
}

SSC
routing::select_ssc(const SSSC &sssc, int nsc)
{
  // This is the selected set.
  SSC ssc;

  switch(m_st)
      {
      case first:
        ssc = select_first(sssc, nsc);
        break;

      case fittest:
        ssc = select_fittest(sssc, nsc);
        break;

      default:
        assert(false);
      }

  // Now in ssc we have got a fragment that has at least nsc
  // subcarriers, but most likely it has more.  We need to select
  // exactly nsc subcarriers.
  SSC::const_iterator fin = ssc.begin();
  advance(fin, nsc);
  ssc.erase(fin, ssc.end());

  return ssc;
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
  SSC result;

  SSSC sssc = split(ssc);

  for(SSSC::const_iterator i = sssc.begin(); i != sssc.end(); ++i)
    {
      const SSC &tmp = *i;

      // We only care about those fragments that can handle nsc.
      if (tmp.size() >= nsc)
        {
          result = tmp;
          break;
        }
    }

  return result;
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
  SSC result;

  SSSC sssc = split(ssc);

  for(SSSC::const_iterator i = sssc.begin(); i != sssc.end(); ++i)
    {
      const SSC &tmp = *i;

      // We only care about those fragments that can handle nsc.
      if (tmp.size() >= nsc)
        {
          if (result.empty())
            result = tmp;
          else
            if (tmp.size() < result.size())
              result = tmp;
        }
    }

  return result;
}
