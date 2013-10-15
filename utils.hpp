#ifndef UTILS_HPP
#define UTILS_HPP

#include "graph.hpp"

#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>

#include <boost/graph/dijkstra_shortest_paths.hpp>

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>

#include <cassert>
#include <iostream>
#include <list>
#include <set>
#include <string>
#include <vector>
#include <set>

namespace acc = boost::accumulators;

/**
 * Generate a random number in [min, max].
 */
template<typename T>
int
get_random_int(int min, int max, T &gen)
{
  boost::uniform_int<> range(min, max);
  boost::variate_generator<T &, boost::uniform_int<> > rgen(gen, range);
  return rgen();
}

/**
 * Check whether b is a subset of a.
 */
bool
includes(const SSC &a, const SSC &b);

/**
 * SSC intersection.
 */
SSC
intersection(const SSC &s1, const SSC &s2);

/**
 * Exclude those subcarriers that won't be able to support a signal
 * with p subcarriers.
 */

SSC
exclude(const SSC &ssc, int p);

/**
 * Exclude e from ssc.
 */
void
exclude(SSC &ssc, const SSC &e);

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
  std::vector<int> dist(num_vertices(g));
  std::vector<Vertex> pred(num_vertices(g));

  typename boost::graph_traits<G>::vertex_iterator vi, ve;
  for (tie(vi, ve) = vertices(g); vi != ve; ++vi)
    {
      typename boost::graph_traits<G>::vertex_descriptor v = *vi;

      boost::dijkstra_shortest_paths
        (g, v, boost::predecessor_map(&pred[0]).distance_map(&dist[0]));

      put(boost::vertex_distance, g, v, dist);
      put(boost::vertex_predecessor, g, v, pred);
    }
}

/**
 * Prints a path from node i to node j in graph g.
 */

template<typename G>
std::string
path_to_string(typename boost::graph_traits<G>::vertex_descriptor i,
               typename boost::graph_traits<G>::vertex_descriptor j,
               const G &g)
{
  std::ostringstream str;

  str << "From " << boost::get(boost::vertex_name, g, i)
      << " to " << boost::get(boost::vertex_name, g, j);

  // Check if the shortest path from node i to node j exists.
  if (i == j || boost::get(boost::vertex_predecessor, g, j)[i] != i)
    {
      int hops = 0;
      typename boost::graph_traits<Graph>::vertex_descriptor curr = i;

      str << ": " << boost::get(boost::vertex_name, g, i);

      while(curr != j)
        {
          curr = boost::get(boost::vertex_predecessor, g, j)[curr];
	  str << " -> " << get(boost::vertex_name, g, curr);
          ++hops;
        }

      str << ", hops = " << hops;
      str << ", distance = "
	  << boost::get(boost::vertex_distance, g, i)[j];
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

  typename boost::graph_traits<G>::vertex_iterator i, ie;
  typename boost::graph_traits<G>::vertex_iterator j, je;
  
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

  boost::uniform_int<> dist(0, c.size() - 1);
  boost::variate_generator<T &, boost::uniform_int<> >
    rand_gen(gen, dist);

  typename C::const_iterator i = c.begin();
  std::advance(i, rand_gen());

  return *i;
}

/**
 * This is the << operator for a vector.
 */
template <typename T>
std::ostream &operator << (std::ostream &os, const std::vector<T> &v)
{
  typename std::vector<T>::const_iterator i = v.begin();

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
std::ostream &operator << (std::ostream &os, const std::set<T> &v)
{
  typename std::set<T>::const_iterator i = v.begin();

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
  typedef typename boost::graph_traits<G>::vertex_descriptor Vertex;
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
std::pair<typename boost::graph_traits<G>::vertex_descriptor,
	  typename boost::graph_traits<G>::vertex_descriptor>
random_node_pair(const G &g, R &gen)
{
  typedef typename boost::graph_traits<G>::vertex_descriptor Vertex;

  assert(check_components(g));
  std::set<Vertex> s = *get_components(g).begin();

  assert(s.size());
  Vertex src = get_random_element(s, gen);
  s.erase(src);
  assert(s.size());
  Vertex dst = get_random_element(s, gen);

  return std::make_pair(src, dst);
}

template<typename G>
double
calculate_load(const G &g, int sc)
{
  acc::accumulator_set<double, acc::stats<acc::tag::mean> > load_acc;

  typename boost::graph_traits<G>::edge_iterator ei, ee;
  for (tie(ei, ee) = edges(g); ei != ee; ++ei)
    load_acc(double((sc - boost::get(boost::edge_subcarriers, g, *ei).size())) / sc);

  return acc::mean(load_acc);
}

#endif /* UTILS_HPP */
