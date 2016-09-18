#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "graph.hpp"

#include <boost/optional.hpp>

#include <utility>

// The type of the connection.  It can establish, reconfigure and tear
// down a connection, but it doesn't report the statistics.
class connection
{
public:
  connection(graph &g);
  ~connection();

  const demand &
  get_demand() const;

  bool
  is_established() const;

  // Establish the connection for the given demand.  True if
  // successful.  If unsuccessful, the state of the object doesn't
  // change.
  bool
  establish(const demand &d);

  // Return the length of the connection.  The connection must be
  // established.
  int
  get_len() const;

  // Return the number of links of the connection.  The connection
  // must be established.
  int
  get_nol() const;

  // Return the number of subscribers of the connection.  The
  // connection must be established.
  int
  get_nsc() const;

  void
  tear_down();

private:
  graph &m_g;
  demand m_d;
  boost::optional<sscpath> m_p;

  int m_id;

  static int counter;
};

#endif /* CONNECTION_HPP */
