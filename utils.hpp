#ifndef UTILS_HPP
#define UTILS_HPP

#include "edge_probs.hpp"
#include "graph.hpp"
#include "poisson.hpp"
#include "polynomial.hpp"
#include "sparse_matrix.hpp"
#include "matrixes.hpp"

#include <boost/graph/graphviz.hpp>
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/regex.hpp>

#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <vector>

/**
 * Iterate over all elements of the matrix MNAME.  The element has
 * indexes INAME and JNAME, the reference to element is ENAME, and
 * the matrix type is MatrixType.
 */
#define FOREACH_MATRIX_ELEMENT(MNAME, INAME, JNAME, ENAME, MatrixType)    \
  for (MatrixType::const_iterator                                         \
         IIJS = MNAME.begin(); IIJS != MNAME.end(); ++IIJS)               \
    for(MatrixType::mapped_type::const_iterator                           \
          JIJS = IIJS->second.begin(); JIJS!= IIJS->second.end(); ++JIJS) \
      for(bool cont_ = true; cont_;)                                      \
        for(int INAME = IIJS->first; cont_;)                              \
          for(int JNAME = JIJS->first; cont_;)                            \
            for(const MatrixType::mapped_type::mapped_type                \
                  &ENAME = JIJS->second; cont_; cont_ = false)

/**
 * This function calculates the shortes paths and packet preferences.
 * The data for shortest paths are stored in vertex_distance and
 * vertex_predecessor.  The data for the packet preferences are stored
 * in the vertex_bundle property.
 */
void
complete_graph(Graph &g);

/**
 * This function returns the mapping between vertex names and
 * vertexes.
 */
map<string, Vertex>
get_vertex_names(const Graph &g);

/**
 * This function reads graphviz file from a file, not a stream as the
 * read_graphviz function does.
 *
 * @return true if successful, false otherwise
 */
bool
read_graphviz_filename(const char* file_name, Graph &g);

/*
 * @return true if successful, false otherwise
 */
bool
load_tm_file(const char *file_name, const map<string, Vertex> &vertex_names,
             fp_matrix &tm);

/*
 * @return true if successful, false otherwise
 */
bool
load_tm(istream &is, const map<string, Vertex> &vertex_names,
        fp_matrix &tm);

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
 * This is the << operator for a map.
 */
template <typename K, typename V>
ostream &operator << (ostream &os, const map<K, V> &v)
{
  for (typename map<K, V>::const_iterator i = v.begin(); i != v.end(); ++i)
    os << "(" << i->first << ", " << i->second << ")";

  return os;
}

template <typename T>
vector<T> &
operator += (vector<T> &v1, const vector<T> &v2)
{
  assert(v1.size() == v2.size());

  for(int i = 0; i < v1.size(); ++i)
    v1[i] += v2[i];

  return v1;
}

/**
 * Returns the sum of coefficients for tpoly.
 */
template <typename T>
T
sum(const tpoly<T> &p)
{
  T result = T();

  for(typename tpoly<T>::const_iterator i = p.begin(); i != p.end(); ++i)
    result += (i->second);

  return result;
}

/**
 * Returns the sum of all tpoly objects in the map.
 */
template <typename T>
tpoly<T>
sum(const map<Vertex, tpoly<T> > &c)
{
  tpoly<T> result;

  for(typename map<Vertex, tpoly<T> >::const_iterator
        i = c.begin(); i != c.end(); ++i)
    result += i->second;

  return result;
}

/**
 * Returns the distance of the shortest path
 * from node i to node j in graph g.
 */
int
get_distance(Vertex i, Vertex j, const Graph &g);

/**
 * Returns the number of wavelengths that leave node j.
 */
int
get_output_capacity(const Graph& g, Vertex j);

std::string
path_to_string(Vertex i, Vertex j, const Graph &g);

std::string
to_string(Edge e, const Graph &g);

/**
 * Prints the information on the network nodes.
 */
void
print_nn(const Graph &g, std::ostream &os);

/**
 * Prints the information on the network links.
 */
void
print_nl(const Graph &g, std::ostream &os);

/**
 * Prints the information on the demands.
 */
void
print_tm(const Graph &g, const fp_matrix &tm, std::ostream &os);

/**
 * Prints the information on the shortest paths.
 */
void
print_sp(const Graph &g, std::ostream &os);

/**
 * Prints the information on packet preferences.
 */
void
print_pp(const Graph &g, std::ostream &os);

/**
 * Prints the routing priorities.
 */
void
print_rp(const Graph &g, std::ostream &os);

/**
 * Prints the input.
 */
void
print_input(const Graph &g, const fp_matrix &tm, std::ostream &os);

/**
 * This function prints the node flows.
 */
void
print_nf(const pp_matrix &ppm, const pt_matrix &ptm,
         const Graph &g, std::ostream &os);

/**
 * This function prints the link loads.
 */
void
print_ll(const map<Edge, double> &ll,
         const Graph &g, std::ostream &os);

/**
 * This function prints the probabilities of routing packets.
 */
void
print_pr(const pp_matrix &ppm, const pt_matrix &ptm,
         const Graph &g, std::ostream &os);

/**
 * This function prints the packet presence matrix.
 */
void
print_ppm(const pp_matrix &ppm, const Graph &g, std::ostream &os);

/**
 * This function prints the packet trajectory matrix.
 */
void
print_ptm(const pt_matrix &ptm, const Graph &g, std::ostream &os);

/**
 * Prints the output.
 */
void
print_output(const pp_matrix &ppm, const pt_matrix &ptm,
             const Graph &g, std::ostream &os);

/**
 * Calculate the link load based on the packet trajectory matrix.
 */
void
calculate_ll(const pt_matrix &ptm, map<Edge, double> &ll, const Graph &g);

/**
 * Returns a dist_poly for packets that arrive at node j and that are
 * destined to node i.  If the flag admitted is true, we also consider
 * the just-admitted packets.
 *
 * If j == i, then you get data on packets that go to local drop.
 *
 * If j != i, then you get data on packets that ask routing.
 */
dist_poly
node_dist_poly(Vertex j, Vertex i, const pp_matrix &ppm, bool admitted);

/**
 * Returns a dist_poly for packets that traverse link e and that are
 * destined to node i.
 */
dist_poly
link_dist_poly(Edge e, Vertex i, const pt_matrix &ptm);

/**
 * Calculates the edge_probs_map for node j based on the ppm and ptm.
 */
edge_probs_map
calc_edge_probs_map(const Graph &g, Vertex j,
                    const pp_matrix &ppm, const pt_matrix &ptm);

/**
 * Returns the distribution with which packet were admitted at node j
 * that go to node i.
 */
distro
get_adm_distro(const pp_matrix &ppm, Vertex j, Vertex i);

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
 * Make sure that the graph has only one connected component, and that
 * perhaps there are some lonely vertexes.  Sort the list in the
 * decreasing order of the number of elements in the sets.
 */
list<set<Vertex> >
get_components(const Graph &g);

/**
 * There must be only one connected component.  There could be other
 * components, but they can't be connected (i.e. they are lone
 * vertexes).
 *
 * @return: true if only one component is connected, false otherwise.
 */
bool
check_components(const Graph &g);

#endif /* UTILS_HPP */
