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
