#ifndef UTILS_NETGEN_HPP
#define UTILS_NETGEN_HPP

#include "graph.hpp"
#include "utils.hpp"

#include <iterator>
#include <vector>

#include <boost/graph/random.hpp>
#include <boost/graph/iteration_macros.hpp>

/**
 * Names the vertices.
 */
void
name_vertices(Graph &g);

/**
 * Sets the distance property on edges.
 */
template<typename T>
void
set_distances(Graph &g, int min, int max, T &gen)
{
  boost::uniform_int<> range(min, max);
  boost::variate_generator<T &, boost::uniform_int<> > rgen(gen, range);
  boost::randomize_property<edge_weight_t>(g, rgen);
}

/**
 * Sets the subcarriers property on edges.
 */
template<typename G>
void
set_subcarriers(G &g, int subcarriers)
{
  std::vector<int> v(subcarriers);
  
  typename property_map<G, edge_weight2_t>::type pm = get(edge_weight2_t(), g);
  typename graph_traits<G>::edge_iterator ei, ee;
  for (tie(ei, ee) = edges(g); ei != ee; ++ei) {
    pm[*ei] = v;
  }
}

/**
 * Move the vertex from the lonely group to either the set of
 * connected or saturated vertexes.
 */
void
move(Vertex v, const Graph &g, std::set<Vertex> &lonely,
     std::set<Vertex> &connected, std::set<Vertex> &saturated);

/**
 * Check whether to move the vertex from the set of connected vertexes
 * to the set of saturated vertexes.
 */
void
move_if_needed(Vertex v, const Graph &g, std::set<Vertex> &connected,
               std::set<Vertex> &saturated);

/**
 * Add a random edge.
 * @return true on success, false otherwise.
 */
template<typename T>
bool
add_random_edge(Graph &g, std::set<Vertex> &lonely,
                std::set<Vertex> &connected,
                std::set<Vertex> &saturated,
                T &gen)
{
  // The condition for the first edge.
  if (lonely.size() >= 2 && connected.empty() && saturated.empty())
    {
      // Select two lone vertexes, which will be the end nodes of the
      // first edge created.
      Vertex src = get_random_element(lonely, gen);
      // Move the src node from lonely before we pick the dst node.
      move(src, g, lonely, connected, saturated);
      Vertex dst = get_random_element(lonely, gen);
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
      Vertex src = get_random_element(lonely, gen);
      Vertex dst = get_random_element(connected, gen);
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
      Vertex src = get_random_element(connected, gen);
      // These are the vertexes that can be destination nodes.
      set<Vertex> sifted = connected;
      sifted.erase(src);
      BGL_FORALL_OUTEDGES_T(src, e, g, Graph)
        sifted.erase(target(e, g));
      // Now pick from the sifted set.
      Vertex dst = get_random_element(sifted, gen);
      bool status = add_edge(src, dst, g).second;
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
generate_graph(Graph &g, int nodes, int edges, T &gen)
{
  assert(nodes >= 2);
  assert(edges >= 0);

  // Create a graph with the following number of nodes.
  g = Graph(nodes);

  // The set of lone vertexes.
  std::set<Vertex> lonely = get_vertexes<std::set<Vertex> >(g);
  // The set of vertexes in the connected component that have not been
  // saturated yet.
  std::set<Vertex> connected;
  // The set of saturated vertexes.  A saturated edge is connected to
  // every other node with a single edge.
  std::set<Vertex> saturated;

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

#endif /* UTILS_NETGEN_HPP */
