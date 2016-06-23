#ifndef ROUTING_HPP
#define ROUTING_HPP

#include "graph.hpp"
#include "sim.hpp"

#include <memory>
#include <boost/optional.hpp>

class routing: public sim
{  
public:
  // The type of specturm selection:
  // first - the first (lowest number of a slice) contiguous fragment
  // fittest - the fittest (smallest) contiguous fragment
  // random - any contiguous fragment
  enum class st_t {none, first, fittest, random};

  // The type of routing:
  // cdijkstra - constrained dijkstra
  enum class rt_t {none, cdijkstra, edksp, yenksp};

  /**
   * Route the demand, i.e., find the path, and allocate resources.
   * If function fails, no result is returned.
   */
  static boost::optional<sscpath>
  route(graph &g, const demand &d);

  /**
   * Route the demand, i.e., find the path, and allocate resources.
   * If function fails, no result is returned.
   */
  static boost::optional<sscpath>
  route(graph &g, const demand &d, const SSC &ssc);

  /**
   * Tear down the path in the graph.  This process puts back the
   * slices on the edges that are used by the path.
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

  /**
   * Set up the given path.
   */
  bool
  set_up_path(graph &g, const sscpath &p);

protected:
  /**
   * This is the worker function for the route function.
   */
  virtual sscpath
  route_w(graph &g, const demand &d) = 0;

  /**
   * Make the template depend on the SCDT (slice data type).  We make
   * the function templated, because we can use it for both SSC and
   * SSSC.
   *
   * From the given SCDT, select the SSC according to the selection
   * policy of the class.  The returned SSC has exactly nsc
   * slices, even though the chosen contiguous fragment from SSC
   * could have had more slices.
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

      case st_t::random:
        ssc = select_random(scdt, nsc);
        break;

      default:
        assert(false);
      }

    if (ssc.size() > nsc)
      {
        // Now in ssc we have got a fragment that has more than nsc
        // slices, and so we need to trim the result to have
        // exactly nsc slices.
        SSC::const_iterator fin = ssc.begin();
        advance(fin, nsc);
        ssc.erase(fin, ssc.end());
      }

    assert(ssc.size() == nsc || ssc.empty());

    return ssc;
  }

  // Select a contiguous ssc with the lowest slice numbers from
  // SSC.  It returns the whole available fragment, i.e. it can have
  // more slices than nsc.
  static SSC
  select_first(const SSC &ssc, int nsc);

  // Select the fittest slices, i.e. the smallest slice
  // fragment from SSC that fits the required number of slices.
  // It returns the whole available fragment, i.e. it can have more
  // slices than nsc.
  static SSC
  select_fittest(const SSC &ssc, int nsc);

  // Select the random slices, i.e. the random slice fragment from SSC
  // that fits the required number of slices.  It returns the whole
  // available fragment, i.e. it can have more slices than nsc.
  static SSC
  select_random(const SSC &ssc, int nsc);

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
