#ifndef UTILS_NETGEN_HPP
#define UTILS_NETGEN_HPP

#include "graph.hpp"
#include "utils.hpp"

#include <iterator>
#include <vector>
#include <iostream>

#include "mypoint.hpp"
#include "teventqueue.hpp"
#include <list>
#include <set>
#include <map>

#include <boost/graph/random.hpp>
#include <boost/graph/iteration_macros.hpp>
#include <boost/iterator/counting_iterator.hpp>

using namespace std;

/**
 * Names the vertices.
 */
void
name_vertices(graph &g);

/**
 * Sets the distance property on edges.
 */
template<typename T>
void
set_distances(graph &g, int min, int max, T &gen)
{
  boost::uniform_int<> range(min, max);
  boost::variate_generator<T &, boost::uniform_int<> > rgen(gen, range);
  boost::randomize_property<boost::edge_weight_t>(g, rgen);
}

/**
 * Sets the subcarriers property on edges.
 */
template<typename G>
void
set_subcarriers(G &g, int subcarriers)
{
  SSC ssc(boost::counting_iterator<int>(0),
          boost::counting_iterator<int>(subcarriers));

  // Total subcarriers property map.
  typename boost::property_map<G, boost::edge_nosc_t>::type
    tpm = get(boost::edge_nosc_t(), g);
  // Available subcarriers property map.
  typename boost::property_map<G, boost::edge_ssc_t>::type
    apm = get(boost::edge_ssc_t(), g);

  typename boost::graph_traits<G>::edge_iterator ei, ee;
  for (tie(ei, ee) = edges(g); ei != ee; ++ei)
    {
      tpm[*ei] = subcarriers;
      apm[*ei] = ssc;
    }
}

/**
 * Prints the subcarriers property of edges.
 */
template<typename G>
void
print_subcarriers(G &g)
{
  typename boost::property_map<G, boost::edge_ssc_t>::type
    pm = boost::get(boost::edge_ssc_t(), g);
  typename boost::graph_traits<G>::edge_iterator ei, ee;
  for (boost::tie(ei, ee) = boost::edges(g); ei != ee; ++ei)
    std::cout << pm[*ei] << "\n";
}

/**
 * Move the vertex from the lonely group to either the set of
 * connected or saturated vertexes.
 */
void
move(vertex v, const graph &g, std::set<vertex> &lonely,
     std::set<vertex> &connected, std::set<vertex> &saturated);

/**
 * Check whether to move the vertex from the set of connected vertexes
 * to the set of saturated vertexes.
 */
void
move_if_needed(vertex v, const graph &g, std::set<vertex> &connected,
               std::set<vertex> &saturated);

/**
 * Add a random edge.
 * @return true on success, false otherwise.
 */
template<typename T>
bool
add_random_edge(graph &g, std::set<vertex> &lonely,
                std::set<vertex> &connected,
                std::set<vertex> &saturated,
                T &gen)
{
  // The condition for the first edge.
  if (lonely.size() >= 2 && connected.empty() && saturated.empty())
    {
      // Select two lone vertexes, which will be the end nodes of the
      // first edge created.
      vertex src = get_random_element(lonely, gen);
      // Move the src node from lonely before we pick the dst node.
      move(src, g, lonely, connected, saturated);
      vertex dst = get_random_element(lonely, gen);
      move(dst, g, lonely, connected, saturated);
      bool status = add_edge(src, dst, g).second;
      assert(status);
      return status;
    }
  // The condition for lonely vertexes and a connected component.
  else if (!lonely.empty() && !connected.empty())
    {
      // Add a new edge where one vertex belongs to the connected
      // component, while the other is a lone one.
      vertex src = get_random_element(lonely, gen);
      vertex dst = get_random_element(connected, gen);
      bool status = add_edge(src, dst, g).second;
      assert(status);
      move(src, g, lonely, connected, saturated);
      move_if_needed(dst, g, connected, saturated);
      return status;
    }
  // The condition for a connected component only.
  else if (lonely.empty() && connected.size() >= 2)
    {
      // Now we have to create an edge where both vertexes of the edge
      // belong to the connected component.  We have to be carefull
      // not to create a parallel edge.
      vertex src = get_random_element(connected, gen);
      // These are the vertexes that can be destination nodes.
      std::set<vertex> sifted = connected;
      sifted.erase(src);
      BGL_FORALL_OUTEDGES_T(src, e, g, graph)
        sifted.erase(target(e, g));
      // Now pick from the sifted set.
      vertex dst = get_random_element(sifted, gen);
      bool status = boost::add_edge(src, dst, g).second;
      assert(status);
      move_if_needed(src, g, connected, saturated);
      move_if_needed(dst, g, connected, saturated);
      return status;
    }

  return false;
}

