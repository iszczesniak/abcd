#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <set>
#include <string>
#include <utility>
#include <vector>

#include <boost/graph/adjacency_list.hpp>

// The set of subcarriers.
typedef std::set<int> SSC;

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

typedef boost::graph_traits<graph>::edge_descriptor Edge;
typedef boost::graph_traits<graph>::vertex_descriptor Vertex;

namespace std {

  /**
   * This is the operator needed by std::map when its key is the Edge
   * type.
   */
  bool operator <(const Edge &, const Edge &);

  bool operator == (const Edge &, const Edge &);

}

// The cost and edge pair.  It is used to store the cost of reaching a
// node along the given edge.
typedef std::pair<int, Edge> CEP;

// The mapping from CEP to SSC.  This mapping tells what SSC is
// available at a node provided a given CEP was used.
typedef std::map<CEP, SSC> C2S;

// A map of vertexes to C2S.  It's used to store the complete
// information for a given vertex.
typedef std::map<Vertex, C2S> V2C2S;

// The path definition.
typedef std::pair<std::list<Edge>, SSC> Path;

#endif /* GRAPH_HPP */
