#include "dijkstra.hpp"
#include "graph.hpp"

#include <iterator>
#include <queue>
#include <utility>

using namespace boost;
using namespace std;

SSC
exclude(const SSC &ssc, int p)
{
  SSC result;

  return result;
}

V2C2S
dijkstra(const Graph &g, Vertex src, Vertex dst, int p, const SSC &src_ssc)
{
  V2C2S r;

  // The null edge.
  Edge ne = *(edges(g).second);

  // We put here the information that allows us to process the source
  // node in the loop below.  We say that we reach the source node
  // with cost 0 on the subcarriers passed in the ssc argument along
  // the null edge.  The null edge signals the beginning of the path.
  // We have to filter ssc to exclude subcarriers that can't support
  // the signal with p subcarriers.
  r[src][CEP(0, ne)] = exclude(src_ssc, p);

  // The following map implements the priority queue.  The key is a
  // CEP, and the value is the vertex we are reaching.  The maps works
  // as the priority queue since the first element in the key is the
  // cost, and since the map sorts its elements in the ascending
  // order.  The value is the vertex.  The value could be null as
  // well, but we want to use CEP as a key, and need to store the
  // vertex as well.
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
  map<CEP, Vertex> q;

  // We reach vertex src with cost 0 along the null edge.
  q[make_pair(0, ne)] = src;

  while(!q.empty())
    {
      map<CEP, Vertex>::iterator i = q.begin();
      CEP cep = i->first;
      // We have them at cost c and after crossing edge e.
      Vertex v = i->second;
      int c = cep.first;
      Edge e = cep.second;
      q.erase(i);

      // The C2S for node v.
      C2S &c2s = r[v];

      // The CEP that we process in this loop has to be in the C2S for
      // node v.
      C2S::const_iterator j = c2s.find(cep);
      assert(j != c2s.end());

      // These subcarriers are now available at node v for further
      // search.  There might be other subcarriers available in the
      // c2s, but we care only about the one that we got with edge e
      // at cost c.
      const SSC &v_ssc = j->second;

      // Itereate over the out edges of the vertex.
      graph_traits<Graph>::out_edge_iterator ei, eei;
      for(tie(ei, eei) = out_edges(v, g); ei != eei; ++ei)
      	{
	  Edge e = *ei;
	  
	  const SSC &l_ssc = get(edge_subcarriers, g, e);

	  // Candidate SSC.
	  SSC c_ssc;
	  // SSC set_intersection(v_ssc.begin(), v_ssc.end(),
	  // l_ssc.begin(), l_ssc.end(),
	  // inserter(c_ssc, c_ssc.begin()));
      	}
    }

  return r;
}
