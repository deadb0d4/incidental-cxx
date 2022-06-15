#include <gtest/gtest.h>

#include <cmath>

#include <algo/closest_points.hpp>
#include <bitmask/bitmask.hpp>
#include <limits>
#include <debug/to_string.hpp>
#include <debug/macros.hpp>
#include <urand/uniform.hpp>
#include <uhash/map.hpp>

namespace {

using namespace std;

double GetMinimalDistance(const vector<pair<int, int>>& pts) {
  auto [i, j] = algo::slow::ClosestEuclideanPair(pts);
  return algo::EuclideanDist(pts[i], pts[j]);
}

void RandomPickTest(
    int count,
    int l = numeric_limits<int>::min(),
    int r = numeric_limits<int>::max()) {
  uhash::Set<pair<int, int>> uniques;
  while (count--) {
    auto x = urand::Uniform(l, r);
    auto y = urand::Uniform(l, r);
    uniques.insert(make_pair(x, y));
  }
  vector<pair<int, int>> pts(begin(uniques), end(uniques));
  if (uniques.size() < 2) {
    EXPECT_ANY_THROW(algo::ClosestEuclideanPair(pts));
    return;
  }
  auto [i, j] = algo::ClosestEuclideanPair(pts);
  EXPECT_EQ(algo::EuclideanDist(pts[i], pts[j]), GetMinimalDistance(pts))
      << (pts.size() < 100 ? debug::ToString(pts) : "too big");
}

}  // namespace

TEST(ClosestPoints, EuclideanDist) {
  EXPECT_EQ(5.0, algo::EuclideanDist({3, 4}, {0, 0}));
  EXPECT_EQ(5.0, algo::EuclideanDist({13, 24}, {10, 20}));
  EXPECT_EQ(sqrt(2), algo::EuclideanDist({3, 4}, {4, 3}));
}

TEST(ClosestPoints, SlowClosestEuclideanPair) {
  vector<pair<int, int>> pts = {
      {-4, 1},
      {-1, 3},
      {-2, 1},
      {-1, -2},
      {2, 2},
      {3, -2},
      {-5, 2},
      {-8, 3},
  };
  EXPECT_EQ(
      (std::pair<size_t, size_t>{0, 6}), algo::slow::ClosestEuclideanPair(pts));
}

TEST(ClosestPoints, ClosestEuclideanPairEdge) {
  EXPECT_ANY_THROW(algo::ClosestEuclideanPair({}));
  EXPECT_ANY_THROW(algo::ClosestEuclideanPair({{-1, 2}}));
  {
    vector<pair<int, int>> pts = {
        {14, 23},
        {10, 20},
        {14, 23},
        {10, 20},
    };
    auto [i, j] = algo::ClosestEuclideanPair(pts);
    EXPECT_EQ(0.0, algo::EuclideanDist(pts[i], pts[j]));
  }
  {
    vector<pair<int, int>> pts = {
        {14, 23},
        {14, 23},
        {10, 20},
    };
    auto [i, j] = algo::ClosestEuclideanPair(pts);
    EXPECT_EQ(0.0, algo::EuclideanDist(pts[i], pts[j]));
  }
}

TEST(ClosestPoints, ClosestEuclideanPairTiny) {
  vector<pair<int, int>> pts = {
      {14, 23},
      {10, 20},
  };
  auto [i, j] = algo::ClosestEuclideanPair(pts);
  EXPECT_EQ(5.0, algo::EuclideanDist(pts[i], pts[j]));
}

TEST(ClosestPoints, ClosestEuclideanPairBase) {
  using namespace bitmask;
  vector<pair<int, int>> all_pts;
  for (int i = 0; i <= 4; i += 2) {
    for (int j = -10; j <= 10; j += 7) {
      all_pts.emplace_back(i, j);
    }
  }
  for (auto sub : Subsets(Full(all_pts.size()))) {
    if (sub.Count() < 2) {
      continue;
    }
    vector<pair<int, int>> pts;
    for (int i : sub.KeysLess(all_pts.size())) {
      pts.push_back(all_pts[i]);
    }
    auto [i, j] = algo::ClosestEuclideanPair(pts);
    EXPECT_EQ(GetMinimalDistance(pts), algo::EuclideanDist(pts[i], pts[j]))
        << debug::ToString(pts);
  }
}

TEST(ClosestPoints, ClosestEuclideanPairRandomDense) {
  for (int t = 0; t < 5; ++t) {
    for (int w : {5, 10, 15}) {
      for (int c = 0; c <= w * w; c += w) {
        RandomPickTest(c, -w, w);
      }
    }
  }
}

TEST(ClosestPoints, ClosestEuclideanPairRandomBigValues) {
  int offset = numeric_limits<int>::max() / 2;
  for (int c : {10, 100, 1000}) {
    for (int t = 0; t < (10000 / c); ++t) {
      RandomPickTest(c, offset - 100, offset + 100);
    }
  }
}

TEST(ClosestPoints, ClosestEuclideanPairRandomUnbounded) {
  for (int t = 0; t < 10; ++t) {
    RandomPickTest(1000);
  }
}
