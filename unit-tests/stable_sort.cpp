#include <gtest/gtest.h>

#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>
#include <string>

#include <algo/stable_sort.hpp>
#include <urand/uniform.hpp>
#include <debug/to_string.hpp>

namespace {

using namespace std;

bool compare_first(const pair<int, int>& lhs, const pair<int, int>& rhs) {
  return lhs.first < rhs.first;
}

template <class T, class Cmp = std::less<T>>
void Check(vector<T>& vec, Cmp cmp = {}) {
  auto copy = vec;
  algo::StableSort(begin(vec), end(vec), cmp);
  EXPECT_TRUE(is_sorted(begin(vec), end(vec))) << debug::ToString(copy) << '\n'
                                               << debug::ToString(vec) << '\n';
}

void ReverseCheck(size_t size) {
  vector<int> vec(size);
  iota(begin(vec), end(vec), 0);
  reverse(begin(vec), end(vec));
  Check(vec);
}

auto RandomVec(int size) {
  vector<int> vec(size);
  for (auto&& e : vec) {
    e = urand::Uniform(0, size);
  }
  return vec;
}

void PermutationCheck(int size) {
  EXPECT_TRUE(size < 7);

  vector<int> vec(size);
  iota(begin(vec), end(vec), 0);
  do {
    auto input = vec;
    Check(input);
  } while (std::next_permutation(begin(vec), end(vec)));
}

auto IndexVector(const vector<int>& vec) {
  vector<pair<int, int>> res(vec.size());
  for (int i = 0; i < vec.size(); ++i) {
    res[i] = {vec[i], i};
  }
  return res;
}

}  // namespace

TEST(MergeSortWithoutBuffer, Simple) {
  vector<vector<int>> inputs = {
      {}, {1}, {2, 1, 2}, {3, 2, 1}, {1, 2, 3}, {3, 1, 2}};
  for (auto&& vec : inputs) {
    Check(vec);
  }
}

TEST(MergeSortWithoutBuffer, ReverseIota) {
  for (int i = 0; i < 20; ++i) {
    ReverseCheck(i);
  }
}

TEST(MergeSortWithoutBuffer, RandomSort) {
  for (int i = 0; i < 20; ++i) {
    auto vec = RandomVec(i);
    Check(vec);
  }
}

TEST(MergeSortWithoutBuffer, PermutationCheck) {
  for (int i = 0; i < 7; ++i) {
    PermutationCheck(i);
  }
}

TEST(MergeSortWithoutBuffer, StableSimple) {
  vector<vector<int>> inputs = {
      {},
      {1},
      {2, 1},
      {2, 1, 2},
      {2, 1, 1},
      {1, 2, 1},
      {1, 1, 1},
      {1, 1, 0, 2, 2, 6, 7}};
  for (auto&& vec : inputs) {
    auto idx = IndexVector(vec);
    Check(idx, compare_first);
  }
}

TEST(MergeSortWithoutBuffer, StableAlternate) {
  vector<int> vec(20);
  for (int i = 0; i < vec.size(); ++i) {
    vec[i] = i % 2;
  }
  auto idx = IndexVector(vec);
  Check(idx, compare_first);
}

TEST(MergeSortWithoutBuffer, StableRandom) {
  int times = 10;
  while (times--) {
    auto vec = RandomVec(30);
    auto idx = IndexVector(vec);
    Check(idx, compare_first);
  }
}
