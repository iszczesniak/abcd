#include "routing.hpp"

int dijkstra::m_max_len = INT_MAX;

routing::routing_type &
routing::get()
{
  return routing;
}

sscpath
routing::route(const graph &g, const demand &d)
{
  sscpath result;
  
  assert(d.first.first != d.first.second);

  switch (type)
    {
    case cdijkstra:
      result = cdijkstra(g, d);
      break;
      
    case ed_ksp:
      result = ed_ksp(g, d);
      break;

    default:
      abort();
    }

  return result;
}
  
sscpath
routing::cdijkstra_f(const graph &g, const demand &d)
{
  sscpath result;
  // We allow to allocate the signal on any of the subcarriers.
  V2C2S r = dijkstra::search(g, d);
  result = dijkstra::trace(g, r, d);
  if (!result.first.empty())
    set_up_path(g, p.second);
}

sscpath
routing::ed_ksp_f(const graph &g, const demand &d)
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
routing::tear_down_path(graph &g, const sscpath &p)
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

routing::select_t &
routing::get_select()
{
  return select;
}

int &
routing::get_max_len()
{
  return max_len;
}

SSC
routing::select_ssc(const SSSC &sssc, int nsc)
{
  // This is the selected set.
  SSC ssc;

  switch(select)
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
