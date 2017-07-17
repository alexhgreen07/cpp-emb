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

#ifndef __ARRAY_HPP__
#define __ARRAY_HPP__

#include "stdlib.h"
#include <assert.h>

namespace emb
{

template <typename T> struct span {
  T *storage;
  size_t size;

  span() : storage(NULL), size(0) {}

  span(T *storage, size_t size) : storage(storage), size(size) {}

  template <typename T2>
  span(span<T2> val) : storage((T *)val.storage), size(val.size) {
    assert(sizeof(T2) == sizeof(T));
  }

  T &operator[](size_t i) {
    assert(i < size);
    return storage[i];
  };

  typedef T *iterator;
  typedef const T *const_iterator;

  const_iterator begin() const { return &storage[0]; }
  const_iterator end() const { return (&storage[size]); }

  iterator begin() { return &storage[0]; }
  iterator end() { return (&storage[size]); }
};

template <typename T, size_t N> struct array {
  T storage[N];
  static constexpr size_t size = N;

  T &operator[](size_t i) {
    assert(i < size);
    return storage[i];
  };
  const T &operator[](size_t i) const {
    assert(i < size);
    return storage[i];
  };

  operator span<T>() const { return span<T>{storage, size}; }

  operator span<T>() { return span<T>{storage, size}; }

  operator span<const T>() const { return span<const T>{storage, size}; }

  typedef T *iterator;
  typedef const T *const_iterator;

  const_iterator begin() const { return &storage[0]; }
  const_iterator end() const { return (&storage[size]); }

  iterator begin() { return &storage[0]; }
  iterator end() { return (&storage[size]); }
};

template <typename T> bool compare(span<T> lhs, span<T> rhs) {
  bool same = true;

  for (unsigned int i = 0; i < lhs.size; i++) {
    if (lhs[i] != rhs[i]) {
      same = false;
      break;
    }
  }

  return same;
}

template <typename T> void copy(span<T> dest, span<T> src) {
  for (unsigned int i = 0; i < src.size; i++) {
    dest[i] = src[i];
  }
}

}

#endif
