#ifndef BOOST_GRAPH_ED_KSP
#define BOOST_GRAPH_ED_KSP

#include <map>
#include <list>

#include <boost/property_map/property_map.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>

namespace boost {

  template <typename Graph, typename Weight>
  std::list<std::list<typename Graph::edge_descriptor>>
  ed_ksp(Graph& g,
         typename graph_traits<Graph>::vertex_descriptor s,
         typename graph_traits<Graph>::vertex_descriptor t,
         Weight wm)
  {
    typedef typename graph_traits<Graph>::edge_descriptor edge_t;
    typedef typename Weight::value_type weight_t;
    typedef typename std::list<typename Graph::edge_descriptor> path_t;
      
    std::list<path_t> paths;
    
    // Fill in the maps.
    std::map<edge_t, edge_t> e2e;
    std::map<edge_t, int> e2c;
    std::map<edge_t, int> e2r;
    std::map<edge_t, weight_t> e2w;

    typename graph_traits<Graph>::edge_iterator ei, ee;
    for (tie(ei, ee) = edges(g); ei != ee; ++ei)
      {
        // Normal edge.
        const edge_t &e = *ei;
        // Reverse edge.
        edge_t r;
        // Status of the edge creation.
        bool success;
        tie(r, success) = boost::edge(target(e, g), source(e, g), g);
        assert(success);

        // Map the edge.
        e2e[e] = r;
      
        // The weight of the normal edge.
        int weight = get(wm, e);
      
        // Properties for the normal edge.
        e2w[e] = weight;
        e2c[e] = 1;
      
        // Properties for the reverse edge.
        e2w[r] = -weight;
        e2c[r] = 0;
      }

    associative_property_map<std::map<edge_t, edge_t>> rev(e2e);
    associative_property_map<std::map<edge_t, int>> cap(e2c);
    associative_property_map<std::map<edge_t, int>> res(e2r);
    associative_property_map<std::map<edge_t, int>> wgt(e2w);

    successive_shortest_path_nonnegative_weights(g, s, t,
                                                 capacity_map(cap).
                                                 residual_capacity_map(res).
                                                 reverse_edge_map(rev).
                                                 weight_map(wgt));

    typename graph_traits<Graph>::out_edge_iterator oi, oie;
    for (tie(oi, oie) = out_edges(s, g); oi != oie; ++oi)
      {
        const edge_t &e = *oi;

        if (get(cap, e) == 1 && get(res, e) == 0)
          {
            paths.push_back(path_t());
            path_t &p = paths.back();

            p.push_back(e);
            put(cap, e, 0);

            /*
            while (target(e, g) != t)
              {
                typename graph_traits<Graph>::out_edge_iterator oj, oje;
                for (tie(oj, oje) = out_edges(s, g); oj != oje; ++oj);
              }
            */
          }
      }
      
      return paths;
  }

  template <typename Graph>
  std::list<std::list<typename Graph::edge_descriptor>>
  ed_ksp(Graph& g,
         typename graph_traits<Graph>::vertex_descriptor s,
         typename graph_traits<Graph>::vertex_descriptor t)
  {
    return ed_ksp(g, s, t, get(edge_weight_t(), g));
  }

} // boost

#endif /* BOOST_GRAPH_ED_KSP */
