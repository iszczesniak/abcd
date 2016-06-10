#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include "event.hpp"
#include "module.hpp"

#include <queue>

// We need to forward-declare the module class.  We cannot include its
// header file, because that header file includes this file
// (simulation.hpp), which will not finally, define the simulation
// class, and the compilation fails.  At this moment the declaration
// of the module class is enough, because we only use a pointer to it.
class module;

template<typename T, typename M, typename R>
class simulation
{
  // The DES priority queue.
  std::priority_queue<event<S>> m_q;
  
  // The model we're working on.
  static M m_mdl;

  // The random number generator.
  static R m_rng;

  // Keeps track of the simulation time.
  T m_t;

public:
  typedef T time_type;
  typedef M model_type;
  typedef R rng_type;

  // Get the model.
  M &mdl() const
  {
    return m_mdl;
  }

  // Get the random number generator.
  R &
  rng() const
  {
    return m_rng;
  }

  // Run the simulation with the given simulation time limit.
  static void
  run(T)
  {
    // The event loop.
    while(!m_q.empty())
      {
        const event &e = m_q.top();

        m_t = e.get_time();

        if (m_t > sim_time)
          break;

        e.process();
        m_q.pop();
      }
  }

  // Schedule an event at the given time for the given module.
  static void
  schedule(T, module<S> *)
  {
    assert(t >= m_t);
    m_q.push(event(t, m));
  }

  // Return the current simulaion time.
  static T
  now()
  {
    return m_t;
  }
};

#endif /* SIMULATION_HPP */
