#ifndef SDI_ARGS_HPP
#define SDI_ARGS_HPP

#include "cdijkstra.hpp"
#include "connection.hpp"
#include "routing.hpp"

#include <string>
#include <boost/optional.hpp>

using namespace std;

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
  
  /// The network type.
  std::string nt;

  /// The number of nodes in the graph.
  int nr_nodes;

  /// The number of edges in the graph.
  boost::optional<int> nr_edges;

  /// The number of slices.
  int nr_sc;

  /// The maximal length of a path we want.
  boost::optional<int> ml;

  /// The maximal length coefficient of a path we want.
  boost::optional<double> mlc;

  /// The K for the k-shortest paths.
  boost::optional<unsigned> K;

  /// The spectrum selection type.
  std::string st;

  /// The routing algorithm.
  std::string rt;

  /// The reconfiguration type.
  std::string re;

  /// -----------------------------------------------------------------
  /// The traffic options
  /// -----------------------------------------------------------------

  /// The mean client arrival time.
  double mcat;

  /// The offered load.
  double ol;

  /// The mean holding time.
  double mht;

  /// The mean number of slices.
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
