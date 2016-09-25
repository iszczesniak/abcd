#include "cli_args.hpp"

#include "cdijkstra.hpp"
#include "utils.hpp"
#include "utils_netgen.hpp"

#include <iostream>

#define BOOST_TEST_MODULE Arguments

#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

/*
 * Test that the arguments are interpreted right with the default seed
 * value of 1.
 */
BOOST_AUTO_TEST_CASE(cli_args_test_1)
{
  const char *argv[] = {"",
                        "--nt", "random",
                        "--nodes", "10",
                        "--edges", "30",
                        "--slices", "50",
                        "--ol", "1",
                        "--mht", "2",
                        "--mnsc", "5",
                        "--st", "first",
                        "--rt", "cdijkstra",
                        "--mlc", "2",
                        "--hash", "blablabla"};

  int argc = sizeof(argv) / sizeof(char *);

  cli_args args = process_cli_args(argc, argv);

  routing::set_rt(args.rt);
  routing::set_st(args.st);

  BOOST_CHECK(args.hash == "blablabla");

  BOOST_CHECK(args.nr_nodes == 10);
  BOOST_CHECK(args.nr_edges == 30);
  BOOST_CHECK(args.nr_sc == 50);
  BOOST_CHECK(args.mlc.get() == 2);
  BOOST_CHECK(routing::get_st() == routing::st_t::first);
  BOOST_CHECK(routing::get_rt() == routing::rt_t::cdijkstra);
  
  BOOST_CHECK_CLOSE(args.ol, 1, 0.0001);
  BOOST_CHECK_CLOSE(args.mht, 2, 0.0001);
  BOOST_CHECK_CLOSE(args.mnsc, 5.0, 0.0001);

  BOOST_CHECK(args.seed == 1);
}

/*
 * Make sure that:
 * - seed is 2
 * - st is fittest
 * - ml is 1000
 */
BOOST_AUTO_TEST_CASE(cli_args_test_2)
{
  const char *argv[] = {"",
                        "--nt", "random",
                        "--nodes", "10",
                        "--edges", "30",
                        "--slices", "50",
                        "--ol", "1",
                        "--mht", "2",
                        "--mnsc", "5",
                        "--st", "fittest",
                        "--hash", "blablabla",
                        "--rt", "cdijkstra",
                        "--seed", "2"};

  int argc = sizeof(argv) / sizeof(char *);

  cli_args args = process_cli_args(argc, argv);
  routing::set_st(args.st);

  BOOST_CHECK(args.seed == 2);
  BOOST_CHECK(routing::get_st() == routing::st_t::fittest);
}
