#ifndef __FSM_HPP__
#define __FSM_HPP__

#include <assert.h>

#include "allocator.hpp"
#include "array.hpp"
#include "list.hpp"
#include "system.hpp"

using namespace System;

namespace FsmFramework {
class Scheduler;
class Signal;

class Fsm {
  friend class Scheduler;

public:
  typedef void (Fsm::*State)();

  Fsm();
  virtual ~Fsm() {}

  /*!
  \brief Sets the next state of the state machine
  \param next The next state of the state machine. This can be a class
  function pointer of any class which can be cast into the \ref Fsm class.
  */
  template <class T> void nextState(void (T::*next)());

protected:
  void start(Fsm &newParent);

  virtual void initial() = 0;
  void done();

  void wait(Signal &signal, unsigned long milliseconds = (unsigned long)-1);
  void sleep(unsigned long milliseconds);

  bool expired();

  Scheduler *scheduler;

private:
  void setup(Scheduler &scheduler);

  /*!
  \brief Executes the next state in the state machine
  */
  void execute();
  State next;
  Fsm *parent;
  Signal *lastSignal;
};

class Scheduler {
  friend class Signal;

public:
  struct Entry {
    Entry(Fsm *fsm = NULL);

    Fsm *fsm;
    Signal *signal;
    unsigned long expiry;
  };

  typedef list<Entry>::node Executor;

  Scheduler(ClockInterface &clock, SystemInterface &system);
  virtual ~Scheduler() {}

  void start(Executor &executor);
  void terminate();

  void execute();

  void sleepCurrentExecutor(unsigned long milliseconds);
  void transfer(Fsm &fsm);

  bool isIdle();

  virtual void enterCriticalSection();
  virtual void exitCriticalSection();

private:
  void insertCurrentExecutorByTimeout(unsigned long milliseconds,
                                      list<Entry> &pendingList,
                                      Signal *signal = NULL);

  void wakeupExpired(list<Entry> &pendingList);
  void wakeup(list<Entry> &pendingList);
  void wakeup(list<Entry> &pendingList, Signal &signal, bool all = false,
              bool isr = false);

  ClockInterface &clock;
  SystemInterface &system;
  list<Entry> ready;
  list<Entry> waiting;
  list<Entry> pending;
  Executor *currentExecutor;
};

struct Signal {
  friend class Scheduler;

  void waitCurrentExecutor(Scheduler &scheduler,
                           unsigned long milliseconds = (unsigned long)-1);
  void signal();
  void signalIsr();
  void broadcast();

protected:
  Scheduler *scheduler = NULL;
};

template <class T> void Fsm::nextState(void (T::*next)()) {
  this->next = (State)next;
}

void buildExecutor(emb::Allocator &allocator,
                   FsmFramework::Scheduler &scheduler, FsmFramework::Fsm &fsm);
} // namespace FsmFramework

#endif
