#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include "event.hpp"
#include "module.hpp"

#include <queue>

template<typename T, typename M, typename R>
class simulation
{
  typedef simulation<T, M, R> self;

  // The DES priority queue.
  static std::priority_queue<event<self>> m_q;

  // Keeps track of the simulation time.
  static T m_t;

protected:
  // The model we're working on.
  static M m_mdl;

  // The random number generator.
  static R m_rng;

public:
  typedef T time_type;
  typedef M model_type;
  typedef R rng_type;

  // Get the model.
  static M &
  mdl()
  {
    return m_mdl;
  }

  // Get the random number generator.
  static R &
  rng()
  {
    return m_rng;
  }

  // Run the simulation with the given simulation time limit.
  static void
  run(T limit)
  {
    // The event loop.
    while(!m_q.empty())
      {
        const event<self> &e = m_q.top();

        m_t = e.get_time();

        if (m_t > limit)
          break;

        e.process();
        m_q.pop();
      }
  }

  // Schedule an event at the given time for the given module.
  static void
  schedule(T t, module<self> *m)
  {
    assert(t >= m_t);
    m_q.push(event<self>(t, m));
  }

  // Return the current simulaion time.
  static T
  now()
  {
    return m_t;
  }
};

template<typename T, typename M, typename R>
std::priority_queue<event<simulation<T, M, R>>> simulation<T, M, R>::m_q;

template<typename T, typename M, typename R>
M simulation<T, M, R>::m_mdl;

template<typename T, typename M, typename R>
R simulation<T, M, R>::m_rng;

template<typename T, typename M, typename R>
T simulation<T, M, R>::m_t;

#endif /* SIMULATION_HPP */
