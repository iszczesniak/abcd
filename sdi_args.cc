#include "sdi_args.hpp"
#include "connection.hpp"

#include <iostream>
#include <map>
#include <string>

#include <boost/program_options.hpp>

using namespace std;
namespace po = boost::program_options;

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
dijkstra::select_t
select_interpret (const string &select)
{
  map <string, dijkstra::select_t> select_map;
  select_map["first"] = dijkstra::first;
  select_map["fittest"] = dijkstra::fittest;
  return interpret ("select", select, select_map);
}

// Handles the select parameter.
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
      po::options_description opts("The options");

      opts.add_options()
        ("help,h", "produce help message")

        ("nodes", po::value<int>()->required(),
         "the number of nodes to generate")

        ("edges", po::value<int>()->required(),
         "the number of edges to generate")

        ("subcarriers", po::value<int>()->required(),
         "the number of subcarriers")

        ("clients", po::value<int>()->required(),
         "the number of clients")

        ("l_sleep", po::value<double>()->required(),
         "the lambda for the sleep time")

        ("mnc", po::value<double>()->required(),
         "the mean number of changes")

        ("l_change", po::value<double>()->required(),
         "the lambda for the change time")

        ("mnsc", po::value<double>()->required(),
         "the mean number of subcarriers")

        ("reconf", po::value<string>()->required(),
         "the way connections should be reconfigured")

        ("select", po::value<string>()->required(),
         "the way subcarriers should be selected")

        ("network", po::value<string>()->required()->default_value("random"),
         "the graph type")

        ("seed", po::value<int>()->default_value(1),
         "the seed of the random number generator")

        ("sim_time", po::value<double>()->default_value(100),
         "the limit on the simulation time")

        ("max_len", po::value<int>()->required(),
         "the maximal length of a path")

        ("hash", po::value<string>()->required(),
         "the hash of all the parameters except the seed");

      po::variables_map vm;
      po::store(po::command_line_parser(argc, argv).options(opts).run(), vm);

      if (vm.count("help"))
        {
          cout << opts << "\n";
          exit(0);
        }

      // If there is something wrong with parameters, we will get
      // notified automatically and the program will exit.
      po::notify(vm);

      result.nr_nodes = vm["nodes"].as<int>();
      result.nr_edges = vm["edges"].as<int>();
      result.nr_sc = vm["subcarriers"].as<int>();
      result.nr_clients = vm["clients"].as<int>();
      result.l_sleep = vm["l_sleep"].as<double>();
      result.mnc = vm["mnc"].as<double>();
      result.l_change = vm["l_change"].as<double>();
      result.mnsc = vm["mnsc"].as<double>();
      result.seed = vm["seed"].as<int>();
      result.sim_time = vm["sim_time"].as<double>();
      result.max_len = vm["max_len"].as<int>();
      result.hash = vm["hash"].as<string>();
      result.reconf = reconf_interpret(vm["reconf"].as<string>());
      result.select = select_interpret(vm["select"].as<string>());
      result.network = network_interpret(vm["network"].as<string>());
    }
  catch(const std::exception& e)
    {
      cerr << e.what() << std::endl;
      exit(1);
    }

  return result;
}
