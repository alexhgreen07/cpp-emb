#include <CppUTest/TestHarness.h>

#include "array.hpp"

static constexpr size_t arraySize = 10;

using namespace emb;

TEST_GROUP(Array) { array<char, arraySize> testArray; };

TEST(Array, BasicInitialization) { LONGS_EQUAL(arraySize, testArray.size); }

TEST(Array, SetAndGetSingleElement) {
  testArray[0] = 'a';
  CHECK(testArray[0] == 'a');
}

TEST(Array, SetAndGetMultipleElements) {
  unsigned int start = 2;
  unsigned int end = 2;

  for (unsigned int i = start; i < end; i++) {
    testArray[i] = i;
  }

  for (unsigned int i = start; i < end; i++) {
    LONGS_EQUAL(i, testArray[i]);
  }
}

TEST(Array, RangeBaseForLoop) {
  for (unsigned int i = 0; i < arraySize; i++) {
    testArray[i] = i;
  }

  char count = 0;
  for (auto c : testArray) {
    LONGS_EQUAL(count, c);
    count++;
  }
}
