#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>
#include <boost/random.hpp>

#include "graph.hpp"
#include "sdi_args.hpp"
#include "utils_netgen.hpp"

int main() {
  sdi_args args;

  args.network = random_network;
  args.nr_nodes = 100;
  args.nr_edges = 400;
  
  boost::mt19937 rng(1);

  graph g = generate_graph(args, rng);

  vertex s = 0, t = 1;

  boost::successive_shortest_path_nonnegative_weights(g, s, t);
  int cost =  boost::find_flow_cost(g);

  return 0;
}
