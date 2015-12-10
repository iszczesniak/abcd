#ifndef ROUTING_HPP
#define ROUTING_HPP

#include "graph.hpp"

#include <memory>
#include <boost/optional.hpp>

class routing
{  
public:
  // The type of specturm selection:
  // first - the first subcarriers that fit the demand are chosen
  // smallest - the smallest set of contiguous subcarriers are chosen
  enum class st_t {none, first, fittest};

  // The type of routing:
  // cdijkstra - constrained dijkstra
  enum class rt_t {none, cdijkstra, edksp, yenksp};
  
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
   * The maximum length of a path.
   */
  static void
  set_ml(boost::optional<int> ml);

  static boost::optional<int>
  get_ml();

  /**
   * The K for the k-shortest paths.
   */
  static void
  set_K(boost::optional<unsigned> K);

  static boost::optional<unsigned>
  get_K();

  /**
   * Set the spectrum selection type.
   */
  static void
  set_st(const std::string &st);

  static st_t
  get_st();

  /**
   * Set the routing type.
   */
  static void
  set_rt(const std::string &rt);

  static rt_t
  get_rt();

protected:
  /**
   * This is the worker function for the route function.
   */
  virtual sscpath
  route_w(graph &g, const demand &d) = 0;

  /**
   * Set up the given path.
   */
  bool
  set_up_path(graph &g, const sscpath &p);

  /**
   * Make the template depend on the SCDT (subcarrier data type).  We
   * make the function templated, because we use it for both SSC and
   * SSSC.
   *
   * From the given SCDT, select the SSC according to the selection
   * policy of the class.  The returned SSC has exactly nsc
   * subcarriers, even though the chosen contiguous fragment from SSC
   * could have had more subcarriers.
   */
  template <typename SCDT>
  static SSC
  select_ssc(const SCDT &scdt, int nsc)
  {
    // This is the selected set.
    SSC ssc;

    switch (m_st)
      {
      case st_t::first:
        ssc = select_first(scdt, nsc);
        break;

      case st_t::fittest:
        ssc = select_fittest(scdt, nsc);
        break;

      default:
        assert(false);
      }

    if (ssc.size() > nsc)
      {
        // Now in ssc we have got a fragment that has more than nsc
        // subcarriers, and so we need to trim the result to have
        // exactly nsc subcarriers.
        SSC::const_iterator fin = ssc.begin();
        advance(fin, nsc);
        ssc.erase(fin, ssc.end());
      }

    assert(ssc.size() == nsc || ssc.empty());

    return ssc;
  }

  // Select a contiguous ssc with the lowest subcarrier numbers from
  // SSSC.  It returns the whole available fragment, i.e. it can have
  // more subcarriers than nsc.
  static SSC
  select_first(const SSSC &sssc, int nsc);

  // Select a contiguous ssc with the lowest subcarrier numbers from
  // SSC.  It returns the whole available fragment, i.e. it can have
  // more subcarriers than nsc.
  static SSC
  select_first(const SSC &ssc, int nsc);

  // Select the fittest subcarriers, i.e. the smallest subcarrier
  // fragment that fits the required subcarriers from SSSC.  It
  // returns the whole available fragment, i.e. it can have more
  // subcarriers than nsc.
  static SSC
  select_fittest(const SSSC &sssc, int nsc);

  // Select the fittest subcarriers, i.e. the smallest subcarrier
  // fragment from SSC that fits the required number of subcarriers.
  // It returns the whole available fragment, i.e. it can have more
  // subcarriers than nsc.
  static SSC
  select_fittest(const SSC &ssc, int nsc);

  // Interpret the string and return the routing type.
  static rt_t
  rt_interpret (const std::string &rt);

  // Interpret the string and return the spectrum selection type.
  static st_t
  st_interpret (const std::string &st);

  /// The spectrum selection type.
  static st_t m_st;

  /// The maximal length of a path.
  static boost::optional<int> m_ml;

  /// The K for the k-shortest paths.
  static boost::optional<unsigned> m_K;

  /// The routing algorithm.
  static rt_t m_rt;

  /// The singleton, which does the routing.
  static std::unique_ptr<routing> singleton;
};

#endif /* ROUTING_HPP */
