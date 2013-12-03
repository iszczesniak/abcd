#include "dijkstra.hpp"
#include "graph.hpp"
#include "utils.hpp"

#include <iterator>
#include <queue>
#include <map>
#include <utility>

using namespace std;

typedef map<CEP, vertex> pqueue;

/**
 * Check whether there is a better or equal result in c2s than the new
 * result, i.e. of a lower or equal cost and with a SSC that includes
 * "ssc".
 */
bool
has_better_or_equal(const C2S &c2s, const COST &cost, const SSC &ssc)
{
  // We examine the existing results with the cost lower or equal to
  // "cost".
  for (auto &e: c2s)
    // Stop searching when we reach a result with the cost higher than
    // cost.
    if (e.first.first > cost)
      break;
    else
      // Check whether the existing result includes "ssc".
      if (includes(e.second, ssc))
        return true;

  return false;
}

/**
 * Check whether there is a worse or equal result in c2s, i.e. of a
 * larger or equal cost and with a SSC that is included in "ssc".
 */
void
purge_worse(pqueue &q, C2S &c2s, const COST &cost, const SSC &ssc)
{
  // We examine existing results with the cost larger or equal to "cost".
  C2S::iterator i = c2s.begin();
  while(i != c2s.end() && i->first.first >= cost)
    {
      SSSC &sssc = i->second;

      SSSC::iterator j = sssc.begin();
      while(j != sssc.end())
        // Check whether "ssc" includes an existing result.
        if (includes(ssc, *j))
          {
            SSSC::iterator j2 = j;
            ++j;
            sssc.erase(j2);
          }
        else
          ++j;

      // Discard the CEP for which SSSC is empty.
      if (sssc.empty())
        {
          C2S::iterator i2 = i;
          ++i;
          q.erase(i2->first);
          c2s.erase(i2);
        }
      else
        ++i;
    }
}

void
relaks(pqueue &q, C2S &c2s, const CEP &cep, vertex v, const SSC &ssc)
{
  // Check whether there is an SSC in c2s that includes c_ssc at the
  // same or lower cost then c_cep.  If yes, then we can ignore this
  // new result.
  if (!has_better_or_equal(c2s, cep.first, ssc))
    {
      // There are no better or equal results than the new result.
      // There might be worse results, and so we need to remove them.
      purge_worse(q, c2s, cep.first, ssc);

      // Since there was no better or equal result, we use the new
      // result.
      q[cep] = v;
      c2s[cep].insert(ssc);
    }
}

void
relaks(pqueue &q, C2S &c2s, const CEP &cep, vertex v, const SSSC &sssc)
{
  for(auto &ssc: sssc)
    relaks(q, c2s, cep, v, ssc);
}

V2C2S
dijkstra(const graph &g, const demand &d)
{
  SSC ssc;
  vertex src = d.first.first;

  // Itereate over the out edges of the vertex.
  boost::graph_traits<graph>::out_edge_iterator ei, eei;
  for(boost::tie(ei, eei) = boost::out_edges(src, g); ei != eei; ++ei)
    {
      // The edge that we examine in this iteration.
      const edge &e = *ei;
      // The subcarriers available on the edge.
      const SSC &e_ssc = boost::get(boost::edge_ssc, g, e);

      // Add e_ssc to ssc.
      include(ssc, e_ssc);
    }

  return dijkstra(g, d, ssc);
}

