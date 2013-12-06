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
         "the lambda for the sleep time")

        ("l_change", po::value<double>()->required(),
         "the lambda for the sleep time")

        ("seed", po::value<int>()->default_value(1),
         "the seed of the random number generator");

      po::variables_map vm;
      po::store(po::command_line_parser(argc, argv).options(opts).run(), vm);

      if (vm.count("help"))
        {
          cout << opts << "\n";
          exit(0);
        }

      po::notify(vm);

      result.nr_nodes = vm["nodes"].as<int>();
      result.nr_edges = vm["edges"].as<int>();
      result.nr_sc = vm["subcarriers"].as<int>();
      result.nr_clients = vm["clients"].as<int>();
      result.l_sleep = vm["l_sleep"].as<double>();
      result.mnc = vm["mnc"].as<double>();
      result.l_change = vm["l_change"].as<double>();
      result.seed = vm["seed"].as<int>();
    }
  catch(const std::exception& e)
    {
      cerr << e.what() << std::endl;
      exit(1);
    }

  return result;
}
