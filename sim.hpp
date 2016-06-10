#ifndef SIM_HPP
#define SIM_HPP

#include "simulation.hpp"
#include "graph.hpp"
#include "module.hpp"

#include <boost/random.hpp>

typedef simulation<double, graph, boost::mt19937> sim;
typedef module<sim> mod;

#endif /* SIM_HPP */
