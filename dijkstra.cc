#include "dijkstra.hpp"
#include "graph.hpp"
#include "utils.hpp"

#include <iterator>
#include <queue>
#include <map>
#include <utility>

using namespace std;

/**
 * Check whether there is a better or equal result in c2s, i.e. of a
 * lower or equal cost and with a SSC that includes "ssc".
 */
bool
has_better_or_equal(const C2S &c2s, int cost, const SSC &ssc)
{
  // We examine the results with the cost lower or equal to "cost".
  for (C2S::const_iterator i = c2s.begin();
       i != c2s.end() && i->first.first <= cost;
       ++i)
    // Check whether the result includes "ssc".
    if (includes(i->second, ssc))
      return true;

  return false;
}

/**
 * Check whether there is a worse or equal result in c2s, i.e. of a
 * larger or equal cost and with a SSC that is included in "ssc".
 */
bool
purge_worse(map<CEP, vertex> &q, C2S &c2s, int cost, const SSC &ssc)
{
  // We examine the results with the cost larger or equal to "cost".
  C2S::iterator i = c2s.begin();
  while(i != c2s.end() && i->first.first >= cost)
    // Check whether "ssc" includes the result.
    if (includes(ssc, i->second))
      {
        C2S::iterator j = i;
        ++i;
        q.erase(j->first);
        c2s.erase(j->first);
      }
    else
      ++i;
}

void
relaks(map<CEP, vertex> &q, C2S &c2s, const CEP &cep, vertex v, const SSC &ssc)
{
  // Check whether there is an SSC in c2s that includes c_ssc at the
  // same or lower cost then c_cep.  If yes, then just return.
  if (!has_better_or_equal(c2s, cep.first, ssc))
    {
      purge_worse(q, c2s, cep.first, ssc);

      // Since there was no better result, we use the candidate one.
      q[cep] = v;
      c2s[cep] = ssc;
    }
}

V2C2S
dijkstra(const graph &g, vertex src, vertex dst, int p, const SSC &src_ssc)
{
  V2C2S r;

  // The null edge.
  const edge &ne = *(boost::edges(g).second);

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
  map<CEP, vertex> q;

  // We reach vertex src with cost 0 along the null edge.
  q[make_pair(0, ne)] = src;

  while(!q.empty())
    {
      map<CEP, vertex>::iterator i = q.begin();
      CEP cep = i->first;
      vertex v = i->second;
      int c = cep.first;
      edge e = cep.second;
      q.erase(i);

      // C2S for node v.
      const C2S &c2s = r[v];

      // The CEP that we process in this loop has to be in the C2S.
      C2S::const_iterator j = c2s.find(cep);
      assert(j != c2s.end());

      // Stop searching if we have reached the destination node.
      if (v == dst)
        break;

      // These subcarriers are now available at node v for further
      // search.  There might be other subcarriers available in the
      // c2s, but we care only about the one that we got with edge e
      // at cost c.
      const SSC &v_ssc = j->second;

      // Itereate over the out edges of the vertex.
      boost::graph_traits<graph>::out_edge_iterator ei, eei;
      for(boost::tie(ei, eei) = boost::out_edges(v, g); ei != eei; ++ei)
      	{
          // The edge that we examine in this iteration.
	  const edge &e = *ei;
          // The subcarriers available on the edge.
	  const SSC &l_ssc = boost::get(boost::edge_subcarriers, g, e);

	  // Candidate SSC.
	  SSC c_ssc = exclude(intersection(v_ssc, l_ssc), p);

          if (!c_ssc.empty())
            {
              // The cost of the edge.
              int ec = boost::get(boost::edge_weight, g, e);
              // Candidate cost.
              int new_cost = c + ec;
              // Candidate CEP.
              CEP c_cep = make_pair(new_cost, e);
              // The target vertex of the edge.
              vertex t = boost::target(e, g);

              relaks(q, r[t], c_cep, t, c_ssc);
            }
      	}
    }

  return r;
}

path
shortest_path(const graph &g, const V2C2S &r, vertex src, vertex dst)
{
  path p;

  // Find the path from the back.  This is the current node.
  V2C2S::const_iterator dst_i = r.find(dst);
      
  // Make sure there is some data for node dst in r.
  if (dst_i != r.end())
    {
      // The C2S of the destination node.
      const C2S &dst_c2s = dst_i->second;

      if (!dst_c2s.empty())
	{
	  // For dst, the first element in C2S is the best result.
	  C2S::const_iterator bri = dst_c2s.begin();

	  // This is the cost of the whole path.
	  int c = bri->first.first;

	  // This is the path SSC.
	  const SSC &ssc = bri->second;
	  // And we remember this as the final result.
	  p.second = ssc;

	  // We start with the destination node.
	  vertex crt = dst;
	  while(crt != src)
	    {
	      V2C2S::const_iterator i = r.find(crt);
	      const C2S &c2s = i->second;

	      // We look for the solution that costs c and that
	      // contains SSC.
	      C2S::const_iterator j = c2s.begin();
	      for(; j->first.first <= c; ++j)
		if (j->first.first == c && includes(j->second, ssc))
		  break;

	      // The found solution must be of cost c.
	      assert(j->first.first == c);

	      const edge &e = j->first.second;
	      p.first.push_front(e);
	      c -= boost::get(boost::edge_weight, g, e);
	      assert(crt == boost::target(e, g));

	      // This is the new node to examine.
	      crt = source(e, g);
	    }

	  assert(c == 0);
	}
    }

  return p;
}

void
set_up_path(graph &g, const path &p, int sc)
{
  const list<edge> &l = p.first;
  // This is the largest set that can support at least sc subcarriers.
  SSC p_ssc = p.second;

  // We select the first sc subcarriers from sc.
  assert(p_ssc.size() >= sc);
  SSC::iterator ssc_i = p_ssc.begin();
  advance(ssc_i, sc);
  p_ssc.erase(ssc_i, p_ssc.end());
  p_ssc = exclude(p_ssc, sc);
  assert(!p_ssc.empty());

  // Iterate over the edges of the path.
  for(const edge &e: l)
    {
      SSC &e_ssc = boost::get(boost::edge_subcarriers, g, e);
      // Make sure that the edge has the required subcarriers.
      assert(includes(e_ssc, p_ssc));

      // Remove the p_ssc subcarriers from e_ssc.
      exclude(e_ssc, p_ssc);
    }
}
