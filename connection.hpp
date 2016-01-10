#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "graph.hpp"

#include <utility>

// The type of the connection.
class connection
{
  // This is the sscpath with status.
  typedef std::pair<bool, sscpath> sscpathws;

  graph &m_g;
  demand m_d;
  sscpathws m_p;

  int m_id;

  static int counter;

public:
  connection(graph &g);
  ~connection();

  const demand &
  get_demand() const;

  bool
  is_established() const;

  // Establish the connection for the given demand.  True is successfull.
  bool
  establish(const demand &d);

  // Return the length of the established connection.  The connection
  // must be established.
  int
  get_length() const;

  // Return the number of hops of the established connection.  The
  // connection must be established.
  int
  get_hops() const;

  // Return the number of subscribers of the established connection.
  // The connection must be established.
  int
  get_nsc() const;
  
  void
  tear_down();
};

#endif /* CONNECTION_HPP */
