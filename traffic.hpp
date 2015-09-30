#ifndef TRAFFIC_HPP
#define TRAFFIC_HPP

#include <list>

#include <boost/random.hpp>

#include "client.hpp"
#include "graph.hpp"
#include "module.hpp"

class traffic: public module
{
  // The list of clients.
  std::list<client *> lc;

  // The ID counter.
  int idc;

  // The mean client arrival time.
  double mcat;
  // The holding time distribution.
  boost::exponential_distribution<> catd;
  // The holding time generator.
  boost::variate_generator<boost::mt19937 &,
                           boost::exponential_distribution<> > catg;

  double mht, mbst, mdct, mnsc;

public:
  traffic(double mcat, double mht, double mbst,
          double mdct, double mnsc);

  ~traffic();

  // Processes the event and changes the state of the client.
  void operator()(double t);

  // Return the number of clients.
  int nr_clients() const;

private:
  void schedule_next(double);
};

#endif /* TRAFFIC_HPP */
