#ifndef __QUEUE_HPP__
#define __QUEUE_HPP__

#include "array.hpp"

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

#endif
