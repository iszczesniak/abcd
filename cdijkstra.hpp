#ifndef CDIJKSTRA_HPP
#define CDIJKSTRA_HPP

#include "graph.hpp"
#include "routing.hpp"

#include <map>

/**
 * Constrained Dijkstra.
 */
class cdijkstra: public routing
{
  typedef std::map<CEP, vertex> pqueue;

  // The following map implements the priority queue.  The key is a
  // CEP, and the value is the vertex we are reaching.  The maps works
  // as the priority queue since the first element in the key is the
  // cost, and since the map sorts its elements in the ascending
  // order.  The value is the vertex.  The value could be null as
  // well, but we want to use CEP as a key, and need to store the
  // vertex as well.
  //
  // We need to know not only the vertex, but the edge too, because we
  // allow for multigraphs (i.e. with parallel edges), and so we need
  // to know what edge was used to reach the vertex.
  // 
  // We could pass only the edge and figure out the vertex from the
  // edge, but there is one special case that prevents us from doing
  // that: the source node, for which the null edge is used.
  // Furthermore, figuring out the end node might be problematic for
  // undirected graphs.
  pqueue q;

protected:
  sscpath
  route_w(graph &g, const demand &d);

  /**
   * Find the shortest path in graph g for demand d.  We start the
   * search at node src.  In this version of the function we try to
   * find a path without any constraints on the ssc.
   */
  V2C2S
  search(const graph &g, const demand &d);

  /**
   * Find the shortest path in graph g for demand d.  We start the
   * search at node src with the set of subcarriers ssc.
   */
  V2C2S
  search(const graph &g, const demand &d, const SSC &ssc);

  /**
   * Returns a candidate shortest path.
   */
  sscpath
  trace(const graph &g, const V2C2S &r, const demand &d);

private:
  /**
   * Check whether there is a better or equal result in c2s than the
   * new result, i.e. of a lower or equal cost and with a SSC that
   * includes "ssc".
   */
  bool
  has_better_or_equal(const C2S &c2s, const COST &cost, const SSC &ssc);

  /**
   * Purge a worse or equal result in c2s, i.e., of a larger or equal
   * cost and with a SSC that is included in "ssc".
   */
  void
  purge_worse(C2S &c2s, const COST &cost, const SSC &ssc);

  void
  relax(C2S &c2s, const CEV &cev, const SSC &ssc);

  
  void
  relax(C2S &c2s, const CEV &cev, const SSSC &sssc);
};

#endif /* CDIJKSTRA_HPP */
