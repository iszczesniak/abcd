//=======================================================================
// Copyright 2015 by Ireneusz Szcześniak
// Authors: Ireneusz Szcześniak
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#ifndef BOOST_GRAPH_ED_KSP
#define BOOST_GRAPH_ED_KSP

#include <list>
#include <map>
#include <set>
#include <vector>

#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/filtered_graph.hpp>
#include <boost/graph/visitors.hpp>
#include <boost/property_map/vector_property_map.hpp>
#include <boost/utility/value_init.hpp>
 
namespace boost {

  // Exclude edge filter
  template <typename Graph>
  struct edksp_filter
  {
    typedef typename Graph::edge_descriptor edge_descriptor;
    typedef typename std::set<edge_descriptor> edge_set;

    // The filter must be default-constructible, so it is.
    edksp_filter(): m_excluded() {};
    
    edksp_filter(const edge_set *excluded): m_excluded(excluded) {};

    inline bool operator()(const edge_descriptor &e) const
    {
      return m_excluded->count(e) == 0;
    }

    const edge_set *m_excluded;
  };

  template <typename Graph, typename Weight>
  std::multimap<typename Weight::value_type,
                std::list<typename Graph::edge_descriptor>>
  edge_disjoint_ksp(const Graph& g,
         typename graph_traits<Graph>::vertex_descriptor s,
         typename graph_traits<Graph>::vertex_descriptor t,
         Weight wm)
  {
    typedef typename graph_traits<Graph>::vertex_descriptor vertex_descriptor;
    typedef typename graph_traits<Graph>::edge_descriptor edge_descriptor;
    typedef typename std::list<typename Graph::edge_descriptor> path_type;
    typedef typename Weight::value_type weight_type;

    // The result.
    std::multimap<weight_type, path_type> result;

    // The set of excluded edges.
    std::set<edge_descriptor> excluded;
    // The filter for excluding edges.
    edksp_filter<Graph> f(&excluded);
    // The filtered graph type.
    typedef boost::filtered_graph<Graph, edksp_filter<Graph> > fg_type;
    // The filtered graph.
    fg_type fg(g, f);

    // In each iteration, we try to find a shortest path.
    do
      {
        boost::vector_property_map<edge_descriptor> pred(num_vertices(g));

        boost::dijkstra_shortest_paths
          (fg, s,
           visitor(make_dijkstra_visitor(record_edge_predecessors(pred, on_edge_relaxed()))));

        // Break the loop if no solution was found.
        if (pred[t] == edge_descriptor())
          break;

        // The cost of the shortest path.
        value_initialized<weight_type> cost;
        // The path found.
        path_type path;

        // Trace the solution to the source.
        vertex_descriptor c = t;
        while (c != s)
          {
            const edge_descriptor &e = pred[c];
            // Build the path.
            path.push_front(e);
            // Exclude the edge, so that it's not used in the next
            // shortest paths.
            excluded.insert(e);
            // Calculate the cost of the path.
            cost += get(wm, e);
            // Find the predecessing vertex.
            c = source(e, g);
          }
        
        result.insert(make_pair(cost, path));

      } while(true);
      
    return result;
  }

  template <typename Graph>
  std::multimap<typename property_map<Graph, edge_weight_t>::value_type,
                std::list<typename Graph::edge_descriptor>>
  edge_disjoint_ksp(Graph& g,
         typename graph_traits<Graph>::vertex_descriptor s,
         typename graph_traits<Graph>::vertex_descriptor t)
  {
    return edge_disjoint_ksp(g, s, t, get(edge_weight_t(), g));
  }

} // boost

#endif /* BOOST_GRAPH_ED_KSP */