/**
 * Generate the graph.  The graph has one connected component, but
 * there can be some lone vertexes.  We don't allow loop edges
 * (i.e. that start and end at some node), and we don't allow parallel
 * edges.
 *
 * @return the number of edges actually created.
 */
template<typename T>
int
generate_graph(graph &g, int nodes, int edges, T &gen)
{
  assert(nodes >= 2);
  assert(edges >= 0);

  // Create a graph with the following number of nodes.
  g = graph(nodes);

  // The set of lone vertexes.
  std::set<vertex> lonely = get_vertexes<std::set<vertex> >(g);
  // The set of vertexes in the connected component that have not been
  // saturated yet.
  std::set<vertex> connected;
  // The set of saturated vertexes.  A saturated edge is connected to
  // every other node with a single edge.
  std::set<vertex> saturated;

  // In every iteration we add a new random edge.
  for (int created = 0; created < edges; ++created)
    if (!add_random_edge(g, lonely, connected, saturated, gen))
      {
        assert(lonely.empty());
        assert(connected.size() <= 1);
        assert(saturated.size() >= nodes - 1);
        return created;
      }

  return edges;
}

/**
 * Generate the graph.  The graph has one connected component, but
 * there can be some lone vertexes.  We don't allow loop edges
 * (i.e. that start and end at some node), and we don't allow parallel
 * edges.
 *
 * @return the number of edges actually created.
 */
template<typename T> // <AG> usunąć szablon
unsigned int
generate_Gabriel_graph(graph &g, int nodes)
{
  assert(nodes >= 2);


  // Create a graph with the following number of nodes.
  g = graph(nodes);

  // The set of lone vertexes.
  std::set<vertex> lonely = get_vertexes<std::set<vertex> >(g);
  

  unsigned int w = 100;
  unsigned int h = 100;
  unsigned int number = nodes;
  list<TNode *>P = generate_Nodes(w, h, number);
  
  list<TTriangle *> triangles;
  delaunayTriangulation(triangles, P);

  convertDelaunay2GabrielGraph(P);

  std::cout << "Number of edges: " << edgeNumber(P) << std::endl;
  
  map<TNode*, vertex> mapNV;
  
  // tworzymy odwzorowanie TNode* na vertex z boosta
  std::set<vertex>::iterator itV = lonely.begin();
  for (list<TNode *>::iterator it = P.begin(); it != P.end(); ++it, ++itV)
  {
     mapNV.insert( pair<TNode*, vertex&> (*it, *itV)  );
  }
  unsigned edges = 0;  
  
  for (list<TNode *>::iterator it = P.begin(); it != P.end(); ++it, ++itV)
  {
      vertex src = mapNV[*it];
      set<TNode*> myEdges = (*it)->getEdges();
      
      for (set<TNode*>::iterator itE = myEdges.begin(); itE != myEdges.end(); ++itE)
      {
         ++edges;
         vertex dst = mapNV[*itE];      
         (*itE)->removeEdge(*it);
         edge e;
         bool status;
         tie(e, status) = add_edge(src, dst, g);
         boost::get(boost::edge_weight, g, e) = (int) (dist2( (*it)->getPoint() , (*itE)->getPoint() ) + 0.5);
         assert(status);    
      }
  }
  
  // <AG> zwolnić pamięć dla P i tr
  for (list<TNode *>::iterator it = P.begin(); it != P.end(); ++it, ++itV)
  {
    delete *it;
  }
  
  for (list<TTriangle *>::iterator it =  triangles.begin(); it != triangles.end(); ++it)
  {
    delete *it;
  }
  return edges;
}




#endif /* UTILS_NETGEN_HPP */
