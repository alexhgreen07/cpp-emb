#ifndef __TEST_FSM_HPP__
#define __TEST_FSM_HPP__

#include <CppUTest/TestHarness.h>
#include <unistd.h>

#include "fsm.hpp"

using namespace FsmFramework;

struct MockClock : public System::ClockInterface {
  virtual unsigned long millis() override { return rawMillis; }
  virtual unsigned long micros() override { return rawMillis * 1000; }
  virtual void delay(unsigned int ms) override { usleep(ms * 1000); }
  virtual void delayMicroseconds(unsigned int us) override { usleep(us); };

  void tick(unsigned long millisToTick = 1) { rawMillis += millisToTick; }

  unsigned long rawMillis = 0;
};

struct MockSystem : public System::SystemInterface {
  virtual void startCriticalSection() override {}
  virtual void endCriticalSection() override {}
};

struct FsmTestGroupBase : public Utest {
  MockClock testClock;
  Scheduler scheduler;
  MockSystem system;

  FsmTestGroupBase();

  void execute(unsigned int count = 1, unsigned int ticks = 0);

  unsigned int totalExecutionCount = 0;
};

struct TestFsm : public Fsm {
  using Fsm::expired;

  virtual void initial() override;
  void increment();
  void ping();
  void pong();
  void incrementAndSleep();
  void startFsm();
  void doneFsm();
  void incremenetFromSignal();

  unsigned int counter = 0;
  unsigned int pingCount = 0;
  unsigned int pongCount = 0;

  static constexpr unsigned int delay = 100;

  struct TestChildFsm : public Fsm {

    virtual ~TestChildFsm(){};
    using Fsm::start;
    virtual void initial() override;

    unsigned int counter = 0;
  } testFsm;

  bool done = false;

  Signal *signal = NULL;
};

#endif
