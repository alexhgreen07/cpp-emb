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

#ifndef __SPAN_HPP__
#define __SPAN_HPP__

#include <array.hpp>

namespace emb {

template <typename T> struct span {
  T *storage;
  size_t size;

  constexpr span() : storage(NULL), size(0) {}

  constexpr span(T *storage, size_t size) : storage(storage), size(size) {}

  template <size_t N>
  constexpr span(array<T, N> &arr) : span(&arr[0], arr.size) {}

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
