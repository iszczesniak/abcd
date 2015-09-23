#include <list>
#include <queue>

#include "client.hpp"
#include "event.hpp"
#include "graph.hpp"
#include "sdi_args.hpp"
#include "stats.hpp"
#include "utils_netgen.hpp"

#include <boost/random.hpp>

using namespace std;

int
main(int argc, const char* argv[])
{
  sdi_args args = process_sdi_args(argc, argv);

  // Simulation time.
  double sim_time = args.sim_time;

  // Random number generator.
  boost::mt19937 gen(args.seed);

  // Generate the graph.
  graph g = generate_graph(args, gen);

  // Set how the connections should be reconfigured.
  connection::get_reconf() = args.reconf;

  // Set how the ssc should be selected.
  dijkstra::get_select() = args.select;

  // Set the maximal length of a connection.
  dijkstra::get_max_len() = args.max_len;

  // The DES priority queue.
  pqueue q;

  // Create the traffic module for the simulation.
  traffic t(g, q, gen, args.mcat, args.mht,
            args.mbst, args.mdct, args.mnsc);

  // The stats module.
  stats s(args, g, q, t);

  // The event loop.
  while(!q.empty())
    {
      if (q.top().get_time() > sim_time)
	break;

      q.top().process();
      q.pop();
    }
}
