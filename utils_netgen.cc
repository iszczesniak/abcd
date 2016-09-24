#include "utils_netgen.hpp"

#include "utils.hpp"

#include <cmath>
#include <stdlib.h>     /* srand, rand */
#include <time.h>   
#include <sstream>
#include <iomanip>

#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/range.hpp>

using namespace std;

// Handles the network parameter.
nt_t
nt_interpret (const string &nt)
{
  map <string, nt_t> nt_map;
  nt_map["random"] = nt_t::random_network;
  nt_map["gabriel"] = nt_t::gabriel_network;
  return interpret ("network type", nt, nt_map);
}

void
name_vertices(graph &g)
{
  int count = 1;

  int number = num_vertices(g);
  int width = int(log10(number)) + 1;

  BGL_FORALL_VERTICES(v, g, graph)
    {
      ostringstream out;
      out << "v" << setw(width) << setfill('0') << count++;
      boost::get(boost::vertex_name, g, v) = out.str();
    }
}

void
move(vertex v, const graph &g, std::set<vertex> &lonely,
     std::set<vertex> &connected, std::set<vertex> &saturated)
{
  lonely.erase(v);

  if (num_vertices(g) >= 3)
    connected.insert(v);
  else
    saturated.insert(v);
}

void
move_if_needed(vertex v, const graph &g, std::set<vertex> &connected,
               std::set<vertex> &saturated)
{
  int n = boost::num_vertices(g);
  int od = boost::out_degree(v, g);

  // A node can have the out degree of at most (n - 1).
  assert(od < n);

  // A node is saturated if its out degree is (n - 1).
  if (od == n - 1)
    {
      connected.erase(v);
      saturated.insert(v);
    }
}

graph
generate_gabriel_graph(const cli_args &args)
{
  assert(args.nr_nodes >= 2);
  srand (args.seed);
  // Create a graph with the following number of nodes.
  graph g = graph(args.nr_nodes);

  boost::property_map<graph, boost::edge_weight_t>::type
    wm = get(boost::edge_weight_t(), g);

  // The set of lone vertexes.
  std::set<vertex> lonely = get_vertexes<std::set<vertex> >(g);

  unsigned int number = args.nr_nodes;
  // Square kilometers required for the given number of nodes.
  unsigned skm = number * 10000;
  unsigned int w = std::sqrt(skm);
  unsigned int h = std::sqrt(skm);
  list<TNode *> P = generate_Nodes(w, h, number);
  
  list<TTriangle *> triangles;
  delaunayTriangulation(triangles, P);

  convertDelaunay2GabrielGraph(P);

  unsigned int edges = edgeNumber(P);
  
  map<TNode*, vertex> mapNV;
  
  std::set<vertex>::iterator itV = lonely.begin();
  for (list<TNode *>::iterator it = P.begin(); it != P.end(); ++it, ++itV)
    mapNV[*it] = *itV;
  
  for (list<TNode *>::iterator it = P.begin(); it != P.end(); ++it, ++itV)
  {
      vertex src = mapNV[*it];
      set<TNode*> myEdges = (*it)->getEdges();
      
      for (set<TNode*>::iterator itE = myEdges.begin(); itE != myEdges.end(); ++itE)
      {
         vertex dst = mapNV[*itE];      
         (*itE)->removeEdge(*it);
         edge e;
         bool status;
         tie(e, status) = add_edge(src, dst, g);
         int dist = (int) (sqrt(dist2((*it)->getPoint(), (*itE)->getPoint())) + 0.5);
         wm[e] = dist;
         assert(status);    
      }
  }

  // <AG> zwolnić pamięć dla P i tr
  for (list<TNode *>::iterator it = P.begin(); it != P.end(); ++it)
    delete *it;

  for (list<TTriangle *>::iterator it = triangles.begin();
       it != triangles.end(); ++it)
    delete *it;

  return g;
}

void
calc_sp_stats(const graph &g, dbl_acc &hop_acc, dbl_acc &len_acc)
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
            assert(pred[d] != d);

            // Record the number of hops.
            int hops = 0;
            vertex c = d;
            while(c != s)
              {
                c = pred[c];
                ++hops;
              }
            hop_acc(hops);

            // Record the path length.
            len_acc(dist[d]);
          }
    }
}

double
calc_mcat(const cli_args &args, const graph &g, double mnh)
{
  // The network capacity, i.e. the number of slices of all links.
  double cap = 0;
  for (const auto &e: boost::make_iterator_range(edges (g)))
    cap += boost::get(boost::edge_nosc, g, e);

  // The mean connection arrival time.
  double mcat = args.mht * mnh * args.mnsc / (args.ol * cap);

  return mcat;
}
