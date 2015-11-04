#ifndef ROUTING_HPP
#define ROUTING_HPP

#include "graph.hpp"
#include <memory>

class routing
{  
public:
  // The type of specturm selection:
  // first - the first subcarriers that fit the demand are chosen
  // smallest - the smallest set of contiguous subcarriers are chosen
  enum select_t {first, fittest};

  /**
   * Route the demand, i.e., find the path, and allocate resources.
   */
  static sscpath
  route(graph &g, const demand &d);
  
  /**
   * Tear down the path in the graph.  This process puts back the
   * subcarriers on the edges that are used by the path.
   */
  static void
  tear_down(graph &g, const sscpath &p);

  /**
   * Set the maximul length of a path.
   */
  static void
  set_ml(int ml);

  /**
   * Set the spectrum selection type.
   */
  static void
  set_st(const std::string &st);

  /**
   * Set the routing type.
   */
  static void
  set_rt(const std::string &rt);
  
protected:
  /**
   * This is the worker function for the route function.
   */
  virtual sscpath
  route_w(graph &g, const demand &d) = 0;

  /**
   * Set up the given path.
   */
  void
  set_up_path(graph &g, const sscpath &p);
  
  /**
   * From the given SSSC, select the SSC according to the "select"
   * field of the class.  The returned SSC has exactly nsc
   * subcarriers, even though the chosen contiguous fragment from SSSC
   * had more subcarriers.
   */
  SSC
  select_ssc(const SSSC &sssc, int nsc);

  // Select a contiguous ssc with the lowest subcarrier numbers from
  // SSSC.  It returns the whole available fragment, i.e. it can have
  // more subcarriers than nsc.
  SSC
  select_first(const SSSC &sssc, int nsc);

  // Select a contiguous ssc with the lowest subcarrier numbers from
  // SSC.  It returns the whole available fragment, i.e. it can have
  // more subcarriers than nsc.
  SSC
  select_first(const SSC &ssc, int nsc);

  // Select the fittest subcarriers, i.e. the smallest subcarrier
  // fragment that fits the required subcarriers from SSSC.  It
  // returns the whole available fragment, i.e. it can have more
  // subcarriers than nsc.
  SSC
  select_fittest(const SSSC &sssc, int nsc);

  // Select the fittest subcarriers, i.e. the smallest subcarrier
  // fragment from SSC that fits the required number of subcarriers.
  // It returns the whole available fragment, i.e. it can have more
  // subcarriers than nsc.
  SSC
  select_fittest(const SSC &ssc, int nsc);

  /// The spectrum selection type.
  static select_t m_st;

  /// The maximal length of a path.
  static int m_ml;

  /// The singleton, which does the routing.
  static std::unique_ptr<routing> singleton;
};

#endif /* ROUTING_HPP */
