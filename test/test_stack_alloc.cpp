#include "stack_alloc.hpp"

#include <CppUTest/TestHarness.h>

using namespace emb;

struct TestStruct {
  int stuff;
  char otherstuff;
  long morestuff;

  TestStruct() {
    morestuff = 0;
    otherstuff = 1;
    stuff = 2;
  }

  TestStruct(int stuff, char otherstuff)
      : stuff(stuff), otherstuff(otherstuff) {
    morestuff = 3;
  }
};

TEST_BASE(StackAllocBase) {
  array<char, 64> storage;
  StackAllocator stack;
  Allocator allocator;

  StackAllocBase() : stack(storage), allocator(stack) {}
};

TEST_GROUP_BASE(StackAlloc, StackAllocBase){};

TEST(StackAlloc, BasicInitialization) {
  LONGS_EQUAL(storage.size(), stack.available());
}

TEST(StackAlloc, AllocationWithoutArguments) {
  auto testStruct = allocator.allocate<TestStruct>();

  LONGS_EQUAL(2, testStruct->stuff);
  LONGS_EQUAL(1, testStruct->otherstuff);
  LONGS_EQUAL(0, testStruct->morestuff);
}

TEST(StackAlloc, AllocationWithArguments) {
  auto testStruct = allocator.allocate<TestStruct>(4, 5);

  LONGS_EQUAL(3, testStruct->morestuff);
  LONGS_EQUAL(5, testStruct->otherstuff);
  LONGS_EQUAL(4, testStruct->stuff);
}

TEST(StackAlloc, AllocateAndDeallocate) {
  auto testStruct = allocator.allocate<TestStruct>();
  allocator.deallocate(testStruct);
  LONGS_EQUAL(storage.size(), stack.available());
}

TEST(StackAlloc, AllocateAndDelete) {
  auto testStruct = allocator.allocate<TestStruct>();
  delete testStruct;
  LONGS_EQUAL(storage.size(), stack.available());
}
