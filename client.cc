#include "client.hpp"

#include <utility>

#include "simulation.hpp"
#include "stats.hpp"
#include "traffic.hpp"
#include "utils_netgen.hpp"

using namespace std;

client::client(double mht, double mnsc, traffic &tra):
  mht(mht), htd(1 / mht), htg(rng, htd),
  mnsc(mnsc), nscd(mnsc - 1), nscdg(rng, nscd),
  conn(g), st(stats::get()), tra(tra)
{
  if (set_up())
    {
      // Register the client with the traffic.
      tra.insert(this);
      // Holding time.
      double ht = htg();
      // Tear down time.
      tdt = now() + ht;
      schedule(tdt);
    }
  else
    tra.delete_me_later(this);
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
  bool status = conn.establish(d);

  // Report whether the connection was established or not.
  st.established(status);

  // If established, report more info.
  if (status)
    st.established_info(conn.get_length(), conn.get_hops(), conn.get_nsc());

  return status;
}

void client::tear_down()
{
  assert(conn.is_established());
  conn.tear_down();
  tra.erase(this);
  tra.delete_me_later(this);
}
