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
#include <boost/accumulators/statistics/variance.hpp>

namespace ba = boost::accumulators;

using namespace std;

void
net_stats(const sdi_args &args)
{
  // The accumulator with double values.
  typedef ba::accumulator_set<double, ba::stats<ba::tag::mean,
                                                ba::tag::variance> > dbl_acc;

  // Link lengths.
  dbl_acc lls;

  // Node degrees.
  dbl_acc nds;
  
  for (int i = 0; i < 100; ++i)
    {
      // Random number generator.
      boost::mt19937 rng(args.seed + i);

      // Generate the graph.
      graph g = generate_graph(args, rng);

      assert(check_components(g));

      // Calculate the mean value of the edge lengths.
      auto es = boost::edges(g);
      for (auto ei = es.first; ei != es.second; ++ei)
        lls(boost::get(boost::edge_weight, g, *ei));

      // Calculate the mean value of the node degree.
      auto ns = boost::vertices(g);
      for (auto ni = ns.first; ni != ns.second; ++ni)
        nds(boost::out_degree(*ni, g));
    }
  
  cout << "Link length: "
       << "mean = " << ba::mean(lls) << ", "
       << "variance = " << ba::variance(lls) << endl;

  cout << "Node degree: "
       << "mean = " << ba::mean(nds) << ", "
       << "variance = " << ba::variance(nds) << endl;
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
