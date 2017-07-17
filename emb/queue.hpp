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

#ifndef __QUEUE_HPP__
#define __QUEUE_HPP__

#include "span.hpp"

namespace emb {

template <typename T> class queue {
public:
  queue(span<T> storage) : storage(storage) {}

  void push(const T &val) {

    assert(size() < maxSize());
    storage[rearIndex()] = val;
    currentSize++;
  }
  void pop() {
    assert(size() > 0);
    frontIndex = (frontIndex + 1) % maxSize();
    currentSize--;
  }

  T &front() {
    assert(size() > 0);
    return storage[frontIndex];
  }
  const T &front() const {
    assert(size() > 0);
    return storage[frontIndex];
  }

  size_t size() const { return currentSize; }
  bool empty() const { return (currentSize == 0); }

  size_t maxSize() const { return storage.size; }

protected:
  span<T> storage;
  size_t frontIndex = 0;
  size_t currentSize = 0;

  size_t rearIndex() { return (frontIndex + size()) % maxSize(); }
};

template <typename T, size_t N> struct queuebuilder {
  array<T, N> a;
  queue<T> q;

  queuebuilder() : q(a) {}
};
}

#endif
