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
#include <boost/optional.hpp>
#include <boost/range/adaptor/reversed.hpp>
#include <boost/utility/value_init.hpp>

#include "custom_dijkstra_call.hpp"
#include "exclude_filter.hpp"

namespace boost {

  template <typename Graph, typename WeightMap, typename IndexMap>
  std::list<std::pair<typename WeightMap::value_type,
                      std::list<typename Graph::edge_descriptor>>>
  yen_ksp(const Graph& g,
          typename Graph::vertex_descriptor s,
          typename Graph::vertex_descriptor t,
          WeightMap wm, IndexMap im, optional<unsigned> K)
  {
    typedef typename Graph::vertex_descriptor vertex_descriptor;
    typedef typename Graph::edge_descriptor edge_descriptor;
    typedef typename WeightMap::value_type weight_type;
    typedef exclude_filter<vertex_descriptor> evf_type;
    typedef exclude_filter<edge_descriptor> eef_type;
    typedef std::list<edge_descriptor> path_type;
    typedef std::pair<weight_type, path_type> kr_type;

    // The results.
    std::list<kr_type> A;
    // The tentative results.
    std::multimap<weight_type, path_type> B;

    // The set of excluded edges.
    std::set<edge_descriptor> exe;
    // The set of excluded vertexes.
    std::set<vertex_descriptor> exv;

    // The filter which excludes edges.
    eef_type ef(&exe);
    // The filter which excludes vertexes.
    evf_type vf(&exv);

    // The filtered graph type.
    typedef boost::filtered_graph<Graph, eef_type, evf_type> fg_type;

    // The filtered graph.
    fg_type fg(g, ef, vf);

    optional<kr_type> okr = custom_dijkstra_call(g, s, t, wm, im);

    if (okr)
      {
        A.push_back(okr.get());

        for (int k = 1; !K || k < K.get(); ++k)
          {
            // The previous shortest result.
            const kr_type &psr = A.back();
            const path_type &psp = psr.second;

            // Iterate over the edges of the previous shortest path.
            for(auto i = psp.begin(); i != psp.end(); ++i)
              {
                // The spur vertex.
                vertex_descriptor sv = source(*i, g);
                // The root path.
                path_type rp = path_type(psp.begin(), i);

                // Iterate over the previous shortest paths.
                for(const auto &kr: A)
                  {
                    const path_type &kp = kr.second;
                    typename path_type::const_iterator i1, i2;

                    // Iterate as long as possible, and as long as
                    // paths are equal.
                    for(tie(i1, i2) = std::make_pair(kp.begin(), rp.begin());
                        i1 != kp.end() && i2 != rp.end() && *i1 == *i2;
                        ++i1, ++i2);

                    // Make sure we didn't reach the end of kp.  If we
                    // did, there is no next edge in kp, which we
                    // could exclude.  Also, make sure we reached the
                    // end of rp, i.e., the kp begins with rp.
                    if (i1 != kp.end() && i2 == rp.end())
                      exe.insert(*i1);
                  }

                // Remove the vertexes that belong to the root path,
                // except the last vertex, i.e., the spur node.
                for (const auto &e: rp)
                  exv.insert(source(e, g));

                // Optional spur result.
                optional<kr_type> osr = custom_dijkstra_call(fg, sv, t, wm, im);

                if (osr)
                  {
                    // The tentative result.
                    kr_type tr = osr.get();
                    for(const auto &e: boost::adaptors::reverse(rp))
                      {
                        tr.second.push_front(e);
                        tr.first += get(wm, e);
                      }
                    B.insert(tr);
                  }

                // Clear the excluded edges and vertexes.
                exe.clear();
                exv.clear();
              }

            // Stop searching when there are no tentative paths.
            if (B.empty())
              break;

            // Take the shortest tentative path and make it the next
            // shortest path.
            A.push_back(*B.begin());
            B.erase(B.begin());
          }
      }

    return A;
  }

  template <typename Graph>
  std::list<std::pair<typename property_map<Graph, edge_weight_t>::value_type,
                      std::list<typename Graph::edge_descriptor>>>
  yen_ksp(Graph& g,
          typename Graph::vertex_descriptor s,
          typename Graph::vertex_descriptor t,
          optional<unsigned> K = optional<unsigned>())
  {
    return yen_ksp(g, s, t, get(edge_weight_t(), g),
                   get(vertex_index_t(), g), K);
  }

} // boost

#endif /* BOOST_GRAPH_YEN_KSP */
