#include <gtest/gtest.h>

#include <vector>
#include <algorithm>
#include <numeric>
#include <utility>

#include <debug/to_string.hpp>

namespace {

using namespace std;

void RotateSlow(vector<int>& vec, int s) {
  s %= vec.size();
  const int j = vec.size() - s;
  reverse(begin(vec), begin(vec) + j);
  reverse(begin(vec) + j, end(vec));
  reverse(begin(vec), end(vec));
}

void RotateFast(vector<int>& vec, int s) {
  s %= vec.size();

  for (int i = 0; i < gcd(s, (int)vec.size()); ++i) {
    for (int j = (i + s) % vec.size(); j != i; j = (j + s) % vec.size()) {
      swap(vec[i], vec[j]);
    }
  }
}

}  // namespace

TEST(ListShift, Slow) {
  {
    vector<int> vec = {1, 2, 3, 4, 5};
    RotateSlow(vec, 2);
    EXPECT_EQ(vec, (vector{4, 5, 1, 2, 3}));
  }
  {
    vector<int> vec = {1, 2, 3};
    RotateSlow(vec, 1);
    EXPECT_EQ(vec, (vector{3, 1, 2}));
  }
}

TEST(ListShift, Fast) {
  {
    vector<int> vec = {1, 2, 3, 4, 5};
    RotateFast(vec, 2);
    EXPECT_EQ(vec, (vector{4, 5, 1, 2, 3}));
  }
  {
    vector<int> vec = {1, 2, 3};
    RotateFast(vec, 1);
    EXPECT_EQ(vec, (vector{3, 1, 2}));
  }
}

TEST(ListShift, SlowVFast) {
  for (int size : {1, 2, 4, 5, 6, 7, 10, 12}) {
    for (int s = 0; s < 2 * size; ++s) {
      vector<int> vec(size);
      iota(begin(vec), end(vec), 0);

      auto cpy = vec;
      RotateFast(vec, s);
      RotateSlow(cpy, s);
      EXPECT_EQ(vec, cpy) << "s = " << s;
    }
  }
}
