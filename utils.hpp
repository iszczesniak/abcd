#ifndef UTILS_HPP
#define UTILS_HPP

#include "graph.hpp"

#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>

#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <vector>

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
