#ifndef UTILS_NETGEN_HPP
#define UTILS_NETGEN_HPP

#include "graph.hpp"
#include "mypoint.hpp"
#include "cli_args.hpp"
#include "utils.hpp"
#include "teventqueue.hpp"

#include <iostream>
#include <iterator>
#include <set>
#include <vector>
#include <utility>

#include <boost/graph/random.hpp>
#include <boost/graph/iteration_macros.hpp>
#include <boost/iterator/counting_iterator.hpp>

using namespace std;

// The network type.
enum class nt_t {random_network, gabriel_network};

nt_t
nt_interpret (const string &nt);

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
 * Sets the slices property on edges.
 */
template<typename G>
void
set_slices(G &g, int slices)
{
  // This is the set of contiguous slices, from 0 to (slices
  // - 1).
  SSC ssc(boost::counting_iterator<int>(0),
          boost::counting_iterator<int>(slices));

  // Total slices property map.
  typename boost::property_map<G, boost::edge_nosc_t>::type
    tpm = get(boost::edge_nosc_t(), g);
  // Available slices property map.
  typename boost::property_map<G, boost::edge_ssc_t>::type
    apm = get(boost::edge_ssc_t(), g);

  typename G::edge_iterator ei, ee;
  for (tie(ei, ee) = edges(g); ei != ee; ++ei)
    {
      tpm[*ei] = slices;
      apm[*ei] = ssc;
    }
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
  // The condition for the first edge ever created in the graph.
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
graph
generate_random_graph(const cli_args &args, T &gen)
{
  assert(args.nr_nodes >= 2);
  assert(args.nr_edges.get() >= 0);

  // Create a graph with the following number of nodes.
  graph g = graph(args.nr_nodes);

  // The set of lone vertexes.
  std::set<vertex> lonely = get_vertexes<std::set<vertex> >(g);
  // The set of vertexes in the connected component that have not been
  // saturated yet.
  std::set<vertex> connected;
  // The set of saturated vertexes.  A saturated node is connected to
  // every other node with a single edge.
  std::set<vertex> saturated;

  // In every iteration we add a new random edge.
  for (int created = 0; created < args.nr_edges.get(); ++created)
    if (!add_random_edge(g, lonely, connected, saturated, gen))
      {
        assert(lonely.empty());
        assert(connected.size() <= 1);
        assert(saturated.size() >= args.nr_nodes - 1);
        // Fail, because we can't create the requested number of edges.
        abort();
      }

  // Make sure we created the requested number of edges.
  assert(num_edges (g) == args.nr_edges.get());

  // The distance for each edge.
  set_distances(g, 100, 200, gen);

  return g;
}

/**
 * Generate the Gabriel graph.  The graph has one connected component.
 * We don't allow for loop edges (i.e. that start and end at the same
 * node), and we don't allow for parallel edges.
 *
 * @return the graph
 */

graph
generate_gabriel_graph(const cli_args &args);

template<typename T>
graph
generate_graph(const cli_args &args, T &gen)
{
  graph g;

  nt_t nt = nt_interpret(args.nt);
  switch (nt)
    {
    case nt_t::random_network:
      g = generate_random_graph(args, gen);
      break;
      
    case nt_t::gabriel_network:
      g = generate_gabriel_graph(args);
      break;

    default:
      abort();
    }
  
  // Name the vertexes.
  name_vertices(g);

  // The number of slices for each edge.
  set_slices(g, args.nr_sc);

  return g;
}

// For the shortest paths between all node pairs, calculate the
// statistics for hops and lengths.
void
calc_sp_stats(const graph &g, dbl_acc &hop_acc, dbl_acc &len_acc);

// Calculate the mean connection arrival time for the given arguments
// and the given graph.
double
calc_mcat(const cli_args &args, const graph &g, double mnh);

#endif /* UTILS_NETGEN_HPP */
