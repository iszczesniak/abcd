#ifndef UTILS_HPP
#define UTILS_HPP

#include "graph.hpp"

#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>

#include <boost/graph/dijkstra_shortest_paths.hpp>

#include <cassert>
#include <iostream>
#include <list>
#include <set>
#include <string>
#include <vector>
#include <set>

/**
 * Exclude those subcarriers that won't be able to support a signal
 * with p subcarriers.
 */

SSC
exclude(const SSC &ssc, int p);

/**
 * Calculates the shortests paths in the graph.
 */

template<typename G>
void complete_graph(G &g)
{
  // Here we do all-pair shortest path.  There are two things to note.
  // 1: We could use the Floyd-Warshal or the Johnson algorithms here,
  // but they return only the distances.  They don't return the
  // parents. 2: We want to shortest paths to a destination from other
  // nodes, and the Dijksta algorithm does the opposite: it returns
  // the shortest paths from a source node to other nodes.  For us it
  // doesn't matter, because links are undirected.
  vector<int> dist(num_vertices(g));
  vector<Vertex> pred(num_vertices(g));

  typename graph_traits<G>::vertex_iterator vi, ve;
  for (tie(vi, ve) = vertices(g); vi != ve; ++vi)
    {
      typename graph_traits<G>::vertex_descriptor v = *vi;

      dijkstra_shortest_paths
        (g, v, predecessor_map(&pred[0]).distance_map(&dist[0]));

      put(vertex_distance, g, v, dist);
      put(vertex_predecessor, g, v, pred);
    }
}

/**
 * Prints a path from node i to node j in graph g.
 */

template<typename G>
std::string
path_to_string(typename graph_traits<G>::vertex_descriptor i,
               typename graph_traits<G>::vertex_descriptor j,
               const G &g)
{
  std::ostringstream str;

  str << "From " << get(vertex_name, g, i)
      << " to " << get(vertex_name, g, j);

  // Check if the shortest path from node i to node j exists.
  if (i == j || get(vertex_predecessor, g, j)[i] != i)
    {
      int hops = 0;
      typename graph_traits<Graph>::vertex_descriptor curr = i;

      str << ": " << get(vertex_name, g, i);

      while(curr != j)
        {
          curr = get(vertex_predecessor, g, j)[curr];
         str << " -> " << get(vertex_name, g, curr);
          ++hops;
        }

      str << ", hops = " << hops;
      str << ", distance = " << get(vertex_distance, g, i)[j];
    }
  else
    str << "doesn't exist";

  return str.str();
}

/**
 * Prints the information on the shortest paths.
 */

template<typename G>
void
print_sp(const G &g, std::ostream &os)
{
  os << "******************************************************\n";
  os << "SHORTEST PATHS\n";
  os << "******************************************************\n";

  // Lists the path from i to j.

  typename graph_traits<G>::vertex_iterator i, ie;
  typename graph_traits<G>::vertex_iterator j, je;
  
  for (tie(i, ie) = vertices(g); i != ie; ++i)
    for (tie(j, je) = vertices(g); j != je; ++j)
      os << path_to_string(*i, *j, g) << std::endl;
}

/**
 * Return a container with vertexes of a graph.
 */
template<typename C>
C
get_vertexes(const Graph &g)
{
  C c;

  // Copy the vertex descriptors.
  std::copy(vertices(g).first, vertices(g).second,
            inserter(c, c.begin()));

  return c;
}

/**
 * Get a random element from a container.
 */
template <typename C, typename T>
typename C::value_type
get_random_element(const C &c, T &gen)
{
  assert(!c.empty());

  uniform_int<> dist(0, c.size() - 1);
  variate_generator<T &, uniform_int<> > rand_gen(gen, dist);

  typename C::const_iterator i = c.begin();
  std::advance(i, rand_gen());

  return *i;
}

/**
 * This is the << operator for a vector.
 */
template <typename T>
ostream &operator << (ostream &os, const vector<T> &v)
{
  typename vector<T>::const_iterator i = v.begin();

  while(i != v.end())
    {
      os << *i;
      if (++i != v.end())
        os << ", ";
    }

  return os;
}

/**
 * This is the << operator for a set.
 */
template <typename T>
ostream &operator << (ostream &os, const set<T> &v)
{
  typename set<T>::const_iterator i = v.begin();

  while(i != v.end())
    {
      os << *i;
      if (++i != v.end())
        os << ", ";
    }

  return os;
}

/**
 * Make sure that the graph has only one connected component, and that
 * perhaps there are some lonely vertexes.  Sort the list in the
 * decreasing order of the number of elements in the sets.
 */
std::list<std::set<Vertex> >
get_components(const Graph &g);

/**
 * There must be only one connected component.  There could be other
 * components, but they can't be connected (i.e. they are lone
 * vertexes).
 *
 * @return: true if only one component is connected, false otherwise.
 */
template<typename G>
bool
check_components(const G &g)
{
  typedef typename graph_traits<G>::vertex_descriptor Vertex;
  typedef typename std::list<std::set<Vertex> > lsv;

  lsv l = get_components(g);

  int connected = 0;
  // Count the number of connected components.
  for (typename lsv::iterator i = l.begin(); i != l.end(); ++i)
    connected += (i->size() >= 2);

  return connected == 1;
}

/**
 * Generate a random node pair.
 */

template<typename G, typename R>
std::pair<typename graph_traits<G>::vertex_descriptor,
          typename graph_traits<G>::vertex_descriptor>
random_node_pair(const G &g, R &gen)
{
  typedef typename graph_traits<G>::vertex_descriptor Vertex;

  assert(check_components(g));
  std::set<Vertex> s = *get_components(g).begin();

  assert(s.size());
  Vertex src = get_random_element(s, gen);
  s.erase(src);
  assert(s.size());
  Vertex dst = get_random_element(s, gen);

  return std::make_pair(src, dst);
}

#endif /* UTILS_HPP */