V2C2S
dijkstra(const graph &g, const demand &d, const SSC &src_ssc)
{
  V2C2S r;

  vertex src = d.first.first;
  vertex dst = d.first.second;
  int nsc = d.second;

  // The null edge.
  const edge &ne = *(boost::edges(g).second);

  // We have to filter ssc to exclude subcarriers that can't support
  // the signal with p subcarriers.
  SSC src_ssc_nsc = exclude(src_ssc, nsc);

  if (!src_ssc_nsc.empty())
    {
      // We put here the information that allows us to process the
      // source node in the loop below.  We say that we reach the
      // source node with cost 0 on the subcarriers passed in the ssc
      // argument along the null edge.  The null edge signals the
      // beginning of the path.
      r[src][CEP(COST(0, 0), ne)].insert(src_ssc_nsc);

      // The following map implements the priority queue.  The key is
      // a CEP, and the value is the vertex we are reaching.  The maps
      // works as the priority queue since the first element in the
      // key is the cost, and since the map sorts its elements in the
      // ascending order.  The value is the vertex.  The value could
      // be null as well, but we want to use CEP as a key, and need to
      // store the vertex as well.
      //
      // We need to know not only the vertex, but the edge too,
      // because we allow for multigraphs (i.e. with parallel edges),
      // and so we need to know what edge was used to reach the
      // vertex.
      // 
      // We could pass only the edge and figure out the vertex from
      // the edge, but there is one special case that prevents us from
      // doing that: the source node, for which the null edge is used.
      // Furthermore, figuring out the end node might be problematic
      // for undirected graphs.
      pqueue q;

      // We reach vertex src with null cost along the null edge.
      q[make_pair(COST(0, 0), ne)] = src;

      while(!q.empty())
        {
          pqueue::iterator i = q.begin();
          CEP cep = i->first;
          vertex v = i->second;
          int c = cep.first.first;
          int h = cep.first.second;
          edge e = cep.second;
          q.erase(i);

          // Stop searching when we reach the destination node.
          if (v == dst)
            break;

          // C2S for node v.
          const C2S &c2s = r[v];

          // The CEP that we process in this loop has to be in the
          // C2S.
          C2S::const_iterator j = c2s.find(cep);
          assert(j != c2s.end());

          // This SSSC is now available at node v for further search.
          // There might be other subcarriers available in the c2s,
          // but we care only about the one that we got with edge e at
          // cost c.
          const SSSC &v_sssc = j->second;

          // Itereate over the out edges of the vertex.
          boost::graph_traits<graph>::out_edge_iterator ei, eei;
          for(boost::tie(ei, eei) = boost::out_edges(v, g); ei != eei; ++ei)
            {
              // The edge that we examine in this iteration.
              const edge &e = *ei;
              // The subcarriers available on the edge.
              const SSC &e_ssc = boost::get(boost::edge_ssc, g, e);
              // Candidate SSC: the ssc available at node v that can
              // be carried by edge e, and that has at least nsc
              // contiguous subcarriers.
              SSSC c_sssc = exclude(intersection(v_sssc, e_ssc), nsc);

              if (!c_sssc.empty())
                {
                  // The cost of the edge.
                  int ec = boost::get(boost::edge_weight, g, e);
                  // Candidate cost.
                  int new_c = c + ec;
                  // Candidate number of hops.
                  int new_h = h + 1;
                  // Candidate COST.
                  COST new_cost = COST(new_c, new_h);
                  // Candidate CEP.
                  CEP c_cep = make_pair(new_cost, e);
                  // The target vertex of the edge.
                  vertex t = boost::target(e, g);

                  relaks(q, r[t], c_cep, t, c_sssc);
                }
            }
        }
    }

  return r;
}

sscpath
shortest_path(const graph &g, const V2C2S &r, const demand &d)
{
  sscpath p;

  vertex src = d.first.first;
  vertex dst = d.first.second;
  int nsc = d.second;

  // Find the path from the back.  This is the current node.
  V2C2S::const_iterator dst_i = r.find(dst);

  // Make sure there is some data for node dst in r.
  if (dst_i != r.end())
    {
      // The C2S of the destination node.
      const C2S &dst_c2s = dst_i->second;

      if (!dst_c2s.empty())
        {
          // For dst, the first element in C2S is the best result,
          // i.e. of the lowest cost.
          C2S::const_iterator bri = dst_c2s.begin();

          // This is the cost of the whole path.
          COST c = bri->first.first;

          // This is the path SSC.
          assert(!(bri->second.empty()));
          const SSC &p_ssc = *(bri->second.begin());
          // And we remember this as the final result.
          p.second = p_ssc;

          // We start with the destination node.
          vertex crt = dst;
          while(crt != src)
            {
              V2C2S::const_iterator i = r.find(crt);
              assert(i != r.end());
              const C2S &c2s = i->second;

              // We look for the solution that costs c and that
              // contains SSC.
              C2S::const_iterator j;
              for(j = c2s.begin(); j != c2s.end(); ++j)
                if (j->first.first == c && includes(j->second, p_ssc))
                  break;

              // Make sure we found the right CEP.
              assert(j != c2s.end());

              const edge &e = j->first.second;
              p.first.push_front(e);
              c.first -= boost::get(boost::edge_weight, g, e);
              --c.second;
              assert(crt == boost::target(e, g));

              crt = boost::source(e, g);
            }

          // The cost at the source node, that we should have reached
          // by now, should be 0.
          assert(c.first == 0);
          assert(c.second == 0);

          // This is the largest set that can support the demand.
          SSC &ssc = p.second;

          // We select the first sc subcarriers from ssc.
          assert(ssc.size() >= nsc);
          SSC::iterator ssc_i = ssc.begin();
          advance(ssc_i, nsc);
          ssc.erase(ssc_i, ssc.end());
          ssc = exclude(ssc, nsc);
          assert(!ssc.empty());
        }
    }

  return p;
}

void
set_up_path(graph &g, const sscpath &p)
{
  const list<edge> &l = p.first;
  const SSC &p_ssc = p.second;

  // Iterate over the edges of the path.
  for(const edge &e: l)
    {
      SSC &e_ssc = boost::get(boost::edge_ssc, g, e);
      // Make sure that the edge has the required subcarriers.
      assert(includes(e_ssc, p_ssc));

      // Remove the p_ssc subcarriers from e_ssc.
      exclude(e_ssc, p_ssc);
    }
}

void
tear_down_path(graph &g, const sscpath &p)
{
  const list<edge> &l = p.first;
  const SSC &p_ssc = p.second;

  // Iterate over the edges of the path.
  for(const edge &e: l)
    {
      SSC &e_ssc = boost::get(boost::edge_ssc, g, e);

      // Make sure that the edge has these subcarriers taken.
      assert(excludes(e_ssc, p_ssc));

      // Put back the p_ssc subcarriers to e_ssc.
      include(e_ssc, p_ssc);
    }
}
