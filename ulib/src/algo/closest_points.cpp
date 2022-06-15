#include <algo/closest_points.hpp>

#include <limits>
#include <tuple>
#include <stdexcept>
#include <cmath>
#include <iterator>
#include <boost/container/small_vector.hpp>

#include <uhash/map.hpp>
#include <debug/macros.hpp>
#include <debug/to_string.hpp>
#include <coro/single_pass.hpp>

namespace algo {

namespace {

using namespace std;

/// @returns: `s` such that
/// `width * s <= x < (width + 1) * s`
int GetBound(int x, int64_t width) {
  int r = x / width;
  if (x < 0 && x % width) {
    --r;
  }
  return r;
}

auto GetKey(pair<int, int> p, int64_t w) {
  return make_pair(GetBound(p.first, w), GetBound(p.second, w));
}

// build for [0, len)
auto BuildCells(
    int64_t w,
    const std::vector<std::pair<int, int>>& pts,
    size_t len) {
  using boost::container::small_vector;
  // in 2d one shouldn't encounter more than 5 points in a cell
  uhash::Map<pair<int64_t, int64_t>, small_vector<size_t, 5>> cells;
  if (!w) {
    // it is weird but better to return asap
    return cells;
  }
  for (int i = 0; i < len; ++i) {
    auto key = GetKey(pts[i], w);
    cells[key].push_back(i);
  }
  return cells;
}

coro::SinglePass<pair<int, int>> AdjCells(pair<int, int> key) {
  for (int dx = -1; dx <= 1; ++dx) {
    for (int dy = -1; dy <= 1; ++dy) {
      co_yield make_pair(key.first + dx, key.second + dy);
    }
  }
}

}  // namespace

double EuclideanDist(std::pair<int, int> lhs, std::pair<int, int> rhs) {
  auto x = (int64_t)lhs.first - rhs.first;
  auto y = (int64_t)lhs.second - rhs.second;
  return std::hypot(x, y);
}

namespace slow {

std::pair<size_t, size_t> ClosestEuclideanPair(
    const std::vector<std::pair<int, int>>& pts) {
  if (pts.empty()) {
    throw std::invalid_argument("no points");
  }
  std::tuple<double, size_t, size_t> res = {
      std::numeric_limits<double>::max(), 0, 0};
  for (size_t i = 0; i < pts.size(); ++i) {
    for (size_t j = 0; j < i; ++j) {
      res = min(res, std::make_tuple(EuclideanDist(pts[j], pts[i]), j, i));
    }
  }
  return std::make_pair(get<1>(res), get<2>(res));
}

}  // namespace slow

std::pair<size_t, size_t> ClosestEuclideanPair(
    const std::vector<std::pair<int, int>>& pts) {
  if (pts.size() < 2) {
    throw std::invalid_argument("< 2 points");
  }
  std::tuple<double, size_t, size_t> res = {
      EuclideanDist(pts[0], pts[1]), 0, 1};
  int64_t w = ceill(get<0>(res));
  auto cells = BuildCells(w, pts, 2);
  for (size_t i = 2; i < pts.size() && w; ++i) {
    auto key = GetKey(pts[i], w);
    for (auto adj_key : AdjCells(key)) {
      auto it = cells.find(adj_key);
      if (it == end(cells)) {
        continue;
      }
      for (size_t j : it->second) {
        res = min(res, std::make_tuple(EuclideanDist(pts[j], pts[i]), j, i));
      }
    }
    if (int64_t new_w = ceill(get<0>(res)); new_w != w) {
      w = new_w;
      cells = BuildCells(w, pts, i + 1);
    } else {
      cells[key].push_back(i);
    }
  }
  return std::make_pair(get<1>(res), get<2>(res));
}

}  // namespace algo
