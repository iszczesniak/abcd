#ifndef TRAFFIC_HPP
#define TRAFFIC_HPP

class traffic: public module
{
  // The list of clients.
  std::list<client *> vc;

  // The random number generator.
  boost::mt19937 &rng;

public:
  /**
   * q is the DES queue, id is the id of the client, r is the random
   * number generator, l_sleep is the lambda of the exponential
   * distribution with mean (1/lambda), mnc is the mean number (for
   * the Poisson distribution) of cell changes that a client makes,
   * l_change is the lambda of the exponential distribution of time
   * between changes.
   */
  client(graph &g, pqueue &q, int id, boost::mt19937 &r,
         double mht, double mbst, double mdct, double mnsc);
  ~client();

  // Schedule the new event based on the current state of the client.
  // This function doesn't change the state of the client.
  void schedule(double t);

  // Processes the event and changes the state of the client.
  void operator()(double t);

  // Tell whether the client is idle.
  bool is_idle();

private:
  std::pair<bool, int>
  set_up();

  std::pair<bool, int>
  reconfigure();

  void
  tear_down();
};

#endif /* TRAFFIC_HPP */
