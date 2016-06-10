#ifndef MODULE_HPP
#define MODULE_HPP

// This is the abstract class for modules.  Modules are able to
// process messages.
template<typename S>
class module: public S
{
public:
  // This function processes an event for this module.
  virtual void operator()(S::time_type t) = 0;

  // Schedule an event for this module at the current time.
  void schedule()
  {
    schedule(now(), this);
  }

  // Schedule an event for this module at time t.
  void schedule(S::time_type t)
  {
    schedule(t, this);
  }
};

#endif /* MODULE_HPP */
