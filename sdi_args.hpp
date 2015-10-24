#ifndef SDI_ARGS_HPP
#define SDI_ARGS_HPP

#include "connection.hpp"
#include "dijkstra.hpp"
#include <string>

using namespace std;

enum network_t {random_network, gabriel_network};

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

  /// The way to reconfigure connections.
  connection::reconf_t reconf;

  /// The way to select subcarriers.
  dijkstra::select_t select;

  /// -----------------------------------------------------------------
  /// The traffic options
  /// -----------------------------------------------------------------

  /// The mean client arrival time.
  double mcat;

  /// The mean holding time.
  double mht;

  /// The mean BS-change time.
  double mbst;

  /// The mean DC-change time.
  double mdct;

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
