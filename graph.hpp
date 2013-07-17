#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
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
                property<vertex_name_t, string,
                property<vertex_distance_t, vector<int>,
		property<vertex_predecessor_t, vector<Vertex> > > >,
                property<edge_subcarriers_t, vector<int> > >
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

#endif /* GRAPH_HPP */
