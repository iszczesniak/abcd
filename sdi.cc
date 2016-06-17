#include "graph.hpp"
#include "sdi_args.hpp"
#include "sim.hpp"
#include "stats.hpp"
#include "utils_netgen.hpp"

#include <boost/random.hpp>

#include <iostream>
#include <string>

using namespace std;

template<typename T>
void
print_stats(const string &s, const T &t)
{
  cout << s << ": "
       << "min = " << ba::min(t) << ", "
       << "mean = " << ba::mean(t) << ", "
       << "max = " << ba::max(t) << ", "
       << "variance = " << ba::variance(t) << endl;
}

void
net_stats(const sdi_args &args_orig)
{
  // Number of nodes.
  dbl_acc nns;

  // Number of links.
  dbl_acc nls;

  // Link lengths.
  dbl_acc lls;

  // Node degrees.
  dbl_acc nds;

  // Shortest path number of hops.
  dbl_acc sphs;

  // Shortest path lengths.
  dbl_acc spls;
  
  for (int i = 0; i < 100; ++i)
    {
      sdi_args args = args_orig;
      
      args.seed += i;
      
      // Random number generator.
      boost::mt19937 rng(args.seed);

      // Generate the graph.
      graph g = generate_graph(args, rng);

      assert(is_connected(g));

      // Record the number of nodes and links.
      nns(num_vertices(g));
      nls(num_edges(g));
      
      // Calculate the mean value of the edge lengths.
      auto es = boost::edges(g);
      for (auto ei = es.first; ei != es.second; ++ei)
        lls(boost::get(boost::edge_weight, g, *ei));

      // Calculate the mean value of the node degree.
      auto ns = boost::vertices(g);
      for (auto ni = ns.first; ni != ns.second; ++ni)
        nds(boost::out_degree(*ni, g));

      // Calculate the shortest path statistics.
      calc_sp_stats(g, sphs, spls);
    }

  print_stats("Number of nodes", nns);
  print_stats("Number of links", nls);
  print_stats("Link length", lls);
  print_stats("Node degree", nds);
  print_stats("Shortest path hops", sphs);
  print_stats("Shortest path length", spls);
}

void
simulate(const sdi_args &args_para)
{
  sdi_args args = args_para;

  // Set the routing type.
  routing::set_rt(args.rt);

  // Set the maximal length of a path.
  routing::set_ml(args.ml);

  // Set the K for the k-shortest paths.
  routing::set_K(args.K);

  // Set the spectrum selection type.
  routing::set_st(args.st);

  // Set how the connections should be reconfigured.
  connection::set_re(args.re);

  // This simulation object.
  sim::rng().seed(args.seed);

  // Generate the graph.
  sim::mdl() = generate_graph(args, sim::rng());

  // Make sure there is only one component.
  assert(is_connected(sim::mdl()));

  dbl_acc hop_acc;
  dbl_acc len_acc;
  calc_sp_stats(sim::mdl(), hop_acc, len_acc);
  
  // Calculate the mean connection arrival time.
  args.mcat = calc_mcat(args, sim::mdl(), ba::mean(hop_acc));

  // Calculate the maximal length of a path.
  if (args.mlc)
    args.ml = args.mlc.get() * ba::max(len_acc);

  // The traffic module.
  traffic t(args.mcat, args.mht, args.mnsc);

  // The stats module.
  stats s(args, t);

  // Run the simulation.
  sim::run(args.sim_time);
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
