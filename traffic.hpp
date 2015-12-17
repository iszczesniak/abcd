#ifndef TRAFFIC_HPP
#define TRAFFIC_HPP

#include <queue>
#include <set>

#include <boost/random.hpp>

#include "client.hpp"
#include "graph.hpp"
#include "module.hpp"

class traffic: public module
{
  // The set of active clients.
  std::set<client *> cs;

  // The queue of clients to delete later.
  std::queue<client *> dl;

  // The ID counter.
  int idc;

  // The mean client arrival time.
  double mcat;
  // The holding time distribution.
  boost::exponential_distribution<> catd;
  // The holding time generator.
  boost::variate_generator<boost::mt19937 &,
                           boost::exponential_distribution<> > catg;

  double mht, mnsc;

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

private:
  void schedule_next(double);

  void delete_clients(bool active);
};

#endif /* TRAFFIC_HPP */
