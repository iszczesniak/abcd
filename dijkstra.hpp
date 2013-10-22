#include "graph.hpp"
#include <utility>

/**
 * Find the shortest path in graph g from source node src to
 * destination node dst for a signal that requires p contiguous
 * subcarriers.  We start the search at node src with the set of
 * subcarriers ssc.
 */
V2C2S
dijkstra(const graph &g, vertex src, vertex dst, int p, const SSC &ssc);

/**
 * Returns the shortest path as a pair of the list of edges and an
 * SSC.
 */
std::pair<std::list<edge>, SSC>
shortest_path(const graph &g, const V2C2S &r, vertex src, vertex dst);

/**
 * Set up the path in the graph.  This process takes away the
 * subcarriers on the edges that are used by the path.
 */
void
set_up_path(graph &g, const path &p, int sc);
