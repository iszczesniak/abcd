#ifndef TRAFFIC_HPP
#define TRAFFIC_HPP

#include <queue>
#include <set>

#include <boost/random.hpp>

#include "client.hpp"
#include "graph.hpp"
#include "module.hpp"
#include "sim.hpp"

class traffic: public module<sim>
{
  // The set of active clients.
  std::set<client *> cs;

  // The queue of clients to delete later.
  std::queue<client *> dl;

  // The ID counter.
  int idc;

  // The mean client arrival time.
  double mcat;
  // The client arrival time distribution.
  boost::exponential_distribution<> catd;
  // The client arrival time generator.
  boost::variate_generator<sim::rng_type &,
                           boost::exponential_distribution<> > catg;

  // The mean holding time.
  double mht;

  // The mean number of slices.
  double mnsc;

  // Shortest distances.
  mutable std::map<npair, int> sd;

public:
  traffic(double mcat, double mht, double mnsc);

  ~traffic();

  // Processes the event and changes the state of the client.
  void operator()(double t);

  // Return the number of clients.
  int nr_clients() const;

  // Insert the client to the traffic.
  void insert(client *);

  // Remote the client from the traffic.
  void erase(client *);

  // Delete this client later.
  void delete_me_later(client *);

  // Calculate the capacity currently served, which is defined as
  // \sum_{i = connections} nsc_i * sp_i, where nsc_i is the number of
  // slices of connection i, and sp_i is the length of the
  // shortest path between end nodes of connection i.
  int
  capacity_served() const;

private:
  void schedule_next(double);
  void delete_clients();
};

#endif /* TRAFFIC_HPP */
