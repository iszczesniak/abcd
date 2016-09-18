#include "yenksp.hpp"

#include "yen_ksp.hpp"
#include "graph.hpp"
#include "utils.hpp"

using namespace std;

sscpath
yenksp::route_w(graph &g, const demand &d, const SSC &ssc)
{
  sscpath result;

  vertex s = d.first.first;
  vertex t = d.first.second;

  std::list<std::pair<int, path>> paths = boost::yen_ksp(g, s, t, m_K);

  for (auto const &p: paths)
    // Consider that path when there is no maximal length given or
    // when the path lenght is not greater than the limit.
    if (!m_ml || p.first <= m_ml.get())
      {
        // This is the path SSC.
        SSC pssc = find_path_ssc(g, p.second);
        // This is the candidate SSC.
        SSC cssc = intersection(pssc, ssc);
        // This is the selected SSC.
        SSC sssc = select_ssc(cssc, d.second);
        if (!sssc.empty())
          {
            // Candidate sscpath.
            sscpath cp(p.second, sssc);
            if (set_up_path(g, cp))
              // The routing was successful, so return here.
              return cp;
          }
      }

  // Return here in the case of failure.
  return sscpath();
}
