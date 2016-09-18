#ifndef EDKSP_HPP
#define EDKSP_HPP

#include "graph.hpp"
#include "routing.hpp"

/**
 * Routing with the edge-disjoint k-shortest paths.
 */
class edksp: public routing
{

protected:
  sscpath
  route_w(graph &g, const demand &d, const SSC &ssc);
};

#endif /* EDKSP_HPP */
