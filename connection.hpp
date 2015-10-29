#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "graph.hpp"

#include <utility>

// The type of the connection.
class connection
{
public:
  // complete - search for a completely new connection between the new
  // source node and the destination without the continuity
  // constraint.

  // incremental - establish a bridging connection between the old
  // source node and the new source node.

  // curtailing - find the shortest paths from the new source node to
  // all the nodes on the already established path and chose the best.

  enum reconf_t {complete, incremental, curtailing};

private:
  // This is the sscpath with status.
  typedef std::pair<bool, sscpath> sscpathws;

  graph &g;
  demand d;
  sscpathws p;

public:
  connection(graph &g);
  ~connection();

  const demand &
  get_demand() const;

  bool
  is_established() const;

  // Set up the connection:
  // @return .first - true if successfull
  // @return .second - the number of links of the established connection
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
  reconfigure_complete(vertex new_Src);

  std::pair<bool, int>
  reconfigure_incremental(vertex new_Src);

  std::pair<bool, int>
  reconfigure_curtailing_worker(vertex new_Src);

  std::pair<bool, int>
  reconfigure_curtailing(vertex new_Src);

  // The parameter that tells how to reconfigure connections.
  static reconf_t reconf;
};

#endif /* CONNECTION_HPP */
