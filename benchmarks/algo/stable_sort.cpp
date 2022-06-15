#include <benchmark/benchmark.h>

#include <vector>

#include <urand/uniform.hpp>
#include <algo/stable_sort.hpp>

namespace {

using namespace std;

auto BuildRandomVec(int size) {
  vector<int> vec(size);
  for (auto&& e : vec) {
    e = urand::Uniform(0, size);
  }
  return vec;
}

void SlowSort(vector<int>& vec) {
  for (int i = 0; i < vec.size(); ++i) {
    for (int j = i; j-- > 1;) {
      if (vec[j - 1] > vec[j]) {
        swap(vec[j - 1], vec[j]);
      }
    }
  }
}

static void SlowSortBench(benchmark::State& state) {
  auto vec = BuildRandomVec(state.range(0));
  for (auto _ : state) {
    SlowSort(vec);
  }
}

static void RandomSortOurs(benchmark::State& state) {
  auto vec = BuildRandomVec(state.range(0));
  // Perform setup here
  for (auto _ : state) {
    // This code gets timed
    algo::StableSort(begin(vec), end(vec), less());
  }
}

static void StdSort(benchmark::State& state) {
  auto vec = BuildRandomVec(state.range(0));
  for (auto _ : state) {
    std::stable_sort(begin(vec), end(vec));
  }
}

}  // namespace

// Register the function as a benchmark
BENCHMARK(RandomSortOurs)
    ->Arg(10)
    ->Arg(50)
    ->Arg(100)
    ->Arg(1000)
    ->Arg(10'000)
    ->Arg(100'000)
    ->Arg(1'000'000)
    ->Arg(10'000'000);

BENCHMARK(SlowSortBench)->Arg(10)->Arg(50)->Arg(100)->Arg(1000);

BENCHMARK(StdSort)
    ->Arg(10)
    ->Arg(100)
    ->Arg(1000)
    ->Arg(10'000)
    ->Arg(100'000)
    ->Arg(1'000'000)
    ->Arg(10'000'000);

// Run the benchmark
BENCHMARK_MAIN();
