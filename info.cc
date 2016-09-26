#include "graph.hpp"
#include "cli_args.hpp"
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
net_stats(const cli_args &args_orig)
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
      cli_args args = args_orig;
      
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

int
main(int argc, const char* argv[])
{
  cli_args args = process_cli_args(argc, argv);
  net_stats(args);
    
  return 0;
}
