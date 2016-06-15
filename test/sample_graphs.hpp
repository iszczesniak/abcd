#ifndef SAMPLE_GRAPHS_HPP
#define SAMPLE_GRAPHS_HPP

#include "graph.hpp"

// Returns this graph:
//
// 0 --- (1, {0, 1}) --- 1 --- (2, {1, 2}) --- 2
//
// Edge label: (w, SSC)
// w - the weight of the edge
// SSC - the set of slices on the edge

graph
sample_graph1();

#endif /* SAMPLE_GRAPHS */
