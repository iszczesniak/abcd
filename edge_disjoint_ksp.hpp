#ifndef EDGE_DISJOINT_KSP
#define EDGE_DISJOINT_KSP

template <typename G>
edge_disjoint_ksp(const G& g)
{
  // Property map.
  boost::property_map<G, boost::edge_weight_t>::type
    wm = get(boost::edge_weight_t(), g);

  // Fill in the maps.
  std::map<edge, edge> e2e;
  std::map<edge, int> e2c;
  std::map<edge, int> e2r;
  std::map<edge, int> e2w;

  boost::graph_traits<graph>::edge_iterator ei, ee;
  for (tie(ei, ee) = edges(g); ei != ee; ++ei)
    {
      // Normal edge.
      const edge &ne = *ei;
      // Reverse edge.
      edge re;
      // Status of the edge creation.
      bool success;
      boost::tie(re, success) =
        boost::edge(boost::target(ne, g), boost::source(ne, g), g);
      assert(success);

      // Map the edges to each other.
      e2e[ne] = re;
      
      // The weight of the normal edge.
      int weight = wm[ne];
      
      // Properties for the normal edge.
      e2w[ne] = weight;
      e2c[ne] = 1;
      
      // Properties for the reverse edge.
      e2w[re] = -weight;
      e2c[re] = 0;
    }

  // Set the source and destination.
  vertex s = 0, t = 1;

  boost::associative_property_map<std::map<edge, edge>> rev(e2e);
  boost::associative_property_map<std::map<edge, int>> cap(e2c);
  boost::associative_property_map<std::map<edge, int>> res(e2r);
  boost::associative_property_map<std::map<edge, int>> wgt(e2w);
  boost::successive_shortest_path_nonnegative_weights(g, s, t,
                                                      capacity_map(cap).
                                                      residual_capacity_map(res).
                                                      reverse_edge_map(rev).
                                                      weight_map(wgt));
}

#endif /* EDGE_DISJOINT_KSP */
