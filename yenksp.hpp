#ifndef YENKSP_HPP
#define YENKSP_HPP

#include "graph.hpp"
#include "routing.hpp"

/**
 * Routing with the Yen k-shortest paths.
 */
class yenksp: public routing
{

protected:
  sscpath
  route_w(graph &g, const demand &d, const SSC &ssc);
};

#endif /* YENKSP_HPP */
