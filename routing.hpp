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

  static select_t select;

  static int max_len;

  static sscpath
  set_up(const graph &g, const demand &d);

  static routing_t &
  get_routing();

private:
  // Route with the constrained Dijkstra.
  static sscpath
  cdijkstra_f(const graph &g, const demand &d);

  // Route with the edge-disjoint k-shortest paths.
  static sscpath
  ed_ksp_f(const graph &g, const demand &d);  

  static routing_t m_rt;
};

#endif /* ROUTING_HPP */
