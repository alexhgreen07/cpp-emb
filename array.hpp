#ifndef __ARRAY_HPP__
#define __ARRAY_HPP__

#include "stdlib.h"
#include <assert.h>

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

#endif
