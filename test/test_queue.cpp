#include <CppUTest/TestHarness.h>

#include "queue.hpp"

using namespace emb;

static constexpr size_t arraySize = 10;

TEST_BASE(QueueTestGroupBase) {
  array<char, arraySize> storage;
  queue<char> testQueue;

  QueueTestGroupBase() : testQueue(storage) {}
};

TEST_GROUP_BASE(Queue, QueueTestGroupBase){

};

TEST(Queue, BasicInitialization) {
  LONGS_EQUAL(0, testQueue.size());
  LONGS_EQUAL(arraySize, testQueue.maxSize());
}

TEST(Queue, PushingSingleElementIncreasesSize) {
  testQueue.push('a');
  LONGS_EQUAL(1, testQueue.size());
}

TEST(Queue, PoppingSingleElementDecreasesSize) {
  testQueue.push('a');
  testQueue.pop();
  LONGS_EQUAL(0, testQueue.size());
}

TEST(Queue, PushedSingleElementAccessibleInFront) {
  char pushedElement = 'a';
  testQueue.push(pushedElement);
  LONGS_EQUAL(pushedElement, testQueue.front());
}

TEST(Queue, PushedMultipleElementAccessibleInFront) {
  char startElement = 'a';
  for (auto pushedElement = startElement; pushedElement < 'd';
       pushedElement++) {
    testQueue.push(pushedElement);
  }

  LONGS_EQUAL(startElement, testQueue.front());
}

TEST(Queue, PushedAndPoppedElementsAccessibleInFront) {
  for (char pushedElement = 'a'; pushedElement < 'd'; pushedElement++) {
    testQueue.push(pushedElement);
    LONGS_EQUAL(pushedElement, testQueue.front());
    testQueue.pop();
  }
}
