#include "client.hpp"

#include <utility>

#include "simulation.hpp"
#include "stats.hpp"
#include "traffic.hpp"
#include "utils_netgen.hpp"

using namespace std;

client::client(double mht, double mnsc, traffic &tra):
  mht(mht), htd(1 / mht), htg(rng, htd),
  mnsc(mnsc - 1), nscd(1 / mnsc), nscdg(rng, nscd),
  conn(g), st(stats::get()), tra(tra)
{
  if (set_up())
    {
      // Register the client with the traffic.
      tra.insert(this);
      // Tear down time.
      tdt = now() + htg();
      schedule(tdt);
    }
}

void client::operator()(double t)
{
  tear_down();
}

bool client::set_up()
{
  // The new demand.
  demand d;
  // The demand end nodes.
  d.first = random_node_pair(g, rng);
  // The number of subcarriers the signal requires.  It's Poisson + 1.
  d.second = nscdg() + 1;

  // Set up the connection.
  pair<bool, int> result = conn.establish(d);

  st.established(result.first);

  if (result.first)
    st.established_length(result.second);
  else
    destroy();

  return result.first;
}

void client::tear_down()
{
  assert(conn.is_established());
  conn.tear_down();
  st.completed(true);
  destroy();
}

void client::destroy()
{
  tra.erase(this);
  delete this;
}
