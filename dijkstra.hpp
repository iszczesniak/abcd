#include "graph.hpp"

/**
 * Find the shortest path in graph g for demand d.  We start the
 * search at node src with the set of subcarriers ssc.
 */
V2C2S
dijkstra(const graph &g, const demand &d, const SSC &ssc);

/**
 * Returns a candidate shortest path.
 */
cpath
shortest_path(const graph &g, const V2C2S &r, const demand &d);

/**
 * Set up the path in the graph.  This process takes away the
 * subcarriers on the edges that are used by the path.
 */
void
set_up_path(graph &g, const cpath &p);
