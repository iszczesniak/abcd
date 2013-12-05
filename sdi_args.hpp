#ifndef SDI_ARGS_HPP
#define SDI_ARGS_HPP

#include "connection.hpp"
#include <string>

using namespace std;

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

  /// The seed
  int seed;
};

/**
 * This function parses the command-line arguments.
 */
sdi_args
process_sdi_args(int argc, char* argv[]);

#endif /* SDI_ARGS_HPP */
