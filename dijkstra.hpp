#include "graph.hpp"

// Band description.  The first elemet tells the starting suncarrier
// number, the second the number of contguous subcarriers used.
typedef pair<int, int> BD;

// Predecessor map.
typedef map<BD, Edge> PM;

// Vertex predecessor map, i.e. a map of vertexes to predecessor maps.
typedef map<Vertex, PM> VPM;

VPM
dijkstra(const Graph &g, Vertex src, Vertex dst, int p);
