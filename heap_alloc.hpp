#ifndef __HEAP_ALOC_HPP__
#define __HEAP_ALOC_HPP__

#include "assert.h"

#include "allocator.hpp"

class HeapAllocator : public RawAllocator {
public:
  virtual ~HeapAllocator() {}

  virtual void *alloc(size_t count) override { return malloc(count); }

  virtual void free(void *ptr) override { free(ptr); }
};

#endif
