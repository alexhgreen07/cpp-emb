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

  constexpr span() : storage(NULL), storageSize(0) {}

  constexpr span(const span &other) = default;
  constexpr span(span &&other) = default;

  constexpr span(T *storage, size_t size)
      : storage(storage), storageSize(size) {}

  template <size_t N> constexpr span(T (&arr)[N]) : span(&arr[0], N) {}

  template <size_t N>
  constexpr span(array<T, N> &arr) : span(&arr[0], arr.size()) {}

  constexpr span first(size_t count) const {
    assert(count >= 0 && count <= size());
    return {data(), count};
  }

  constexpr span last(size_t count) const {
    assert(count >= 0 && count <= size());
    return {data() + (size() - count), count};
  }

  constexpr span subspan(size_t offset, size_t count) const {
    assert((offset == 0 || (offset > 0 && offset <= size())) &&
           (count == storageSize || (count >= 0 && offset + count <= size())));
    return {data() + offset, count == storageSize ? size() - offset : count};
  }

  T &operator[](size_t i) {
    assert(i < storageSize);
    return storage[i];
  };

  constexpr size_t size() const { return storageSize; }
  constexpr size_t length() const { return size(); }
  constexpr bool empty() const { return size() == 0; }

  constexpr T &at(size_t i) const { return this->operator[](i); }
  constexpr T &operator()(size_t i) const { return this->operator[](i); }
  constexpr T *data() const { return storage; }

  typedef T *iterator;
  typedef const T *const_iterator;

  const_iterator begin() const { return &storage[0]; }
  const_iterator end() const { return (&storage[storageSize]); }

  iterator begin() { return &storage[0]; }
  iterator end() { return (&storage[storageSize]); }

private:
  T *storage;
  size_t storageSize;
};
template <typename T>
inline constexpr bool operator==(const span<T> &l, const span<T> &r) {
  if (l.size() != r.size())
    return false;

  auto liter = l.begin();
  auto riter = r.begin();

  do {
    if (*liter != *riter)
      return false;
    liter++;
    riter++;
  } while (liter != l.end());

  return true;
}

template <typename T>
inline constexpr bool operator!=(const span<T> &l, const span<T> &r) {
  return !(l == r);
}
}

#endif
