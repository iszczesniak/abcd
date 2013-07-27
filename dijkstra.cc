#include "dijkstra.hpp"
#include <queue>

using namespace std;

VPM
dijkstra(const Graph &g, Vertex src, Vertex dst, int p)
{
  VPM r;

  priority_queue<pair<int, Vertex> > q;

  q.push(make_pair(0, src));

  while(!q.empty())
    {
      pair<int, Vertex> e = q.top();
      q.pop();
      int c = e.first;
      Vertex v = e.second;      

      // Itereate over the out edges of the vertex.
      graph_traits<Graph>::out_edge_iterator ei, eei;
      for(tie(ei, eei) = out_edges(v, g); ei != eei; ++ei)
	{
	}
    };

  return r;
}
