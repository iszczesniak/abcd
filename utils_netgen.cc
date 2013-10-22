#include "utils_netgen.hpp"

#include <iostream>
#include <iomanip>

using namespace std;

void
name_vertices(graph &g)
{
  int count = 1;

  int number = num_vertices(g);
  int width = int(log10(number)) + 1;

  BGL_FORALL_VERTICES(v, g, graph)
    {
      ostringstream out;
      out << "v" << setw(width) << setfill('0') << count++;
      boost::get(boost::vertex_name, g, v) = out.str();
    }
}

void
move(Vertex v, const graph &g, std::set<Vertex> &lonely,
     std::set<Vertex> &connected, std::set<Vertex> &saturated)
{
  lonely.erase(v);

  if (num_vertices(g) >= 3)
    connected.insert(v);
  else
    saturated.insert(v);
}

void
move_if_needed(Vertex v, const graph &g, std::set<Vertex> &connected,
               std::set<Vertex> &saturated)
{
  int n = boost::num_vertices(g);
  int od = boost::out_degree(v, g);

  // A node can have the out degree of at most (n - 1).
  assert(od < n);

  // A node is saturated if its out degree is (n - 1).
  if (od == n - 1)
    {
      connected.erase(v);
      saturated.insert(v);
    }
}
