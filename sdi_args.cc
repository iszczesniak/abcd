#include "sdi_args.hpp"

#include "connection.hpp"
#include "routing.hpp"
#include "utils.hpp"

#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include <boost/program_options.hpp>

// Option strings.
#define NT_S "nt"
#define ST_S "st"
#define RT_S "rt"
#define RE_S "re"
#define MLC_S "mlc"
#define K_S "K"
#define EDGES_S "edges"
#define NODES_S "nodes"

using namespace std;
namespace po = boost::program_options;

template <typename T>
void requires (const po::variables_map& vm,
               const char* for_arg, const T for_val,
               const char* req)
{
  // Make sure the argument we are looking for is present.
  if (vm.count (for_arg))
    // Make sure the argument has the right value.
    if (vm[for_arg].as<T> () == for_val)
      // Throw, if the required argument is not present.
      if (vm.count (req) == 0)
        {
          ostringstream os;
          os << "Option '" << for_arg 
             << "' that is equal to '" << for_val
             << "' requires option '" << req << "'.";

          throw logic_error (os.str());
        }
}

void requires (const po::variables_map& vm,
               const char* for_arg, const char* req)
{
  // Make sure the argument we are looking for is present.
  if (vm.count (for_arg))
    // Throw, if the required argument is not present.
    if (vm.count (req) == 0)
      throw logic_error (string ("Option '") + for_arg 
                         + "' requires option '" + req + "'.");
}

sdi_args
process_sdi_args(int argc, const char *argv[])
{
  sdi_args result;

  try
    {
      // General options.
      po::options_description gen("General options");
      gen.add_options()
        ("help,h", "produce help message")

        ("net_stats", "produce network stats");

      // Network options.
      po::options_description net("Network options");
      net.add_options()
        (NT_S, po::value<string>()->required(),
         "the network type")

        (NODES_S, po::value<int>(),
         "the number of nodes to generate")

        (EDGES_S, po::value<int>(),
         "the number of edges to generate")

        ("slices", po::value<int>()->required(),
         "the number of slices")

        (MLC_S, po::value<double>(),
         "the maximal length cofficient of a path")

        (K_S, po::value<int>(),
         "the K for the k-shortest paths")

        (ST_S, po::value<string>()->required(),
         "the spectrum selection type")

        (RT_S, po::value<string>()->required(),
         "the routing algorithm")

        (RE_S, po::value<string>()->required(),
         "the reconfiguration type");

      // Traffic options.
      po::options_description tra("Traffic options");
      tra.add_options()
        ("ol", po::value<double>()->required(),
         "the offered load")

        ("mht", po::value<double>()->required(),
         "the mean holding time")

        ("mnsc", po::value<double>()->required(),
         "the mean number of slices");

      // Simulation options.
      po::options_description sim("Simulation options");
      sim.add_options()
        ("seed", po::value<int>()->default_value(1),
         "the seed of the random number generator")

        ("sim_time", po::value<double>()->default_value(100),
         "the limit on the simulation time")

        ("hash", po::value<string>()->required(),
         "the hash of all the parameters except the seed");

      po::options_description all("Allowed options");
      all.add(gen).add(net).add(tra).add(sim);
      
      po::variables_map vm;
      po::store(po::command_line_parser(argc, argv).options(all).run(), vm);

      // Dependencies.
      requires(vm, NT_S, NODES_S);
      requires(vm, NT_S, string("random"), EDGES_S);

      if (vm.count("help"))
        {
          cout << all << "\n";
          exit(0);
        }

      // If there is something wrong with parameters, we will get
      // notified automatically and the program will exit.
      po::notify(vm);

      // The general options.
      if (vm.count("net_stats"))
        result.net_stats = true;

      // The network options.
      result.nt = vm[NT_S].as<string>();
      result.nr_nodes = vm[NODES_S].as<int>();

      if (vm.count(EDGES_S))
        result.nr_edges = vm[EDGES_S].as<int>();
      
      result.nr_sc = vm["slices"].as<int>();

      if (vm.count(MLC_S))
        result.mlc = vm[MLC_S].as<double>();

      if (vm.count(K_S))
        result.K = vm[K_S].as<int>();

      result.st = vm[ST_S].as<string>();
      result.rt = vm[RT_S].as<string>();
      result.re = vm[RE_S].as<string>();

      // The traffic options.
      result.ol = vm["ol"].as<double>();
      result.mht = vm["mht"].as<double>();
      result.mnsc = vm["mnsc"].as<double>();

      // The simulation options.
      result.seed = vm["seed"].as<int>();
      result.hash = vm["hash"].as<string>();
      result.sim_time = vm["sim_time"].as<double>();
    }
  catch(const std::exception& e)
    {
      cerr << e.what() << std::endl;
      exit(1);
    }

  return result;
}
