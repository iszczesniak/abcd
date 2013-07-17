#ifndef UTILS_HPP
#define UTILS_HPP

#include "graph.hpp"

#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>

#include <boost/graph/dijkstra_shortest_paths.hpp>

#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <vector>

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
      Vertex v = *vi;

      dijkstra_shortest_paths
        (g, v, predecessor_map(&pred[0]).distance_map(&dist[0]));

      put(vertex_distance, g, v, dist);
      put(vertex_predecessor, g, v, pred);
    }
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

#endif /* UTILS_HPP */
