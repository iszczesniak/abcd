#include <iostream>
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

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>

namespace ba = boost::accumulators;

using namespace std;

void
net_stats(const sdi_args &args)
{
  // The accumulator with double values.
  typedef ba::accumulator_set<double, ba::stats<ba::tag::mean> > dbl_acc;

  dbl_acc lls;
  
  for (int i = 0; i < 100; ++i)
    {
      // Random number generator.
      boost::mt19937 rng(i);

      // Generate the graph.
      graph g = generate_graph(args, rng);

      assert(check_components(g));

      // Calculate the mean value of the edge lengths.
      auto es = boost::edges(g);
      for (auto ei = es.first; ei != es.second; ++ei)
        lls(boost::get(boost::edge_weight, g, *ei));
    }

  cout << "The mean link length: "
       << ba::mean(lls) << endl;
}

void
simulate(const sdi_args &args)
{
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

int
main(int argc, const char* argv[])
{
  sdi_args args = process_sdi_args(argc, argv);

  if (args.net_stats)
    net_stats(args);
  else
    simulate(args);
    
  return 0;
}
