#include "sdi_args.hpp"
#include "connection.hpp"
#include <iostream>
#include <boost/program_options.hpp>

using namespace std;
namespace po = boost::program_options;

sdi_args
process_sdi_args(int argc, char *argv[])
{
  sdi_args result;

  try
    {
      po::options_description opts("The options");
      opts.add_options()
        ("help,h", "produce help message")

        ("nodes,n", po::value<int>(),
         "the number of nodes to generate")

        ("edges,e", po::value<int>(),
         "the number of edges to generate")

        ("reconf,r", po::value<string>(),
         "the type of connection reconfiguration")

        ("seed,s", po::value<int>()->default_value(1),
         "the seed of the random number generator");

      po::variables_map vm;
      po::store(po::command_line_parser(argc, argv).options(opts).run(), vm);
      po::notify(vm);

      if (vm.count("help"))
        {
          cout << opts << "\n";
          exit(0);
        }

      if(vm.count("nodes"))
        result.nr_nodes = vm["nodes"].as<int>();
      else
        {
          cerr << "You need to give me the number "
               << "of nodes to generate.\n";
          exit(1);
        }

      if(vm.count("edges"))
        result.nr_edges = vm["edges"].as<int>();
      else
        {
          cerr << "You need to give me the number "
               << "of edges to generate.\n";
          exit(1);
        }

      if(vm.count("reconf"))
        {
          string reconf = vm["reconf"].as<string>();
        }
      else
        {
          cerr << "You need to give me the type of the "
               << "connection reconfiguration.  Choose "
               << "one of: " << "\n";
          exit(1);
        }

      // The seed for the random number generator.
      result.seed = vm["seed"].as<int>();
    }
  catch(const std::exception& e)
    {
      cerr << e.what() << std::endl;
      exit(1);
    }

  return result;
}
