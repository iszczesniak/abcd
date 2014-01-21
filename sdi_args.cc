#include "sdi_args.hpp"
#include "connection.hpp"

#include <iostream>
#include <map>

#include <boost/program_options.hpp>

using namespace std;
namespace po = boost::program_options;

// Handles the reconf parameter.
connection::reconf_t
reconf_interpret(const string &reconf)
{
  typedef map<string, connection::reconf_t> reconf_map_t;
  reconf_map_t reconf_map;
  reconf_map["part"] = connection::part;
  reconf_map["anew"] = connection::anew;
  reconf_map["retrace"] = connection::retrace;

  reconf_map_t::iterator i = reconf_map.find(reconf);

  if (i == reconf_map.end())
    {
      cerr << "Wrong value of reconf.  Choose one from:";
      for(reconf_map_t::iterator i = reconf_map.begin();
          i != reconf_map.end(); ++i)
        cerr << " " << i->first;
      cerr << endl;
      exit(1);
    }

  return i->second;
}

// Handles the select parameter.
dijkstra::select_t
select_interpret(const string &select)
{
  typedef map<string, dijkstra::select_t> select_map_t;
  map<string, dijkstra::select_t> select_map;
  select_map["first"] = dijkstra::first;
  select_map["fitest"] = dijkstra::fitest;

  select_map_t::iterator i = select_map.find(select);

  if (i == select_map.end())
    {
      cerr << "Wrong value of select.  Choose one from:";
      for(select_map_t::iterator i = select_map.begin();
          i != select_map.end(); ++i)
        cerr << " " << i->first;
      cerr << endl;
      exit(1);
    }

  return i->second;
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

        ("network", po::value<string>()->required(),
         "the network type")

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

        ("seed", po::value<int>()->default_value(1),
         "the seed of the random number generator")

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
      result.hash = vm["hash"].as<string>();
      result.reconf = reconf_interpret(vm["reconf"].as<string>());
      result.select = select_interpret(vm["select"].as<string>());
    }
  catch(const std::exception& e)
    {
      cerr << e.what() << std::endl;
      exit(1);
    }

  return result;
}
