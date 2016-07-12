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
  enum class re_t {none, complete, curtailing, proposed};

  // The reconfiguration result.  First - new.  Second - reused.
  typedef boost::optional<std::pair<int, int> > rr_t;
    
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

  // Reconfigure the connection for the new destination vertex.
  //
  // Prerequisite: the connection must be established
  //
  // Postcondition: the connection is reconfigured or unchanged.
  rr_t
  reconfigure(vertex);

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

  // Set the reconfiguration type.
  static void
  set_re(const std::string &);

  // Get the reconfiguration type.
  static re_t
  get_re();

  void
  tear_down();

private:
  rr_t
  reconfigure_complete(const demand &);

  // In the curtailing reconfiguration we look for the shortest
  // briding path between any of the nodes of the connection being
  // reconfigured, and the new destination node.
  rr_t
  reconfigure_curtailing(const demand &);

  rr_t
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
