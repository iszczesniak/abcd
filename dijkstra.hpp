#ifndef DIJKSTRA_HPP
#define DIJKSTRA_HPP

#include "graph.hpp"

#include <map>

class dijkstra
{
public:
  // first - the first subcarriers that fit the demand are chosen

  // smallest - the smallest set of contiguous subcarriers are chosen

  enum select_t {first, smallest};
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
};

#endif /* DIJKSTRA_HPP */
