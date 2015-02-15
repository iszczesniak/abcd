#ifndef SDI_ARGS_HPP
#define SDI_ARGS_HPP

#include "connection.hpp"
#include "dijkstra.hpp"
#include <string>

using namespace std;

enum graph_t {random_graph, gabriel_graph};

/**
 * These are the program arguments.  In this single class we store all
 * information passed at the command line.
 */
struct sdi_args
{
  /// The number of nodes in the graph.
  int nr_nodes;

  /// The number of edges in the graph.
  int nr_edges;

  /// The number of subcarriers.
  int nr_sc;

  /// The number of clients.
  int nr_clients;

  /// The lambda for the sleep time.
  double l_sleep;

  /// The mean number of connection changes.
  double mnc;

  /// The lambda for the change.
  double l_change;

  /// The mean number of the subcarriers
  double mnsc;

  /// The maximal length of a path we want.
  int max_len;

  /// The seed
  int seed;

  /// The hash of all the parameters except the seed parameter.  It's
  /// used to identify the configuration of the simulation.
  string hash;

  /// The way to select subcarriers.
  dijkstra::select_t select;

  /// The way connections should be reconfigured.
  connection::reconf_t reconf;

  /// The graph type;
  graph_t grapht;

  /// The limit on the simulation time.
  double sim_time;
};

/**
 * This function parses the command-line arguments.
 */
sdi_args
process_sdi_args(int argc, const char* argv[]);

#endif /* SDI_ARGS_HPP */
