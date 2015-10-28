#include "sdi_args.hpp"
#include "connection.hpp"
#include "routing.hpp"

#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include <boost/program_options.hpp>

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

template <typename T>
T
interpret (const string &name, const string &text,
           const map <string, T> &m)
{
  auto i = m.find (text);

  if (i == m.end ())
    {
      cerr << "Wrong value of " << name << ".  Choose one of:";
      for (auto &p: m)
        cerr << " " << p.first;
      cerr << endl;
      exit (1);
    }

  return i->second;
}

// Handles the routing parameter.
routing::routing_t
rt_interpret (const string &rt)
{
  map <string, routing::routing_t> rt_map;
  rt_map["cdijkstra"] = routing::cdijkstra;
  rt_map["ed_ksp"] = routing::ed_ksp;
  return interpret ("rt", rt, rt_map);
}

// Handles the reconf parameter.
connection::reconf_t
reconf_interpret (const string &reconf)
{
  map <string, connection::reconf_t> reconf_map;
  reconf_map["complete"] = connection::complete;
  reconf_map["incremental"] = connection::incremental;
  reconf_map["curtailing"] = connection::curtailing;
  return interpret ("reconf", reconf, reconf_map);
}

// Handles the select parameter.
routing::select_t
select_interpret (const string &select)
{
  map <string, routing::select_t> select_map;
  select_map["first"] = routing::first;
  select_map["fittest"] = routing::fittest;
  return interpret ("select", select, select_map);
}

// Handles the network parameter.
network_t
network_interpret (const string &network)
{
  map <string, network_t> network_map;
  network_map["random"] = network_t::random_network;
  network_map["gabriel"] = network_t::gabriel_network;
  return interpret ("network", network, network_map);
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
        ("network", po::value<string>()->required(),
         "the graph type")

        ("nodes", po::value<int>()->required(),
         "the number of nodes to generate")

        ("edges", po::value<int>()->default_value(0),
         "the number of edges to generate")

        ("subcarriers", po::value<int>()->required(),
         "the number of subcarriers")

        ("max_len", po::value<int>()->required(),
         "the maximal length of a path")

        ("reconf", po::value<string>()->required(),
         "the way connections should be reconfigured")

        ("select", po::value<string>()->required(),
         "the way subcarriers should be selected")

        ("rt", po::value<string>()->required(),
         "the routing algorithm");

      // Traffic options.
      po::options_description tra("Traffic options");
      tra.add_options()
        ("mcat", po::value<double>()->required(),
         "the mean client-arrival time")

        ("mht", po::value<double>()->required(),
         "the mean holding time")

        ("mbst", po::value<double>()->required(),
         "the mean BS-change time")

        ("mdct", po::value<double>()->required(),
         "the mean DC-change time")

        ("mnsc", po::value<double>()->required(),
         "the mean number of subcarriers");

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
      requires(vm, "network", "nodes");
      requires(vm, "network", string("random"), "edges");

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
      result.network = network_interpret(vm["network"].as<string>());
      result.nr_nodes = vm["nodes"].as<int>();
      result.nr_edges = vm["edges"].as<int>();
      result.nr_sc = vm["subcarriers"].as<int>();
      result.max_len = vm["max_len"].as<int>();
      result.reconf = reconf_interpret(vm["reconf"].as<string>());
      result.select = select_interpret(vm["select"].as<string>());
      result.rt = rt_interpret(vm["rt"].as<string>());

        // The traffic options.
      result.mcat = vm["mcat"].as<double>();
      result.mht = vm["mht"].as<double>();
      result.mbst = vm["mbst"].as<double>();
      result.mdct = vm["mdct"].as<double>();
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
