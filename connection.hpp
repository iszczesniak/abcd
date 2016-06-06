#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "graph.hpp"

#include <utility>

// The type of the connection.  It can establish, reconfigure and tear
// down a connection, but it doesn't report the statistics.
class connection
{
public:
  // The reconfiguration types:
  // complete - route the connection anew
  // our - our method of reconfiguration
  enum class re_t {none, complete, our};

  connection(graph &g);
  ~connection();

  const demand &
  get_demand() const;

  bool
  is_established() const;

  // Establish the connection for the given demand.  True is
  // successfull.
  bool
  establish(const demand &d);

  // Reconfigure the connection.
  std::pair<bool, int>
  reconfigure(vertex new_Src);

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
  set_re(const std::string &re);

  // Get the reconfiguration type.
  static re_t
  get_re();

  void
  tear_down();

private:
  std::pair<bool, int>
  reconfigure_complete(vertex new_Src);

  std::pair<bool, int>
  reconfigure_incremental(vertex new_Src);

  // The parameter that tells how to reconfigure connections.
  static re_t m_re;

  // This is the sscpath with status.
  typedef std::pair<bool, sscpath> sscpathws;

  graph &m_g;
  demand m_d;
  sscpathws m_p;

  int m_id;

  static int counter;
};

#endif /* CONNECTION_HPP */
