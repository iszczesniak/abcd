#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <list>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include <boost/graph/adjacency_list.hpp>

// The set of subcarriers.  It's used to describe the subcarriers that
// are all avaiable along a single path.
typedef std::set<int> SSC;

// The set of SSC.  It's used to describe the subcarriers that are
// available but along different paths.  Each SSC corresponds to a
// different path.
typedef std::set<SSC> SSSC;

namespace boost {
  // Describes the set of available subcarriers on an edge.
  enum edge_subcarriers_t {edge_subcarriers};

  BOOST_INSTALL_PROPERTY(edge, subcarriers);
}

/**
 * The type of the graph we use.
 */
typedef
boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
		      boost::property<boost::vertex_name_t, std::string>,
		      boost::property<boost::edge_weight_t, int, 
		      boost::property<boost::edge_subcarriers_t, SSC> > >
graph;

typedef boost::graph_traits<graph>::edge_descriptor edge;
typedef boost::graph_traits<graph>::vertex_descriptor vertex;

namespace std {

  /**
   * This is the operator needed by std::map when its key is the edge
   * type.
   */
  bool operator <(const edge &, const edge &);

  bool operator == (const edge &, const edge &);

}

// The cost and edge pair.  It is used to store the cost of reaching a
// node along the given edge.
typedef std::pair<int, edge> CEP;

// The mapping from CEP to SSSC.  This mapping tells what SSSC is
// available at a node provided a given CEP was used.
typedef std::map<CEP, SSSC> C2S;

// A map of vertexes to C2S.  It's used to store the complete
// information for a given vertex.
typedef std::map<vertex, C2S> V2C2S;

// The path definition.
typedef std::list<edge> path;

// The SSC path.
typedef std::pair<path, SSC> sscpath;

// Node pair.
typedef std::pair<vertex, vertex> npair;

// The demand type: npair and the number of subcarriers (nsc).
typedef std::pair<npair, int> demand;

// The type of the connection.
typedef std::pair<demand, sscpath> connection;

#endif /* GRAPH_HPP */
