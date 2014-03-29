#ifndef DIJKSTRA_HPP
#define DIJKSTRA_HPP

#include "graph.hpp"

#include <map>

class dijkstra
{
public:
  // first - the first subcarriers that fit the demand are chosen

  // smallest - the smallest set of contiguous subcarriers are chosen

  enum select_t {first, fittest};
  static select_t select;

private:
  typedef std::map<CEP, vertex> pqueue;

public:
  /**
   * Find the shortest path in graph g for demand d.  We start the
   * search at node src.  In this version of the function we try to
   * find a path without any constraints on the ssc.
   */
  static V2C2S
  search(const graph &g, const demand &d);

  /**
   * Find the shortest path in graph g for demand d.  We start the
   * search at node src with the set of subcarriers ssc.
   */
  static V2C2S
  search(const graph &g, const demand &d, const SSC &ssc);

  /**
   * Returns a candidate shortest path.
   */
  static sscpath
  trace(const graph &g, const V2C2S &r, const demand &d);

  /**
   * Set up the path in the graph.  This process takes away the
   * subcarriers on the edges that are used by the path.
   */
  static void
  set_up_path(graph &g, const sscpath &p);

  /**
   * Tear down the path in the graph.  This process puts back the
   * subcarriers on the edges that are used by the path.
   */
  static void
  tear_down_path(graph &g, const sscpath &p);

  /**
   * Return reference to the select field.
   */
  static select_t &
  get_select();

  // From the given SSSC, select the SSC according to the "select"
  // field of the class.  The returned SSC has exactly nsc
  // subcarriers, even though the chosen contiguous fragment from SSSC
  // had more subcarriers.
  static SSC
  select_ssc(const SSSC &sssc, int nsc);

private:
  /**
   * Check whether there is a better or equal result in c2s than the
   * new result, i.e. of a lower or equal cost and with a SSC that
   * includes "ssc".
   */
  static bool
  has_better_or_equal(const C2S &c2s, const COST &cost, const SSC &ssc);

  /**
   * Check whether there is a worse or equal result in c2s, i.e. of a
   * larger or equal cost and with a SSC that is included in "ssc".
   */
  static void
  purge_worse(pqueue &q, C2S &c2s, const COST &cost, const SSC &ssc);

  static void
  relaks(pqueue &q, C2S &c2s, const CEP &cep, vertex v,
         const SSC &ssc);

  static void
  relaks(pqueue &q, C2S &c2s, const CEP &cep, vertex v,
         const SSSC &sssc);

  // Select a contiguous ssc with the lowest subcarrier numbers from
  // SSSC.  It returns the whole available fragment, i.e. it can have
  // more subcarriers than nsc.
  static SSC
  select_first(const SSSC &sssc, int nsc);

  // Select a contiguous ssc with the lowest subcarrier numbers from
  // SSC.  It returns the whole available fragment, i.e. it can have
  // more subcarriers than nsc.
  static SSC
  select_first(const SSC &ssc, int nsc);

  // Select the fittest subcarriers, i.e. the smallest subcarrier
  // fragment that fits the required subcarriers from SSSC.  It
  // returns the whole available fragment, i.e. it can have more
  // subcarriers than nsc.
  static SSC
  select_fittest(const SSSC &sssc, int nsc);

  // Select the fittest subcarriers, i.e. the smallest subcarrier
  // fragment from SSC that fits the required number of subcarriers.
  // It returns the whole available fragment, i.e. it can have more
  // subcarriers than nsc.
  static SSC
  select_fittest(const SSC &ssc, int nsc);
};

#endif /* DIJKSTRA_HPP */
