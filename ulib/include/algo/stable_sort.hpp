/*
 * read about the algorithm here:
 * http://thomas.baudel.name/Visualisation/VisuTri/inplacestablesort.html
 *
 * Better algorithms overview:
 * https://cs.stackexchange.com/questions/2569/worst-case-on-ln-n-in-place-stable-sort
 */

#pragma once

#include <algorithm>
#include <iterator>
#include <vector>
#include <utility>
#include <numeric>
#include <concepts>

namespace algo {

namespace impl {

template <class T, class Iter>
concept Comparator = requires(T t, Iter a, Iter b) {
  { t(*a, *b) } -> std::same_as<bool>;
};

template <std::random_access_iterator It, impl::Comparator<It> Comparator>
class Sorter {
  bool Less(size_t l, size_t r) {
    return less(*(begin + l), *(begin + r));
  }

  void Swap(size_t l, size_t r) {
    std::swap(*(begin + l), *(begin + r));
  }

  void Reverse(size_t b, size_t e) {
    std::reverse(begin + b, begin + e);
  }

  /// @brief shifts segment [b, e) such that
  /// `m` moves right to the `b`
  void Shift(size_t b, size_t m, size_t e) {
    // it is easily vectorized as opposed to
    // `unit-tests/toy/list_shift.cpp` `RotateFast` function
    Reverse(b, m);
    Reverse(m, e);
    Reverse(b, e);
  }

  size_t LowerBound(size_t b, size_t e, size_t x) {
    return std::lower_bound(begin + b, begin + e, *(begin + x)) - begin;
  }

  size_t UpperBound(size_t b, size_t e, size_t x) {
    return std::upper_bound(begin + b, begin + e, *(begin + x)) - begin;
  }

  // [b, m) + [m, e)
  void Merge(size_t b, size_t m, size_t e) {
    if ((e - b) < 2) {
      return;
    }
    if ((e - b) == 2) {
      if (Less(b + 1, b)) {
        Swap(b + 1, b);
      }
      return;
    }
    size_t x, y;
    if ((e - m) > (m - b)) {
      x = m + (e - m) / 2;
      y = UpperBound(b, m, x);
    } else {
      y = b + (m - b) / 2;
      x = LowerBound(m, e, y);
    }
    Shift(y, m, x);
    m = y + (x - m);
    Merge(b, y, m);
    Merge(m, x, e);
  }

 public:
  Sorter(It begin, It end, Comparator less)
      : begin(begin), len((end - begin)), less(less) {
  }

  void Sort() {
    for (size_t l = 1; l < len; l *= 2) {
      for (size_t b = 0; b + l < len; b += 2 * l) {
        Merge(b, b + l, std::min(b + 2 * l, len));
      }
    }
  }

 private:
  const It begin;
  const size_t len;
  Comparator less;
};

}  // namespace impl

template <std::random_access_iterator It, impl::Comparator<It> Comparator>
void StableSort(It begin, It end, Comparator cmp) {
  auto sorter = impl::Sorter(begin, end, cmp);
  sorter.Sort();
}

}  // namespace algo
