#ifndef SAMPLE_GRAPHS_HPP
#define SAMPLE_GRAPHS_HPP

#include "graph.hpp"

#include <tuple>
#include <vector>

// Returns this graph:
//
// 0 --- (1, {0, 1}) --- 1 --- (2, {1, 2}) --- 2
//
// Edge label: (w, SSC)
// w - the weight of the edge
// SSC - the set of slices on the edge

std::tuple<graph, std::vector<vertex>, std::vector<edge> >
sample_graph1();

#endif /* SAMPLE_GRAPHS */
