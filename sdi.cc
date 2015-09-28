#include <list>
#include <queue>

#include "client.hpp"
#include "event.hpp"
#include "graph.hpp"
#include "sdi_args.hpp"
#include "simulation.hpp"
#include "stats.hpp"
#include "utils_netgen.hpp"

#include <boost/random.hpp>

using namespace std;

int
main(int argc, const char* argv[])
{
  sdi_args args = process_sdi_args(argc, argv);

  // Set how the connections should be reconfigured.
  connection::get_reconf() = args.reconf;

  // Set how the ssc should be selected.
  dijkstra::get_select() = args.select;

  // Set the maximal length of a connection.
  dijkstra::get_max_len() = args.max_len;

  // Random number generator.
  boost::mt19937 rng(args.seed);

  // Generate the graph.
  graph g = generate_graph(args, rng);

  // This simulation object.
  simulation sim(g, rng);
  
  // The traffic module.
  traffic t(args.mcat, args.mht, args.mbst, args.mdct,
            args.mnsc);

  // The stats module.
  stats s(args, t);

  // Run the simulation.
  sim.run(args.sim_time);
}
