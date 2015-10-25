#ifndef BOOST_GRAPH_ED_KSP
#define BOOST_GRAPH_ED_KSP

#include <map>
#include <list>

#include <boost/property_map/property_map.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>

namespace boost {

  template <typename Graph, typename Weight>
  std::multimap<typename Weight::value_type,
                std::list<typename Graph::edge_descriptor>>
  ed_ksp(const Graph& _g,
         typename graph_traits<Graph>::vertex_descriptor s,
         typename graph_traits<Graph>::vertex_descriptor t,
         Weight wm)
  {
    typedef typename graph_traits<Graph>::edge_descriptor edge_t;
    typedef typename graph_traits<Graph>::vertex_descriptor vertex_t;
    typedef typename Weight::value_type weight_t;
    typedef typename std::list<typename Graph::edge_descriptor> path_t;

    // Create a copy, because we'll modify it.
    Graph g = _g;

    // The result.
    std::multimap<weight_t, path_t> result;
    
    // Fill in the maps.
    std::map<edge_t, edge_t> e2e;
    std::map<edge_t, int> e2c;
    std::map<edge_t, int> e2r;
    std::map<edge_t, weight_t> e2w;

    // The set of edges.
    std::set<edge_t> soe;
    typename graph_traits<Graph>::edge_iterator ei, ee;
    for (tie(ei, ee) = edges(g); ei != ee; ++ei)
      soe.insert(*ei);

    for (const auto &e: soe)
      {
        // Reverse edge.
        edge_t r;
        // Status of the edge creation.
        bool success;
        tie(r, success) = add_edge(target(e, g), source(e, g), g);
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
    associative_property_map<std::map<edge_t, weight_t>> wgt(e2w);

    successive_shortest_path_nonnegative_weights(g, s, t,
                                                 capacity_map(cap).
                                                 residual_capacity_map(res).
                                                 reverse_edge_map(rev).
                                                 weight_map(wgt));

    typename graph_traits<Graph>::out_edge_iterator oi, oie;
    for (tie(oi, oie) = out_edges(s, g); oi != oie; ++oi)
      {
        edge_t e = *oi;

        if (get(cap, e) == 1 && get(res, e) == 0)
          {
            path_t p;
            weight_t w = weight_t();

            do
              {
                // Push the edge to the path.
                p.push_back(e);
                // Increase the cost.
                w += get(wgt, e);

                // Set the capacity to zero, so that the edge is not
                // used for a different path.
                put(cap, e, 0);

                // This is the next vertex of the path.
                vertex_t nv = target(e, g);

                // Break when we reach the destination node.
                if (nv == t)
                  break;

                typename graph_traits<Graph>::out_edge_iterator oj, oje;
                for (tie(oj, oje) = out_edges(nv, g); oj != oje; ++oj)
                  {
                    e = *oj;

                    if (get(cap, e) == 1 && get(res, e) == 0)
                      break;
                  }

                assert(oj != oje);
              } while(true);

            result.insert(make_pair(w, p));
          }
      }
      
      return result;
  }

  template <typename Graph>
  std::multimap<typename property_map<Graph, edge_weight_t>::value_type,
                std::list<typename Graph::edge_descriptor>>
  ed_ksp(Graph& g,
         typename graph_traits<Graph>::vertex_descriptor s,
         typename graph_traits<Graph>::vertex_descriptor t)
  {
    return ed_ksp(g, s, t, get(edge_weight_t(), g));
  }

} // boost

#endif /* BOOST_GRAPH_ED_KSP */
