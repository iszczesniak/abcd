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

  std::multimap<int, path> paths;
  paths = boost::edge_disjoint_ksp(g, s, t);

  for (auto const &p: paths)
    {
      // Find all the SSC along the path.
      SSC ssc = find_path_ssc(g, p.second);
      // Select the right SSC.
      ssc = select_ssc(ssc, d.second);
      // Candidate sscpath.
      sscpath cp(p.second, ssc);
      if (set_up_path(g, cp))
        // The routing was successful, so return here.
        return cp;
    }

  // Return here in the case of failure.
  return sscpath();
}
