#include "dijkstra.hpp"
#include <queue>
#include <utility>
#include <boost/iterator/counting_iterator.hpp>

using namespace boost;
using namespace std;

MSCPI
dijkstra(const Graph &g, Vertex src, Vertex dst, int p, const SSC &ssc)
{
  MSCPI r;

  // The null edge.
  Edge ne = *(edges(g).second);

  // We put here the information that allows us to process the source
  // node in the loop below.  We say that we reach the source node
  // with cost 0 on the subcarriers passed in the ssc argument along
  // the null edge.  The null edge signals the beginning of the path.
  r[src].insert(CPI(0, PI(ne, ssc)));

  // The priority queue of the pairs.  In pair pqe the first element
  // (pqe.first) tells the cost of reaching the vertex
  // (pqe.second.first) along the edge (pqe.second.second).
  //
  // We need to know not only the vertex, but the edge too, because we
  // allow for multigraphs (i.e. with parallel edges), and so we need
  // to know what edge was used to reach the vertex.
  // 
  // We could pass only the edge and figure out the vertex from the
  // edge, but there is one special case that prevents us from doing
  // that: the source node, for which the null edge is used.
  // Furthermore, figuring out the end node might be problematic for
  // undirected graphs.
  priority_queue<pair<pair<int, Edge>, Vertex> > q;

  // We reach vertex src with cost 0 along the null edge.
  q.push(make_pair(0, make_pair(src, ne)));

  while(!q.empty())
    {
      // Here we process vertex v.  
      pair<pair<int, Edge>, Vertex> pqe = q.top();
      q.pop();
      int c = pqe.first.first;
      Edge e = pqe.first.second;
      Vertex v = pqe.second;

      // The SCPI for node v in r.
      SCPI &scpi = r[v];

      // Security check: make sure there is the info in r on reaching
      // vertex v along edge e with cost c.  Is there a chance that
      // this info is missing?  For instance, a better result was
      // found before and was introduced there?
      //assert(includes(scpi, cost, edge));
      
      // We reached vertex v with cost c.  Now we should know with
      // what subcarriers we reached this node.

      // Itereate over the out edges of the vertex.
      //      graph_traits<Graph>::out_edge_iterator ei, eei;
      //      for(tie(ei, eei) = out_edges(v, g); ei != eei; ++ei)
      //	{
      //	}
    }

  return r;
}
