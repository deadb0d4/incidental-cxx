#include <benchmark/benchmark.h>

#include <iostream>
#include <vector>
#include <coroutine>
#include <chrono>

#include <coro/single_pass.hpp>

#include <boost/coroutine2/coroutine.hpp>

using namespace std;

inline namespace common {

struct Node {
  int value;
  Node* left;
  Node* right;

  Node(int x) : value(x), left(nullptr), right(nullptr) {
  }
};

Node* MakeFullTree(int n, int s = 0) {
  auto p = new Node(s);
  if (n == 1) {
    return p;
  }
  n /= 2;
  p->left = MakeFullTree(n, s + 1);
  p->right = MakeFullTree(n, s + 2 * n);
  return p;
}

}  // namespace common

namespace slow {

using generator = coro::SinglePass<int>;

generator Dfs(Node* p) {
  if (!p) {
    co_return;
  }
  if (!p->left && !p->right) {
    co_yield p->value;
    co_return;
  }
  // not only you alloc, but each leaf will have to
  // propagate through whole stack of co_yield's
  for (int x : Dfs(p->left)) {
    co_yield x;
  }
  for (int x : Dfs(p->right)) {
    co_yield x;
  }
}

}  // namespace slow

static void BM_Slow(benchmark::State& state) {
  auto root = MakeFullTree(state.range(0));
  for (auto _ : state) {
    int res = 0;
    for (int x : slow::Dfs(root)) {
      res += x;
    }
  }
}

namespace {

struct Stack {
  vector<char> buffer;
  size_t end;
  size_t frame_size;

  Stack(size_t size) : buffer(size), end(0), frame_size(0) {
  }

  void* BackAddress() {
    return buffer.data() + end - frame_size;
  }

  void Pop() {
    end -= frame_size;
  }

  void* New(size_t count) {
    frame_size = count;

    end += frame_size;
    return BackAddress();
  }

  bool Empty() {
    return !end;
  }
};

struct Wait {
  bool await_ready() const noexcept {
    return false;
  }

  void await_resume() const noexcept {
  }

  coroutine_handle<> await_suspend(coroutine_handle<> coro) const noexcept {
    return handle;
  }

  coroutine_handle<> handle;
};

struct SingleValue {
  bool ok{false};
  int value{0};

  void Set(int x) {
    ok = true;
    value = x;
  }

  int Pop() {
    ok = false;
    return value;
  }

  bool IsSet() const {
    return ok;
  }
};

struct Task {
  Stack stack;
  SingleValue value;

  bool HasStack();

  auto Yield(int x);

  void Resume();

  Task() : stack(2'000), value() {
  }
};

// thread locals on macOS are extremely slow (x2-x3 slower here)
// thread locals on Linux (tested on ubuntu) are OK though
// https://github.com/rust-lang/rust/issues/60141 looks relevant
static Task* task = new Task();

struct Coro {
  struct promise_type {
   public:
    friend struct Coro;

    // disable HALO
    __attribute__((noinline)) Coro get_return_object() {
      return {std::coroutine_handle<promise_type>::from_promise(*this)};
    }

    std::suspend_always initial_suspend() const {
      return {};
    }

    auto final_suspend() const noexcept {
      auto& stack = task->stack;

      stack.Pop();
      if (stack.Empty()) {
        return Wait{std::noop_coroutine()};
      }
      return Wait{std::coroutine_handle<promise_type>::from_address(
          stack.BackAddress())};
    }

    void unhandled_exception() {
      terminate();
    }

    void return_void() {
    }

    auto yield_value(int x) {
      task->value.Set(x);
      return suspend_always{};
    }

    static void* operator new(size_t count) {
      return task->stack.New(count);
    }

    static void operator delete(void* ptr) {
    }
  };

  using coro_handle = std::coroutine_handle<promise_type>;

  Coro(coro_handle h) : handle(h) {
  }

  ~Coro() {
    if (handle) {
      handle.destroy();
    }
  }

  auto operator co_await() {
    return Wait{handle};
  }

  coro_handle handle{};
};

bool Task::HasStack() {
  return not stack.Empty();
}

void Task::Resume() {
  coroutine_handle<Coro::promise_type>::from_address(stack.BackAddress())
      .resume();
}

Coro CoroDfs(Node* p) {
  if (!p) {
    co_return;
  }
  if (!p->left && !p->right) {
    co_yield p->value;
    co_return;
  }
  co_await CoroDfs(p->left);
  co_await CoroDfs(p->right);
}

}  // namespace

static void BM_Coro(benchmark::State& state) {
  auto root = MakeFullTree(state.range(0));
  for (auto _ : state) {
    int res = 0;
    for (auto call = CoroDfs(root); task->HasStack(); task->Resume()) {
      if (task->value.IsSet()) {
        res += task->value.Pop();
      }
    }
  }
}

namespace fast {

void Dfs(Node* p, auto&& leaf_callback) {
  if (!p) {
    return;
  }
  if (!p->left && !p->right) {
    leaf_callback(p->value);
    return;
  }
  Dfs(p->left, leaf_callback);
  Dfs(p->right, leaf_callback);
}

}  // namespace fast

namespace fiber {

using coro_t = boost::coroutines2::coroutine<int>;

auto Dfs(Node* p) {
  coro_t::pull_type source([p](coro_t::push_type& sink) {
    fast::Dfs(p, sink);
  });
  return source;
}

}  // namespace fiber

static void BM_Fiber(benchmark::State& state) {
  auto root = MakeFullTree(state.range(0));

  for (auto _ : state) {
    int res = 0;
    for (int x : fiber::Dfs(root)) {
      res += x;
    }
  }
}

static void BM_Fast(benchmark::State& state) {
  auto root = MakeFullTree(state.range(0));

  for (auto _ : state) {
    int res = 0;
    fast::Dfs(root, [&res](int x) {
      res += x;
    });
  }
}

BENCHMARK(BM_Slow)   //
    ->Arg(1 << 14)   //
    ->Arg(1 << 20);  //

BENCHMARK(BM_Coro)   //
    ->Arg(1 << 14)   //
    ->Arg(1 << 20)   //
    ->Arg(1 << 22)   //
    ->Arg(1 << 24);  //

BENCHMARK(BM_Fiber)  //
    ->Arg(1 << 14)   //
    ->Arg(1 << 20)   //
    ->Arg(1 << 22)   //
    ->Arg(1 << 24);  //

BENCHMARK(BM_Fast)   //
    ->Arg(1 << 14)   //
    ->Arg(1 << 20)   //
    ->Arg(1 << 22)   //
    ->Arg(1 << 24);  //
