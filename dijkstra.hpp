#include "graph.hpp"

#include <map>
#include <set>
#include <utility>

// The set of subcarriers.
typedef set<int> SSC;

// The predecessor info.
typedef pair<Edge, SSC> PI;

// The cost associated with the predecessor info.
typedef pair<int, PI> CPI;

// The set of the CPIs.
typedef set<CPI> SCPI;

// Vertex predecessor map, i.e. a map of vertexes to predecessor maps.
typedef map<Vertex, SCPI> MSCPI;

MSCPI
dijkstra(const Graph &g, Vertex src, Vertex dst, int p);
