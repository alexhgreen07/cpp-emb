#include <CppUTest/TestHarness.h>

#include <array.hpp>

#include "test_fsm.hpp"

using emb::array;
using namespace FsmFramework;

bool TestFsm::expired() { return Fsm::expired(); }

void TestFsm::initial() {}

void TestFsm::increment() { counter++; }

void TestFsm::ping() {
  pingCount++;
  nextState(&TestFsm::pong);
}

void TestFsm::pong() {
  pongCount++;
  nextState(&TestFsm::ping);
}

void TestFsm::incrementAndSleep() {
  counter++;
  sleep(delay);
}

void TestFsm::startFsm() {
  testFsm.start(*this);
  nextState(&TestFsm::doneFsm);
}

void TestFsm::doneFsm() { done = true; }

void TestFsm::incremenetFromSignal() {
  counter++;
  wait(*signal, delay);
}

void TestFsm::TestChildFsm::initial() {
  counter++;
  done();
}

FsmTestGroupBase::FsmTestGroupBase() : scheduler(testClock, system) {}

void FsmTestGroupBase::execute(unsigned int count, unsigned int ticks) {
  for (unsigned int i = 0; i < count; i++) {
    scheduler.execute();
    testClock.tick(ticks);
    totalExecutionCount++;
  }
}

struct FsmBase : public FsmTestGroupBase {
  array<Scheduler::Executor, 10> executors;
  array<TestFsm, 9> otherFsms;
  TestFsm testFsm;
  Signal signal;

  FsmBase() {
    unsigned int cnt = 0;
    for (auto &fsm : otherFsms) {
      executors[cnt].value.fsm = &fsm;
      scheduler.start(executors[cnt]);
      cnt++;
    }
    executors[cnt].value.fsm = &testFsm;
    scheduler.start(executors[cnt]);

    testFsm.signal = &signal;

    for (auto &fsm : otherFsms) {
      fsm.signal = &signal;
      fsm.nextState(&TestFsm::increment);
    }
  }
};

TEST_GROUP_BASE(Fsm, FsmBase){

};

TEST(Fsm, SingleStateExecution) {
  testFsm.nextState(&TestFsm::increment);
  scheduler.execute();

  LONGS_EQUAL(1, testFsm.counter);
  for (auto &fsm : otherFsms) {
    LONGS_EQUAL(1, fsm.counter);
  }
}

TEST(Fsm, SingleStateMultiExecution) {
  unsigned int executionCount = 10;

  testFsm.nextState(&TestFsm::increment);

  execute(executionCount);

  LONGS_EQUAL(executionCount, testFsm.counter);
  for (auto &fsm : otherFsms) {
    LONGS_EQUAL(totalExecutionCount, fsm.counter);
  }
}

TEST(Fsm, PingPongExecution) {
  unsigned int executionCount = 10;

  testFsm.nextState(&TestFsm::ping);

  execute(2 * executionCount);

  LONGS_EQUAL(executionCount, testFsm.pingCount);
  LONGS_EQUAL(executionCount, testFsm.pongCount);
  for (auto &fsm : otherFsms) {
    LONGS_EQUAL(totalExecutionCount, fsm.counter);
  }
}

TEST(Fsm, IncrementWithSleep) {
  unsigned int executionCount = 10;

  testFsm.nextState(&TestFsm::incrementAndSleep);

  execute(executionCount);
  LONGS_EQUAL(1, testFsm.counter);

  testClock.tick(testFsm.delay);

  execute(executionCount);

  LONGS_EQUAL(2, testFsm.counter);
  for (auto &fsm : otherFsms) {
    LONGS_EQUAL(totalExecutionCount, fsm.counter);
  }
}

TEST(Fsm, ChildFsmTransfersExecution) {
  testFsm.nextState(&TestFsm::startFsm);

  while (!testFsm.done) {
    execute();
  }

  LONGS_EQUAL(1, testFsm.testFsm.counter);
  for (auto &fsm : otherFsms) {
    LONGS_EQUAL(totalExecutionCount, fsm.counter);
  }
}

TEST(Fsm, WaitsForSignal) {
  unsigned int executionCount = 10;
  unsigned int signalCount = 10;

  testFsm.nextState(&TestFsm::incremenetFromSignal);

  execute(executionCount);

  LONGS_EQUAL(1, testFsm.counter);

  for (unsigned int i = 0; i < signalCount; i++) {
    signal.signal();
    execute(executionCount);
  }

  LONGS_EQUAL(executionCount + 1, testFsm.counter);
  for (auto &fsm : otherFsms) {
    LONGS_EQUAL(totalExecutionCount, fsm.counter);
  }
  CHECK_FALSE(testFsm.expired());
}

TEST(Fsm, WaitsForBroadcast) {
  unsigned int executionCount = 10;
  unsigned int signalCount = 10;

  testFsm.nextState(&TestFsm::incremenetFromSignal);
  for (auto &fsm : otherFsms) {
    fsm.nextState(&TestFsm::incremenetFromSignal);
  }

  execute(executionCount);

  LONGS_EQUAL(1, testFsm.counter);
  for (auto &fsm : otherFsms) {
    LONGS_EQUAL(1, fsm.counter);
  }

  for (unsigned int i = 0; i < signalCount; i++) {
    signal.broadcast();
    execute(executionCount);
  }

  LONGS_EQUAL(executionCount + 1, testFsm.counter);
  for (auto &fsm : otherFsms) {
    LONGS_EQUAL(executionCount + 1, fsm.counter);
  }
}

TEST(Fsm, WaitsForSignalTimeout) {
  unsigned int executionCount = 10;

  testFsm.nextState(&TestFsm::incremenetFromSignal);

  execute(executionCount);

  testClock.tick(testFsm.delay);

  execute(executionCount);

  LONGS_EQUAL(2, testFsm.counter);
  for (auto &fsm : otherFsms) {
    LONGS_EQUAL(totalExecutionCount, fsm.counter);
  }
  CHECK(testFsm.expired());
}

TEST(Fsm, WaitsForMultipleSignals) {
  unsigned int executionCount = 10;
  unsigned int signalCount = 10;
  Signal otherSignal;

  testFsm.nextState(&TestFsm::incremenetFromSignal);
  for (auto &fsm : otherFsms) {
    fsm.signal = &otherSignal;
    fsm.nextState(&TestFsm::incremenetFromSignal);
  }

  execute(executionCount);

  LONGS_EQUAL(1, testFsm.counter);
  for (auto &fsm : otherFsms) {
    LONGS_EQUAL(1, fsm.counter);
  }

  for (unsigned int i = 0; i < signalCount; i++) {
    signal.signal();
    execute(executionCount);
  }

  LONGS_EQUAL(executionCount + 1, testFsm.counter);
  for (auto &fsm : otherFsms) {
    LONGS_EQUAL(1, fsm.counter);
  }

  for (unsigned int i = 0; i < signalCount; i++) {
    otherSignal.broadcast();
    execute(executionCount);
  }

  LONGS_EQUAL(executionCount + 1, testFsm.counter);
  for (auto &fsm : otherFsms) {
    LONGS_EQUAL(executionCount + 1, fsm.counter);
  }
}

TEST(Fsm, CheckNextState) {
  testFsm.nextState(&TestFsm::ping);

  scheduler.execute();
  CHECK(testFsm.isNextState(&TestFsm::pong));

  scheduler.execute();
  CHECK(testFsm.isNextState(&TestFsm::ping));
}
