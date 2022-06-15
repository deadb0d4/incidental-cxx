#include <benchmark/benchmark.h>

#include <numeric>
#include <vector>
#include <uhash/map.hpp>
#include <urand/uniform.hpp>
#include <algo/closest_points.hpp>

namespace {

using namespace std;

auto RandomPickTestcase(
    int count,
    int l = numeric_limits<int>::min(),
    int r = numeric_limits<int>::max()) {
  uhash::Set<pair<int, int>> uniques;
  while (count--) {
    auto x = urand::Uniform(l, r);
    auto y = urand::Uniform(l, r);
    uniques.insert(make_pair(x, y));
  }
  return vector<pair<int, int>>(begin(uniques), end(uniques));
}

void FastWay(benchmark::State& state, const vector<pair<int, int>>& pts) {
  pair<size_t, size_t> p;
  for (auto _ : state) {
    benchmark::DoNotOptimize(p = algo::ClosestEuclideanPair(pts));
  }
}

void SlowWay(benchmark::State& state, const vector<pair<int, int>>& pts) {
  pair<size_t, size_t> p;
  for (auto _ : state) {
    benchmark::DoNotOptimize(p = algo::slow::ClosestEuclideanPair(pts));
  }
}

static void FastWaySparse(benchmark::State& state) {
  FastWay(state, RandomPickTestcase(state.range(0)));
}

static void SlowWaySparse(benchmark::State& state) {
  SlowWay(state, RandomPickTestcase(state.range(0)));
}

static void FastWayDense(benchmark::State& state) {
  int len = state.range(0);
  int bound = len / 10;
  FastWay(state, RandomPickTestcase(len, 0, bound));
}

static void SlowWayDense(benchmark::State& state) {
  int len = state.range(0);
  int bound = len / 10;
  SlowWay(state, RandomPickTestcase(len, 0, bound));
}

}  // namespace

// Register the function as a benchmark
BENCHMARK(SlowWayDense)  //
    ->Arg(1000)          //
    ->Arg(10'000);       //

BENCHMARK(SlowWaySparse)  //
    ->Arg(10)             //
    ->Arg(100)            //
    ->Arg(1000)           //
    ->Arg(10'000)         //
    ->Arg(50'000);        //

BENCHMARK(FastWayDense)  //
    ->Arg(1000)          //
    ->Arg(10'000)        //
    ->Arg(100'000);      //

BENCHMARK(FastWaySparse)  //
    ->Arg(10)             //
    ->Arg(100)            //
    ->Arg(1000)           //
    ->Arg(10'000)         //
    ->Arg(100'000)        //
    ->Arg(500'000);       //
