#include "simulation.hpp"

using namespace std;

simulation::simulation(graph &g):
  g(g)
{
  assert(!singleton);
  singleton = this;
}
