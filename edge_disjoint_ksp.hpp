//=======================================================================
// Copyright 2015 by Ireneusz Szcześniak
// Authors: Ireneusz Szcześniak
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

//=======================================================================
// Function edge_disjoint_ksp finds the k-shortest paths which are
// edge-disjoint, i.e. the paths do not have common edges.  The graph
// can have parallel edges.  The algorithm employs the Dijkstra
// algorithm to find the shortest path.  Next the edges of the found
// path are disabled, and Dijkstra finds the second shortest path.
// This process is repeated until k paths are found or there are no
// more paths to be found.
// =======================================================================

#ifndef BOOST_GRAPH_EDGE_DISJOINT_KSP
#define BOOST_GRAPH_EDGE_DISJOINT_KSP

#include <list>
#include <set>

#include <boost/graph/filtered_graph.hpp>
#include <boost/optional.hpp>

#include "custom_dijkstra_call.hpp"

namespace boost {

  //========================================================================
  // The implementation of the k-shortest paths algorithm.
  //========================================================================

  template <typename Graph, typename WeightMap, typename IndexMap>
  std::list<std::pair<typename WeightMap::value_type,
                      std::list<typename Graph::edge_descriptor>>>
  edge_disjoint_ksp(const Graph& g,
                    typename Graph::vertex_descriptor s,
                    typename Graph::vertex_descriptor t,
                    WeightMap wm, IndexMap im,
                    optional<unsigned> K)
  {
    typedef typename Graph::vertex_descriptor vertex_descriptor;
    typedef typename Graph::edge_descriptor edge_descriptor;
    typedef typename WeightMap::value_type weight_type;
    typedef std::set<edge_descriptor> es_type;
    typedef std::list<edge_descriptor> path_type;
    typedef std::pair<weight_type, path_type> kr_type;
    typedef filtered_graph<Graph, is_not_in_subset<es_type>> fg_type;

    // The result.
    std::list<std::pair<weight_type, path_type>> result;

    // The set of excluded edges.
    es_type excluded;
    // The edge predicate.
    is_not_in_subset<es_type> ep(excluded);
    // The filtered graph.
    fg_type fg(g, ep);

    // In each iteration, we try to find a shortest path.
    do
      {
        // If required, stop at the K-th shortest path.
        if (K && result.size() == K.get())
          break;
          
        // This is the optional k-th result.
        optional<kr_type> okr = custom_dijkstra_call(fg, s, t, wm, im);

        if (!okr)
          break;

        // This is the k-th result.
        const kr_type &kr = okr.get();
        
        // Exclude the edges from the k-th result found.
        for(auto const &e: kr.second)
          excluded.insert(e);

        result.push_back(kr);

      } while(true);
      
    return result;
  }

  //========================================================================
  // A helper function to figure out the default weight map of the graph.
  //========================================================================

  template <typename Graph>
  std::list<std::pair<typename property_map<Graph, edge_weight_t>::value_type,
                      std::list<typename Graph::edge_descriptor>>>
  edge_disjoint_ksp(const Graph& g,
                    typename Graph::vertex_descriptor s,
                    typename Graph::vertex_descriptor t,
                    optional<unsigned> K = optional<unsigned>())
  {
    return edge_disjoint_ksp(g, s, t, get(edge_weight_t(), g),
                             get(vertex_index_t(), g), K);
  }

} // boost

#endif /* BOOST_GRAPH_EDGE_DISJOINT_KSP */
