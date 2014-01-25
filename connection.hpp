#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "graph.hpp"

#include <utility>

// The type of the connection.
class connection
{
public:
  // part - partial reconfiguration: establish a new connection
  // between base stations

  // anew - search for a completely new connection

  // retrace - try to find the shortest path, first from the last node
  // on the path, then from the previous to the last node, and so on.

  // retrace2 - find the shortest paths from the new source node to
  // all the nodes on the already established path and chose the best.

  enum reconf_t {part, anew, retrace, retrace2};

private:
  graph &g;
  demand d;
  sscpath p;

public:
  connection(graph &g);

  const demand &
  get_demand() const;

  bool
  is_established() const;

  std::pair<bool, int>
  set_up(const demand &d);

  std::pair<bool, int>
  reconfigure(vertex new_Src);

  void
  tear_down();

  static reconf_t &
  get_reconf();

private:
  std::pair<bool, int>
  reconfigure_part(vertex new_Src);

  std::pair<bool, int>
  reconfigure_anew(vertex new_Src);

  std::pair<bool, int>
  reconfigure_retrace(vertex new_Src);

  std::pair<bool, int>
  reconfigure_retrace2(vertex new_Src);

  // The parameter that tells how to reconfigure connections.
  static reconf_t reconf;
};

#endif /* CONNECTION_HPP */
