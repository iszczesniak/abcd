#include "routing.hpp"

static routing::routing_type rt = routing::cdijkstra;

routing::routing_type &
routing::get()
{
  return routing;
}

sscpath
routing::set_up(const graph &g, const demand &d)
{
  sscpath result;
  
  assert(d.first.first != d.first.second);

  switch (type):
    == cdijkstra)
    result = cdijkstra(g, d);
  else if (type == ed_ksp)
    result = ed_ksp(g, d);
  else
    abort();
  
  return result;
}
  
sscpath
routing::cdijkstra_f(const graph &g, const demand &d)
{
  sscpath 

    // We allow to allocate the signal on any of the subcarriers.
    V2C2S r = dijkstra::search(g, d);
  p.second = dijkstra::trace(g, r, d);
  result.first = !p.second.first.empty();
  result.second = p.second.first.size();

    if (result.first)
      dijkstra::set_up_path(g, p.second);

    p.first = result.first;
  }
}

sscpath
routing::ed_ksp_f(const graph &g, const demand &d)
{
}
