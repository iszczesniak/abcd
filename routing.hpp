#ifndef ROUTING_HPP
#define ROUTING_HPP

#include "graph.hpp"

class routing
{
public:
  sscpath
  set_up(const graph &g, const demand &d);

  // The type or routing.
  enum type {cdijkstra, ed_ksp};
      
private:
  // Route with the constrained Dijkstra.
  sscpath
  cdijkstra(const graph &g, const demand &d);

  // Route with the edge-disjoint k-shortest paths.
  sscpath
  ed_ksp(const graph &g, const demand &d);  
}

#endif /* ROUTING_HPP */
