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

namespace emb {

template <typename T, size_t N> struct array {
  T storage[N];

  T &operator[](size_t i) {
    assert(i < size());
    return storage[i];
  };
  const T &operator[](size_t i) const {
    assert(i < size());
    return storage[i];
  };

  constexpr size_t size() const { return N; }

  T *data() { return storage; }
  constexpr T *data() const { return storage; }

  typedef T *iterator;
  typedef const T *const_iterator;

  const_iterator begin() const { return &storage[0]; }
  const_iterator end() const { return (&storage[N]); }

  iterator begin() { return &storage[0]; }
  iterator end() { return (&storage[N]); }
};
}

#endif
