#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <list>
#include <map>
#include <set>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include <boost/graph/adjacency_list.hpp>

// The set of subcarriers.  It's used to describe available
// subcarriers, which are all avaiable along a single path.  Look at
// SSSC below.
typedef std::set<int> SSC;

// The set of SSC.  It's used to describe the subcarriers that are
// available but along different paths.  Each SSC corresponds to a
// different path.
typedef std::set<SSC> SSSC;

namespace boost {
  // Describes the set of available subcarriers on an edge.
  enum edge_ssc_t {edge_ssc};

  BOOST_INSTALL_PROPERTY(edge, ssc);

  // Describes the number of subcarriers on an edge.
  enum edge_nosc_t {edge_nosc};

  BOOST_INSTALL_PROPERTY(edge, nosc);
}

/**
 * The type of the graph we use.  The edge_ssc_t property describes
 * the subcarriers available, i.e. not already taken.
 */
typedef
boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
		      boost::property<boost::vertex_name_t, std::string>,
                      boost::property<boost::edge_weight_t, int,
		      boost::property<boost::edge_nosc_t, int,
                      boost::property<boost::edge_ssc_t, SSC> > > >
graph;

typedef graph::edge_descriptor edge;
typedef graph::vertex_descriptor vertex;

// The cost of reaching a vertex.  The first argument is the cost of a
// path, say the length, and the other is the number of hops to reach
// the vertex from the source.  We need the number of hops to choose
// the path with the least number of hops, otherwise we risk looping.
typedef std::pair<int, int> COST;

// The cost, edge and vertex tuple.  Tuple (c, e, v) stores cost c of
// reaching target vertex v along edge e.  You could get the target
// vertex for the given edge with target(e, g), but I'm not sure it's
// always gonna work for undirected edges.  Furthermore, we cannot
// have std::pair<COST, edge> only (i.e., without the vertex), because
// for undirected edges (c, v) and (cep, r) are the same, where r is
// the reverse edge of v.
typedef std::tuple<COST, edge, vertex> CEV;

// The mapping from CEV to SSSC.  This mapping tells what SSSC is
// available at a node provided a given CEV was used.
typedef std::map<CEV, SSSC> C2S;

// A map of vertexes to C2S.  It's used to store the complete
// information for a given vertex.
typedef std::map<vertex, C2S> V2C2S;

// The path.
typedef std::list<edge> path;

// The list of paths.
typedef std::list<path> plist;

// The SSC path.
typedef std::pair<path, SSC> sscpath;

// Node pair.
typedef std::pair<vertex, vertex> npair;

// The demand type: npair and the number of subcarriers (nsc).
typedef std::pair<npair, int> demand;

#endif /* GRAPH_HPP */
