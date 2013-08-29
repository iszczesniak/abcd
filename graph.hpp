#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <set>
#include <string>
#include <utility>
#include <vector>

#include <boost/graph/adjacency_list.hpp>

using namespace std;
using namespace boost;

enum edge_subcarriers_t {edge_subcarriers};

namespace boost {
  BOOST_INSTALL_PROPERTY(edge, subcarriers);
}

typedef
adjacency_list_traits<vecS, vecS, undirectedS>::vertex_descriptor
Vertex;

/**
 * The type of the graph we use.
 */
typedef
adjacency_list <vecS, vecS, undirectedS,
                property<vertex_name_t, string>,
                property<edge_weight_t, int, 
		property<edge_subcarriers_t, set<int> > > >
Graph;

typedef graph_traits<Graph>::edge_descriptor Edge;

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
typedef pair<int, Edge> CEP;

// The set of subcarriers.
typedef set<int> SSC;

// The mapping from CEP to SSC.  This mapping tells what SSC is
// available at a node provided a given CEP was used.
typedef map<CEP, SSC> C2S;

// A map of vertexes to C2S.  It's used to store the complete
// information on the
typedef map<Vertex, C2S> V2C2S;

#endif /* GRAPH_HPP */
