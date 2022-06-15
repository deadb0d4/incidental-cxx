#include <gtest/gtest.h>

#include <bitmask/bitmask.hpp>
#include <urand/uniform.hpp>
#include <debug/to_string.hpp>
#include <debug/macros.hpp>

namespace {

using namespace std;

auto ToVec(bitmask::Bitmask mask) {
  vector<int> vec;
  for (int x : mask.KeysLess(32)) {
    vec.push_back(x);
  }
  return vec;
}

auto FromVec(const vector<int>& vec) {
  bitmask::Bitmask mask;
  for (int x : vec) {
    mask.Add(x);
  }
  return mask;
}

}  // namespace

TEST(Bitmask, Empty) {
  bitmask::Bitmask mask;
  ASSERT_EQ(mask.mask, 0);

  auto keys = ToVec(mask);
  EXPECT_TRUE(keys.empty());

  vector<vector<int>> subsets;
  for (auto sub : Subsets(mask)) {
    subsets.push_back(ToVec(sub));
  }
  ASSERT_EQ(subsets.size(), 1);
  EXPECT_TRUE(subsets[0].empty());
}

TEST(Bitmask, DelSimple) {
  auto mask = FromVec({1, 3, 4, 5});
  mask.Del(4);
  ASSERT_EQ(ToVec(mask), (vector{1, 3, 5}));
  mask.Del(5);
  ASSERT_EQ(ToVec(mask), (vector{1, 3}));
  mask.Del(1);
  ASSERT_EQ(ToVec(mask), (vector{3}));
}

TEST(Bitmask, Subsets) {
  auto mask = FromVec({1, 3, 5});

  vector<vector<int>> exp = {
      {1, 3, 5}, {3, 5}, {1, 5}, {5}, {1, 3}, {3}, {1}, {}};
  vector<vector<int>> got;
  for (auto sub : Subsets(mask)) {
    got.push_back(ToVec(sub));
  }
  ASSERT_EQ(got, exp);
}

TEST(Bitmask, RandomSubset) {
  auto mask = FromVec({1, 3, 5});
  // 101
  urand::debug::MockedUniformValue<int>() = 5;
  auto sub = bitmask::RandomSubset(mask);
  auto get = vector{1, 5};
  EXPECT_EQ(ToVec(sub), get);
}
