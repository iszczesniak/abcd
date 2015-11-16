#ifndef BOOST_GRAPH_ED_KSP
#define BOOST_GRAPH_ED_KSP

#include <list>
#include <map>
#include <set>
#include <vector>

#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/filtered_graph.hpp>
#include <boost/property_map/property_map.hpp>

namespace boost {

  // Exclude edge filter
  template <typename Graph>
  struct edksp_filter
  {
    typedef typename Graph::edge_descriptor edge_descriptor;
    typedef typename std::set<edge_descriptor> edge_set;

    // The filter must be default-constructible, so it is.
    edksp_filter() {};

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
        std::vector<weight_type> dist(num_vertices(g));
        std::vector<vertex_descriptor> pred(num_vertices(g));

        boost::dijkstra_shortest_paths
          (fg, s, boost::predecessor_map(&pred[0]).distance_map(&dist[0]));

        // Break the loop if no solution was found.
        if (pred[t] == t)
          break;

        // The cost of the shortest path.
        weight_type cost = dist[t];
        // The path found.
        path_type path;

        // Trace the solution to the source.
        for (vertex_descriptor c = t; pred[c] != c; c = pred[c])
          {
            vertex_descriptor p = pred[c];
            weight_type ec = dist[c] - dist[p];

            // We're looking for an edge of weight ec.
            typename boost::graph_traits<Graph>::out_edge_iterator ei, eei;
            for(boost::tie(ei, eei) = boost::out_edges(p, g); ei != eei; ++ei)
            {
              // The edge that we examine in this iteration.
              const edge_descriptor &e = *ei;

              // The weight of the edge.
              if (ec == boost::get(boost::edge_weight, g, e))
                {
                  path.push_front(e);
                  excluded.insert(e);
                  break;
                }
            }

            // Make sure we found an edge of weight ec.
            assert(ei != eei);
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
