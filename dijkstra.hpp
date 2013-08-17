#include "graph.hpp"

#include <map>
#include <set>
#include <utility>

// The cost and edge pair.  It is used to store the cost of reaching a
// node along the given edge.
typedef pair<int, Edge> CEP;

// The set of subcarriers.
typedef set<int> SSC;

// The mapping from CEP to SSC.  This mapping tells what SSC is
// available at a node provided a given CEP was used.
typedef map<CEP, SSC> C2S;

// A map of vertexes to C2S.  It's used to store the complete
// information on the
typedef map<Vertex, C2S> V2C2S;

V2C2S
dijkstra(const Graph &g, Vertex src, Vertex dst, int p, const SSC &ssc);
