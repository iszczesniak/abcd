#include <iostream>

#include "cdijkstra.hpp"
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
net_stats(sdi_args &args)
{
  // The accumulator with double values.
  typedef ba::accumulator_set<double, ba::stats<ba::tag::mean,
                                                ba::tag::variance> > dbl_acc;

  // Link lengths.
  dbl_acc lls;

  // Node degrees.
  dbl_acc nds;

  // Shortest path number of hops.
  dbl_acc sphs;

  // Shortest path lengths.
  dbl_acc spls;
  
  for (int n = 2; n <= 10; ++n)
    for (int e = 1; e <= 10; ++e)
      for (int i = 0; i < 1000; ++i)
        {
          cout << "n = " << n << ", e = " << e << ", i = " << i << endl;

          args.nr_nodes = n;
          args.nr_edges = e;
          
          // Random number generator.
          boost::mt19937 rng(i);
          
          // Generate the graph.
          graph g = generate_graph(args, rng);
          
          if (check_components(g))
            {
              auto ns = boost::vertices(g);

              // Calculate stats for shortest paths.
              for (auto ni = ns.first; ni != ns.second; ++ni)
                {
                  vector<int> dist(num_vertices(g));
                  vector<vertex> pred(num_vertices(g));

                  vertex s = *ni;
                  
                  boost::dijkstra_shortest_paths
                    (g, s, boost::predecessor_map(&pred[0]).distance_map(&dist[0]));

                  for (auto nj = ns.first; nj != ns.second; ++nj)
                    if (ni != nj)
                      {
                        vertex d = *nj;

                        // Make sure the path was found.
                        if (pred[d] == d)
                          {
                            cout << "s = " << s << endl;
                            cout << "d = " << d << endl;
                            cout << "pred[d] = " << pred[d] << endl;
                            cout << "in_degree = " << in_degree(d, g) << endl;
                            cout << "out_degree = " << out_degree(d, g) << endl;
                            cout.flush();
                            abort();
                          }
                      }
                }
            }
        }

  cout << "Link length: "
       << "mean = " << ba::mean(lls) << ", "
       << "variance = " << ba::variance(lls) << endl;

  cout << "Node degree: "
       << "mean = " << ba::mean(nds) << ", "
       << "variance = " << ba::variance(nds) << endl;

  cout << "Shortest path length: "
       << "mean = " << ba::mean(spls) << ", "
       << "variance = " << ba::variance(spls) << endl;

  cout << "Shortest path hops: "
       << "mean = " << ba::mean(sphs) << ", "
       << "variance = " << ba::variance(sphs) << endl;
}

void
simulate(const sdi_args &args)
{
  // Set the routing type.
  routing::set_rt(args.rt);

  // Set the maximal length of a path.
  routing::set_ml(args.ml);

  // Set the spectrum selection type.
  routing::set_st(args.st);

  // Random number generator.
  boost::mt19937 rng(args.seed);

  // Generate the graph.
  graph g = generate_graph(args, rng);

  // This simulation object.
  simulation sim(g, rng);
  
  // The traffic module.
  traffic t(args.mcat, args.mht, args.mnsc);

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
