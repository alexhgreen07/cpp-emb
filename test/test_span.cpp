#include <CppUTest/TestHarness.h>

#include "span.hpp"

using namespace emb;

TEST_GROUP(Span){};

TEST(Span, Empty) {
  span<char> sp;

  CHECK(sp.empty());
  LONGS_EQUAL(0, sp.size());
  POINTERS_EQUAL(NULL, sp.data());
}

TEST(Span, RawArrayInitialization) {
  char testArray[] = {0, 1, 2, 3, 4, 5};
  span<char> sp(testArray);

  LONGS_EQUAL(sizeof(testArray), sp.size());
  POINTERS_EQUAL(testArray, sp.data());
}

TEST(Span, PointerAndSizeInitialization) {
  static constexpr unsigned int arraySize = 10;
  char testArray[arraySize];
  span<char> sp(testArray, arraySize);

  LONGS_EQUAL(arraySize, sp.size());
  POINTERS_EQUAL(testArray, sp.data());
}

TEST(Span, ArrayInitialization) {
  static constexpr unsigned int arraySize = 10;
  array<char, arraySize> testArray;
  span<char> sp(testArray);

  LONGS_EQUAL(arraySize, sp.size());
  POINTERS_EQUAL(testArray.data(), sp.data());
}

TEST(Span, DataAccess) {
  char testArray[] = {0, 1, 2, 3, 4, 5};
  span<char> sp(testArray);

  for (unsigned int i = 0; i < sp.size(); i++) {
    LONGS_EQUAL(i, sp[i]);
    LONGS_EQUAL(i, sp(i));
    LONGS_EQUAL(i, sp.at(i));
  }
}

TEST(Span, Iterator) {
  char testArray[] = {0, 1, 2, 3, 4, 5};
  span<char> sp(testArray);

  unsigned int i = 0;
  for (auto &v : sp) {
    LONGS_EQUAL(i, v);
    i++;
  }

  LONGS_EQUAL(sizeof(testArray), i);
}

TEST(Span, Comparison) {
  char testArray0[] = {0, 1, 2, 3, 4, 5};
  char testArray1[] = {0, 1, 2, 3, 4};
  span<char> sp0(testArray0);
  span<char> sp1(testArray1);

  CHECK(sp0 == sp0);
  CHECK(sp0 != sp1);
}

TEST(Span, First) {
  char testArray[] = {0, 1, 2, 3, 4, 5};
  span<char> sp(testArray);

  static constexpr unsigned int subspanSize = 2;

  auto first = sp.first(subspanSize);
  LONGS_EQUAL(subspanSize, first.size());

  for (unsigned int i = 0; i < first.size(); i++) {
    LONGS_EQUAL(i, first[i]);
  }
}

TEST(Span, Subspan) {

  char testArray[] = {0, 1, 2, 3, 4, 5};
  span<char> sp(testArray);

  static constexpr unsigned int subspanSize = 2;
  auto subspan = sp.subspan(0, subspanSize);

  CHECK(subspan == sp.first(subspanSize));
}
