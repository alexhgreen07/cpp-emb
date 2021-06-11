#include "fsm.hpp"

#include "system.hpp"

using namespace System;

namespace FsmFramework {
Fsm::Fsm() : scheduler(NULL), next(NULL), parent(NULL) {}

void Fsm::execute() {
  if (next != 0) {
    (this->*next)();
  }
}

void Fsm::setup(Scheduler &scheduler) {
  this->scheduler = &scheduler;
  nextState((State)&Fsm::initial);
}

void Fsm::sleep(unsigned long milliseconds) {
  assert(scheduler != NULL);
  scheduler->sleepCurrentExecutor(milliseconds);
}

void Fsm::wait(Signal &signal, unsigned long milliseconds) {
  assert(scheduler != NULL);
  signal.waitCurrentExecutor(*scheduler, milliseconds);
}

void Fsm::start(Fsm &newParent) {

  assert(newParent.scheduler != NULL);

  parent = &newParent;
  setup(*parent->scheduler);
  scheduler->transfer(*this);
}

void Fsm::done() {
  if (parent == NULL) {
    scheduler->terminate();
  } else {
    scheduler->transfer(*parent);
  }
}

Scheduler::Entry::Entry(Fsm *fsm) : fsm(fsm), signal(NULL), expiry(0) {}

Scheduler::Scheduler(ClockInterface &clock, SystemInterface &system)
    : clock(clock), system(system), currentExecutor(NULL) {}

void Scheduler::start(Executor &executor) {
  ready.insert(executor);
  executor.value.fsm->setup(*this);
}

void Scheduler::terminate() { ready.remove(*currentExecutor); }

void Scheduler::execute() {
  Executor *current = ready.getHead();
  Executor *next;

  while (current != NULL) {
    currentExecutor = current;
    next = current->next;

    current->value.fsm->execute();

    current = next;
  }

  wakeupExpired(waiting);
  wakeup(pending);
}

void Scheduler::sleepCurrentExecutor(unsigned long milliseconds) {
  insertCurrentExecutorByTimeout(milliseconds, waiting);
}

void Scheduler::transfer(Fsm &fsm) { currentExecutor->value.fsm = &fsm; }

void Scheduler::insertCurrentExecutorByTimeout(unsigned long milliseconds,
                                               list<Entry> &pendingList,
                                               Signal *signal) {

  enterCriticalSection();

  ready.remove(*currentExecutor);

  unsigned long newExpiry = clock.millis();
  if (milliseconds == (unsigned long)-1) {
    newExpiry = milliseconds;
  } else {
    newExpiry += milliseconds;
  }
  Executor *current = pendingList.getHead();

  size_t index = 0;
  while (current != NULL) {
    if (current->value.expiry >= newExpiry) {
      break;
    }
    current = current->next;
    index++;
  }

  pendingList.insert(*currentExecutor, index);
  currentExecutor->value.expiry = newExpiry;
  currentExecutor->value.signal = signal;
  if (signal != NULL)
    signal->scheduler = this;

  exitCriticalSection();
}

void Scheduler::wakeupExpired(list<Entry> &pendingList) {

  enterCriticalSection();

  Executor *current = pendingList.getHead();
  Executor *next;

  while (current != NULL) {
    next = current->next;

    if (clock.millis() >= current->value.expiry) {
      pendingList.remove(0);
      current->value.signal = NULL;
      ready.insert(*current);
    } else {
      break;
    }
    current = next;
  }

  exitCriticalSection();
}

void Scheduler::wakeup(list<Entry> &pendingList) {

  enterCriticalSection();

  while (pendingList.getLength() > 0) {
    auto executor = pendingList.remove(0);
    ready.append(*executor);
  }

  exitCriticalSection();
}

void Scheduler::wakeup(list<Entry> &pendingList, Signal &signal, bool all,
                       bool isr) {

  if (!isr)
    enterCriticalSection();

  Executor *current = pendingList.getHead();
  Executor *next;

  while (current != NULL) {
    next = current->next;

    if (current->value.signal == &signal) {
      pendingList.remove(*current);
      current->value.signal = NULL;
      pending.insert(*current);
      if (!all)
        break;
    }
    current = next;
  }

  if (!isr)
    exitCriticalSection();
}

bool Scheduler::isIdle() { return (ready.getLength() == 0); }

void Scheduler::enterCriticalSection() { system.startCriticalSection(); }

void Scheduler::exitCriticalSection() { system.endCriticalSection(); }

void Signal::waitCurrentExecutor(Scheduler &newScheduler,
                                 unsigned long milliseconds) {
  newScheduler.insertCurrentExecutorByTimeout(milliseconds,
                                              newScheduler.waiting, this);
}

void Signal::signal() {
  if (scheduler != NULL) {
    scheduler->wakeup(scheduler->waiting, *this, false);
  }
}

void Signal::signalIsr() {
  if (scheduler != NULL) {
    scheduler->wakeup(scheduler->waiting, *this, false, true);
  }
}

void Signal::broadcast() {
  if (scheduler != NULL) {
    scheduler->wakeup(scheduler->waiting, *this, true);
  }
}

void buildExecutor(Allocator &allocator, FsmFramework::Scheduler &scheduler,
                   FsmFramework::Fsm &fsm) {
  auto *executor = allocator.allocate<FsmFramework::Scheduler::Executor>(
      FsmFramework::Scheduler::Entry(&fsm));
  scheduler.start(*executor);
}
}
