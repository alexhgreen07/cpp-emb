#ifndef __STACK_ALOC_HPP__
#define __STACK_ALOC_HPP__

#include "assert.h"

#include "allocator.hpp"

#include "array.hpp"

class StackAllocator : public RawAllocator {
public:
  StackAllocator(span<char> stack) : stack(stack), index(0) {}
  virtual ~StackAllocator() {}

  virtual void *alloc(size_t count) override {
    void *returnValue = NULL;
    returnValue = push(count);
    size_t *bytesPushed = (size_t *)push(sizeof(size_t));
    *bytesPushed = count;
    return returnValue;
  }

  virtual void free(void *ptr) override {
    pop(sizeof(size_t));
    size_t *bytesToPop = (size_t *)&stack[index];
    pop(*bytesToPop);
    assert(ptr != &stack[index]);
  }

  size_t available() const { return (stack.size - index); }

protected:
  void *push(size_t count) {
    void *returnValue = NULL;
    assert(count <= available());
    returnValue = (void *)&stack[index];
    index += count;
    return returnValue;
  }

  void pop(size_t count) { index -= count; }

  span<char> stack;
  unsigned int index;
};

#endif
