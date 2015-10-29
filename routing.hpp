#ifndef ROUTING_HPP
#define ROUTING_HPP

#include "graph.hpp"

class routing
{
public:
  // The type of routing.
  enum routing_t {cdijkstra, ed_ksp};

  // The type of specturm selection:
  // first - the first subcarriers that fit the demand are chosen
  // smallest - the smallest set of contiguous subcarriers are chosen
  enum select_t {first, fittest};

  static sscpath
  set_up(const graph &g, const demand &d);

  /**
   * Return reference to the routing type field.
   */
  static routing_t &
  get_rt();

  /**
   * Return reference to the select type field.
   */
  static select_t &
  get_select();

  /**
   * Returns reference to the max_len field.
   */
  static int &
  get_max_len();

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
   * From the given SSSC, select the SSC according to the "select"
   * field of the class.  The returned SSC has exactly nsc
   * subcarriers, even though the chosen contiguous fragment from SSSC
   * had more subcarriers.
   */
  static SSC
  select_ssc(const SSSC &sssc, int nsc);

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

private:
  // Route with the constrained Dijkstra.
  static sscpath
  cdijkstra_f(const graph &g, const demand &d);

  // Route with the edge-disjoint k-shortest paths.
  static sscpath
  ed_ksp_f(const graph &g, const demand &d);  

  static routing_t m_rt;

  static select_t m_select;

  static int m_max_len;
};

#endif /* ROUTING_HPP */
