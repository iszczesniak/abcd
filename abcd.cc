#include "graph.hpp"
#include "cli_args.hpp"
#include "sim.hpp"
#include "stats.hpp"
#include "utils_netgen.hpp"

#include <boost/random.hpp>

using namespace std;

void
simulate(const cli_args &args_para)
{
  cli_args args = args_para;

  // Set the routing type.
  routing::set_rt(args.rt);

  // Set the maximal length of a path.
  routing::set_ml(args.ml);

  // Set the K for the k-shortest paths.
  routing::set_K(args.K);

  // Set the spectrum selection type.
  routing::set_st(args.st);

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
  cli_args args = process_cli_args(argc, argv);
  simulate(args);
    
  return 0;
}
