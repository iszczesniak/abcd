#ifndef IJS_SOURCE
#define IJS_SOURCE

#include "event.hpp"
#include <boost/random.hpp>

class source
{
  boost::mt19937 &r;
  boost::exponential_distribution<> ed;
  boost::variate_generator<boost::mt19937 &,
                           boost::exponential_distribution<> > g;

  pqueue &q;
  int id;

public:
  source(pqueue &, int, boost::mt19937 &, double);
  void operator()(double);
  void schedule(double);
};

#endif
