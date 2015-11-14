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
    typedef typename graph_traits<Graph>::edge_descriptor edge_descriptor;
    typedef typename graph_traits<Graph>::vertex_descriptor vertex_descriptor;
    typedef typename Weight::value_type weight_type;
    typedef typename std::list<typename Graph::edge_descriptor> path_type;

    // The result.
    std::multimap<weight_type, path_type> result;


      
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
