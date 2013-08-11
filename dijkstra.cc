#include "dijkstra.hpp"
#include <queue>
#include <utility>
#include <boost/iterator/counting_iterator.hpp>

using namespace boost;
using namespace std;

MSCPI
dijkstra(const Graph &g, Vertex src, Vertex dst, int p)
{
  MSCPI r;

  // The null edge iterator.
  graph_traits<const Graph>::edge_iterator nei = edges(g).second;

  {
    set<int> all(counting_iterator<int>(0),
                 counting_iterator<int>(10));

    r[src].insert(CPI(0, PI(*nei, all)));
  }

  // The priority queue of the pairs, where the first element in the
  // pair tells the cost of reaching vertex that is the second element
  // in the pair.
  priority_queue<pair<int, pair<Vertex, Edge> > > q;

  // We reach the source vertex with cost 0 and along the null edge.
  q.push(make_pair(0, make_pair(src, *nei)));

  while(!q.empty())
    {
      // Here we process vertex v.  
      pair<int, pair<Vertex, Edge> > pqe = q.top();
      q.pop();
      int c = pqe.first;
      Vertex v = pqe.second.first;
      Edge e = pqe.second.second;

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
