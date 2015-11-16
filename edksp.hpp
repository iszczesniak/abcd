#ifndef EDKSP_HPP
#define EDKSP_HPP

#include "graph.hpp"
#include "routing.hpp"

/**
 * Constrained Dijkstra.
 */
class edksp: public routing
{

protected:
  sscpath
  route_w(graph &g, const demand &d);
};

#endif /* EDKSP_HPP */
