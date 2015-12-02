//=======================================================================
// Copyright 2015 by Ireneusz Szcześniak
// Authors: Ireneusz Szcześniak <www.irkos.org>
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

//=======================================================================
// This is the implementation of the Yen algorithm:
//
// Jin Y. Yen, Finding the k shortest loopless paths in a network,
// Management Science, vol. 17, no. 11, July 1971, pages 712-716
//
// But actually, I found the following explanation better:
//
// https://en.wikipedia.org/wiki/Yen%27s_algorithm
//=======================================================================

#ifndef BOOST_GRAPH_YEN_KSP
#define BOOST_GRAPH_YEN_KSP

#include <list>
#include <set>
#include <map>

#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/filtered_graph.hpp>
#include <boost/graph/visitors.hpp>
#include <boost/property_map/vector_property_map.hpp>
#include <boost/utility/value_init.hpp>
 
namespace boost {

  // This filter excludes the given vertexes.
  template <typename Graph>
  struct yenksp_vfilter
  {
    typedef typename Graph::vertex_descriptor vertex_descriptor;
    typedef typename std::set<vertex_descriptor> vertex_set;

    // The filter must be default-constructible, so it is.
    yenksp_vfilter(): m_excluded() {};
    
    yenksp_vfilter(const vertex_set *excluded): m_excluded(excluded) {};

    inline bool operator()(const vertex_descriptor &e) const
    {
      return m_excluded->count(e) == 0;
    }

    const vertex_set *m_excluded;
  };

  // This filter excludes the given edges.
  template <typename Graph>
  struct yenksp_efilter
  {
    typedef typename Graph::edge_descriptor edge_descriptor;
    typedef typename std::set<edge_descriptor> edge_set;

    // The filter must be default-constructible, so it is.
    yenksp_efilter(): m_excluded() {};
    
    yenksp_efilter(const edge_set *excluded): m_excluded(excluded) {};

    inline bool operator()(const edge_descriptor &e) const
    {
      return m_excluded->count(e) == 0;
    }

    const edge_set *m_excluded;
  };

  template <typename Graph, typename Weight>
  std::list<std::pair<typename Weight::value_type,
                      std::list<typename Graph::edge_descriptor>>>
  yen_ksp(const Graph& g,
          typename graph_traits<Graph>::vertex_descriptor s,
          typename graph_traits<Graph>::vertex_descriptor t,
          Weight wm, int K)
  {
    typedef typename graph_traits<Graph>::vertex_descriptor vertex_descriptor;
    typedef typename graph_traits<Graph>::edge_descriptor edge_descriptor;
    typedef typename std::list<typename Graph::edge_descriptor> path_type;
    typedef typename Weight::value_type weight_type;

    // The results.
    std::list<std::pair<weight_type, path_type>> A;
    // The tentative results.
    std::multimap<weight_type, path_type> B;

    // The set of excluded edges.
    std::set<edge_descriptor> exe;
    // The set of excluded vertexes.
    std::set<vertex_descriptor> exv;
   
    // The filter which excludes edges.
    yenksp_efilter<Graph> ef(&exe);
    // The filter which excludes vertexes.
    yenksp_vfilter<Graph> vf(&exv);

    // The filtered graph type.
    typedef boost::filtered_graph<Graph,
                                  yenksp_efilter<Graph>,
                                  yenksp_vfilter<Graph>> fg_type;
    // The filtered graph.
    fg_type fg(g, ef, vf);
      
    return A;
  }

  template <typename Graph>
  std::list<std::pair<typename property_map<Graph, edge_weight_t>::value_type,
                      std::list<typename Graph::edge_descriptor>>>
  yen_ksp(Graph& g,
          typename graph_traits<Graph>::vertex_descriptor s,
          typename graph_traits<Graph>::vertex_descriptor t,
          int K)
  {
    return yen_ksp(g, s, t, get(edge_weight_t(), g), K);
  }

} // boost

#endif /* BOOST_GRAPH_YEN_KSP */

/*

function YenKSP(Graph, source, sink, K):
   // Determine the shortest path from the source to the sink.
   A[0] = Dijkstra(Graph, source, sink);
   // Initialize the heap to store the potential kth shortest path.
   B = [];
   
   for k from 1 to K:
       // The spur node ranges from the first node to the next to last node in the previous k-shortest path.
       for i from 0 to size(A[k − 1]) − 1:
           
           // Spur node is retrieved from the previous k-shortest path, k − 1.
           spurNode = A[k-1].node(i);
           // The sequence of nodes from the source to the spur node of the previous k-shortest path.
           rootPath = A[k-1].nodes(0, i);
           
           for each path p in A:
               if rootPath == p.nodes(0, i):
                   // Remove the links that are part of the previous shortest paths which share the same root path.
                   remove p.edge(i, i + 1) from Graph;
           
           for each node rootPathNode in rootPath except spurNode:
               remove rootPathNode from Graph;
           
           // Calculate the spur path from the spur node to the sink.
           spurPath = Dijkstra(Graph, spurNode, sink);
           
           // Entire path is made up of the root path and spur path.
           totalPath = rootPath + spurPath;
           // Add the potential k-shortest path to the heap.
           B.append(totalPath);
           
           // Add back the edges and nodes that were removed from the graph.
           restore edges to Graph;
           restore nodes in rootPath to Graph;
                   
       if B is empty:
           // This handles the case of there being no spur paths, or no spur paths left.
           // This could happen if the spur paths have already been exhausted (added to A), 
           // or there are no spur paths at all - such as when both the source and sink vertices 
           // lie along a "dead end".
           break;
       // Sort the potential k-shortest paths by cost.
       B.sort();
       // Add the lowest cost path becomes the k-shortest path.
       A[k] = B[0];
       B.pop();
   
   return A;

*/
