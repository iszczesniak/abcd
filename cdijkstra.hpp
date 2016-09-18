#ifndef CDIJKSTRA_HPP
#define CDIJKSTRA_HPP

#include "graph.hpp"
#include "routing.hpp"

#include <set>

/**
 * Constrained Dijkstra.
 */
class cdijkstra: public routing
{
  // We implement the priority queue with the set, since it's a sorted
  // container.  We ain't using the std::priority_queue, becasue it
  // doesn't support the element removal, which we need.
  //
  // The queue element is of type CEV, i.e., a tuple of COST, edge and
  // vertex.  We need to know not only the target vertex, but the edge
  // too, because we allow for multigraphs (i.e. with parallel edges),
  // and so we need to know what edge was used to reach the target
  // vertex.
  // 
  // We cannot store only the edge and figure out the target vertex
  // from that edge for two reasons. 1) There is one special case for
  // which we cannot figure out the target vertex from the edge: the
  // source node, for which the null edge is used.  2) Having only a
  // pair of COST and edge is wrong for undirected edges, because a
  // pair of (cost, edge) and (cost, redge) is the same, where redge
  // is the edge reverse to edge.  To make an element unique, we need
  // a tuple of (cost, edge, target vertex).
  typedef std::set<CEV> pqueue;

protected:
  sscpath
  route_w(graph &g, const demand &d, const SSC &ssc);

  /**
   * Find the shortest path in graph g for demand d.  We start the
   * search at node src with the set of slices ssc.
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
   * Check whether in c2s there is a better or equal result than the
   * new result (given as arguments cost and ssc), i.e. of a lower or
   * equal cost than "cost" and with a SSC that includes "ssc".
   */
  bool
  has_better_or_equal(const C2S &c2s, const COST &cost, const SSC &ssc);

  /**
   * Purge from c2s those results that are worse or equal to the new
   * result (given as arguments cost and ssc), i.e., of a higher or
   * equal cost and with an SSC that is included in "ssc".
   */
  void
  purge_worse_or_equal(pqueue &p, C2S &c2s, const COST &cost, const SSC &ssc);

  void
  relax(pqueue &q, C2S &c2s, const CEV &cev, const SSC &ssc);
};

#endif /* CDIJKSTRA_HPP */
