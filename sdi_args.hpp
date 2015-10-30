#ifndef SDI_ARGS_HPP
#define SDI_ARGS_HPP

#include "cdijkstra.hpp"
#include "connection.hpp"
#include "routing.hpp"
#include <string>

using namespace std;

// The network type.
enum network_t {random_network, gabriel_network};

// The type of routing.
enum routing_t {cdijkstra, ed_ksp};

/**
 * These are the program arguments.  In this single class we store all
 * information passed at the command line.
 */
struct sdi_args
{
  /// -----------------------------------------------------------------
  /// The general options
  /// -----------------------------------------------------------------

  // Show graph stats.
  bool net_stats = false;

  /// -----------------------------------------------------------------
  /// The network options
  /// -----------------------------------------------------------------
  
  /// The graph type;
  network_t network;

  /// The number of nodes in the graph.
  int nr_nodes;

  /// The number of edges in the graph.
  int nr_edges;

  /// The number of subcarriers.
  int nr_sc;

  /// The maximal length of a path we want.
  int max_len;

  /// The way to select subcarriers.
  routing::select_t select;

  /// The routing algorithm.
  routing::routing_t rt;
  
  /// -----------------------------------------------------------------
  /// The traffic options
  /// -----------------------------------------------------------------

  /// The mean client arrival time.
  double mcat;

  /// The mean holding time.
  double mht;

  /// The mean number of subcarriers.
  double mnsc;

  /// -----------------------------------------------------------------
  /// The simulation options
  /// -----------------------------------------------------------------

  /// The seed
  int seed;

  /// The hash of all the parameters except the seed parameter.  It's
  /// used to identify the configuration of the simulation.
  string hash;

  /// The limit on the simulation time.
  double sim_time;
};

/**
 * This function parses the command-line arguments.
 */
sdi_args
process_sdi_args(int argc, const char* argv[]);

#endif /* SDI_ARGS_HPP */
