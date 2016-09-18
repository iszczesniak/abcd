#ifndef MODULE_HPP
#define MODULE_HPP

// This is the abstract class for modules.  Modules are able to
// process messages.
template <typename S>
struct module: public S
{
  // This function processes an event for this module.
  virtual void operator()(typename S::time_type t) = 0;

  // Schedule an event for this module at the current time.
  void schedule()
  {
    schedule(S::now());
  }

  // Schedule an event for this module at time t.
  void schedule(typename S::time_type t)
  {
    S::schedule(t, this);
  }
};

#endif /* MODULE_HPP */
