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
  // The reconfiguration types:
  // complete - route the connection anew
  // our - our method of reconfiguration
  enum class re_t {none, complete, proposed};

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

  // Reconfigure the connection for the new source vertex.
  //
  // Prerequisite: the connection must be established
  //
  // Postcondition: the connection is reconfigured or unchanged.
  boost::optional<std::pair<int, int> >
  reconfigure(vertex);

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

  // Set the reconfiguration type.
  static void
  set_re(const std::string &);

  // Get the reconfiguration type.
  static re_t
  get_re();

  void
  tear_down();

private:
  bool
  reconfigure_complete(const demand &);

  bool
  reconfigure_proposed(const demand &);

  // The parameter that tells how to reconfigure connections.
  static re_t m_re;

  graph &m_g;
  demand m_d;
  boost::optional<sscpath> m_p;

  int m_id;

  static int counter;
};

#endif /* CONNECTION_HPP */
