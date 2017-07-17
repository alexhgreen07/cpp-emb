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

namespace emb
{

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

}

#endif
