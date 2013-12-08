#include <list>
#include <queue>

#include "client.hpp"
#include "event.hpp"
#include "graph.hpp"
#include "sdi_args.hpp"
#include "stats.hpp"
#include "utils_netgen.hpp"

#include <boost/random.hpp>

using namespace std;

int
main(int argc, const char* argv[])
{
  sdi_args args = process_sdi_args(argc, argv);

  // Simulation time.
  const double sim_limit = 100;

  // Random number generator.
  boost::mt19937 gen(args.seed);

  // Generate the graph.
  graph g;
  int res = generate_graph(g, args.nr_nodes, args.nr_edges, gen);
  assert(res == args.nr_edges);

  // Make sure there is only one component.
  assert(check_components(g));

  // Name the vertexes.
  name_vertices(g);

  // The number of subcarriers for each edge.
  set_subcarriers(g, args.nr_sc);

  // The DES priority queue.
  pqueue q;

  // The stats module.
  stats s(args, g, q);

  // The list of clients.
  std::vector<module *> vc;

  // Create the modules for the simulation.
  for(int i = 0; i < args.nr_clients; ++i)
    {
      client *c = new client(g, q, i, gen,
                             args.l_sleep, args.mnc, args.l_change);
      c->schedule(0);
      vc.push_back(c);
    }

  // The event loop.
  while(!q.empty())
    {
      if (q.top().get_time() > sim_limit)
	break;

      q.top().process();
      q.pop();
    }

  // Delete the clients.
  for(std::vector<module *>::const_iterator i = vc.begin();
      i != vc.end(); ++i)
    delete *i;
}
