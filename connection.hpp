#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "graph.hpp"

// The type of the connection.
class connection
{
public:
  enum reconf_t {part, anew};

private:
  graph &g;
  demand d;
  sscpath p;

public:
  connection(graph &g);
  const demand &get_demand() const;
  bool is_established() const;
  bool set_up(const demand &d);
  bool reconfigure(vertex new_Src);
  void tear_down();
  static reconf_t &get_reconf();

private:
  bool reconfigure_part(vertex new_Src);  
  bool reconfigure_anew(vertex new_Src);

  // The parameter that tells how to reconfigure connections.
  static reconf_t reconf;
};

#endif /* CONNECTION_HPP */
