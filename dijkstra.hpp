#include "graph.hpp"

// Band description.  The first elemet tells the starting suncarrier
// number, the second the number of contguous subcarriers used.
typedef pair<int, int> BD;

// 
typedef map<BD, Edge> VPM;

// This is the result of the Dijkstra search.
typedef map<Vertex, VPM> DSPR;

DSPR
dijkstra(const Graph &g, Vertex src, Vertex dst, int p);
