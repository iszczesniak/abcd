#include <iostream>

#include "cdijkstra.hpp"
#include "graph.hpp"
#include "sdi_args.hpp"
#include "stats.hpp"
#include "utils_netgen.hpp"

#include <boost/random.hpp>

using namespace std;

main(int argc, const char* argv[])
{
  sdi_args args;
  args.nt = "random";
  args.nr_sc = 0;
  
  for (int n = 2; n <= 100; ++n)
    for (int e = 2; e <= (n * (n - 1) / 2); ++e)
      for (int i = 1; i <= 1000; ++i)
        {
          cout << "n = " << n
               << ", e = " << e
               << ", i = " << i << endl;

          args.nr_nodes = n;
          args.nr_edges = e;
          
          // Random number generator.
          boost::mt19937 rng(i);

          // Generate the graph.
          graph g = generate_graph(args, rng);
          
          if (is_connected(g))
            {
              auto ns = boost::vertices(g);

              // Calculate stats for shortest paths.
              for (auto ni = ns.first; ni != ns.second; ++ni)
                {
                  vector<int> dist(num_vertices(g));
                  vector<vertex> pred(num_vertices(g));

                  vertex s = *ni;
                  
                  boost::dijkstra_shortest_paths
                    (g, s, boost::predecessor_map(&pred[0]).distance_map(&dist[0]));

                  for (auto nj = ns.first; nj != ns.second; ++nj)
                    if (ni != nj)
                      {
                        vertex d = *nj;

                        // Make sure the path was found.
                        if (pred[d] == d)
                          {
                            cout << "s = " << s << endl;
                            cout << "d = " << d << endl;
                            cout << "pred[d] = " << pred[d] << endl;
                            cout << "in_degree = " << in_degree(d, g) << endl;
                            cout << "out_degree = " << out_degree(d, g) << endl;
                            cout.flush();
                            abort();
                          }
                      }
                }
            }
        }

  return 0;
}
