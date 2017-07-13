#ifndef __ALLOCATOR_HPP__
#define __ALLOCATOR_HPP__

#include <stdlib.h>

class RawAllocator {
public:
  virtual ~RawAllocator() {}
  virtual void *alloc(size_t size) = 0;
  virtual void free(void *ptr) = 0;
};

class Allocator {
public:
  Allocator(RawAllocator &rawAllocator) : rawAllocator(rawAllocator) {}

  template <typename T, typename... ARGS> T *allocate(ARGS &&... args) {
    return new (*this) Allocated<T>(*this, args...);
  }

  template <typename T> void deallocate(T *ptr) { delete ptr; }

private:
  template <typename T> class Allocated : public T {
  public:
    template <typename... ARGS>
    Allocated(Allocator &stack, ARGS &&... args)
        : T(args...), allocator(stack) {}

    static void *operator new(size_t, Allocator &allocator) {
      return allocator.rawAllocator.alloc(sizeof(Allocated<T>));
    }

    static void operator delete(void *ptr) {
      Allocated *obj = (Allocated *)ptr;
      obj->allocator.rawAllocator.free(ptr);
    }

    Allocator &allocator;
  };

  RawAllocator &rawAllocator;
};

#endif
