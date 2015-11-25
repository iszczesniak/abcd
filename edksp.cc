#include "edksp.hpp"

#include "edge_disjoint_ksp.hpp"
#include "graph.hpp"
#include "utils.hpp"

using namespace std;

sscpath
edksp::route_w(graph &g, const demand &d)
{
  sscpath result;

  vertex s = d.first.first;
  vertex t = d.first.second;

  std::list<std::pair<int, path>> paths;
  paths = boost::edge_disjoint_ksp(g, s, t);

  for (auto const &p: paths)
    // Consider that path when there is no maximal length given or
    // when the path lenght is not greater than the limit.
    if (!m_ml || p.first <= m_ml.get())
      {
        // Find all the SSC along the path.
        SSC ssc = find_path_ssc(g, p.second);
        // Select the right SSC.
        ssc = select_ssc(ssc, d.second);
        if (!ssc.empty())
          {
            // Candidate sscpath.
            sscpath cp(p.second, ssc);
            if (set_up_path(g, cp))
              // The routing was successful, so return here.
              return cp;
          }
      }

  // Return here in the case of failure.
  return sscpath();
}
