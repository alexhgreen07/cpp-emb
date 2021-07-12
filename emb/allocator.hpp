/*
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __ALLOCATOR_HPP__
#define __ALLOCATOR_HPP__

#include <stdlib.h>

namespace emb {

class RawAllocator {
public:
  virtual ~RawAllocator() {}
  virtual void *allocate(size_t size) = 0;
  virtual void deallocate(void *ptr, size_t size) = 0;
};

class Allocator {
public:
  template <typename T> class Allocated : public T {
  public:
    template <typename... ARGS>
    Allocated(Allocator &stack, ARGS &&...args)
        : T(args...), allocator(stack) {}

    static void *operator new(size_t, Allocator &allocator) {
      return allocator.rawAllocator.allocate(sizeof(Allocated<T>));
    }

    static void operator delete(void *ptr) {
      Allocated *obj = (Allocated *)ptr;
      obj->allocator.rawAllocator.deallocate(ptr, sizeof(Allocated<T>));
    }

  private:
    Allocator &allocator;
  };

  Allocator(RawAllocator &rawAllocator) : rawAllocator(rawAllocator) {}

  template <typename T, typename... ARGS>
  Allocated<T> *allocate(ARGS &&...args) {
    return new (*this) Allocated<T>(*this, args...);
  }

  template <typename T> void deallocate(T *ptr) {
    Allocated<T> *allocPtr = (Allocated<T> *)ptr;
    delete allocPtr;
  }

  template <typename T> void deallocate(Allocated<T> *ptr) { delete ptr; }

  RawAllocator &rawAllocator;
};
} // namespace emb

#endif
