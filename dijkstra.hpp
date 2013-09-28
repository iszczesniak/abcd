#include "graph.hpp"

/**
 * Find the shortest path in graph g from source node src to
 * destination node dst for a signal that requires p contiguous
 * subcarriers.  We start the search at node src with the set of
 * subcarriers ssc.
 */
V2C2S
dijkstra(const Graph &g, Vertex src, Vertex dst, int p, const SSC &ssc);

/**
 * Returns the shortest path as a pair of the list of Edges and an
 * SSC.
 */
pair<list<Edge>, SSC>
shortest_path(const Graph &g, const V2C2S &r, Vertex src, Vertex dst);

/**
 * Set up the path in the graph.  This process takes away the
 * subcarriers on the edges that are used by the path.
 */
void
set_up_path(Graph &g, const Path &p, int sc);
