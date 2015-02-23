#include "utils_netgen.hpp"

#include <stdlib.h>     /* srand, rand */
#include <time.h>   
#include <sstream>
#include <iomanip>

using namespace std;

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
generate_gabriel_graph(const sdi_args &args)
{
  assert(args.nr_nodes >= 2);
  srand (args.seed);
  // Create a graph with the following number of nodes.
  graph g = graph(args.nr_nodes);

  // The set of lone vertexes.
  std::set<vertex> lonely = get_vertexes<std::set<vertex> >(g);

  unsigned int w = 100;
  unsigned int h = 100;
  unsigned int number = args.nr_nodes;
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
         boost::get(boost::edge_weight, g, e) = dist;
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
