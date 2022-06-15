#include <benchmark/benchmark.h>

#include <urand/uniform.hpp>
#include <uhash/map.hpp>
#include <unordered_map>

namespace {

using namespace std;

template <class T>
void InsertDelete(benchmark::State& state) {
  vector<pair<int, int>> keys(state.range(0));
  for (auto&& e : keys) {
    e = {urand::Uniform<int>(), urand::Uniform<int>()};
  }
  vector<int> random_incrs(keys.size());
  for (auto&& e : random_incrs) {
    e = urand::Uniform(0, (int)keys.size() - 1);
  }
  T map;
  for (auto _ : state) {
    for (auto&& e : keys) {
      ++map[e];
    }
    for (int i : random_incrs) {
      ++map[keys[i]];
    }
    for (auto x : keys) {
      map.erase(x);
    }
  }
}

static void BM_OurInsertDelete(benchmark::State& state) {
  InsertDelete<uhash::Map<pair<int, int>, int64_t>>(state);
}

static void BM_StdInsertDelete(benchmark::State& state) {
  InsertDelete<std::unordered_map<pair<int, int>, int64_t, uhash::Hasher>>(
      state);
}

}  // namespace

BENCHMARK(BM_OurInsertDelete)  //
    ->Arg(100)                 //
    ->Arg(10'000)              //
    ->Arg(100'000)             //
    ->Arg(1'000'000);          //

BENCHMARK(BM_StdInsertDelete)  //
    ->Arg(100)                 //
    ->Arg(10'000)              //
    ->Arg(100'000)             //
    ->Arg(1'000'000);          //
