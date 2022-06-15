#include <gtest/gtest.h>

#include <exception>
#include <stdexcept>
#include <string>
#include <memory>

#include <coro/single_pass.hpp>

namespace {

template <class T>
using Gen = coro::SinglePass<T>;

Gen<std::unique_ptr<std::string>> Greet() {
  for (auto w : {"hello", "world"}) {
    auto ptr = std::make_unique<std::string>(w);
    co_yield ptr;
  }
}

}  // namespace

TEST(Generator, Simple) {
  std::string res;
  for (auto&& w : Greet()) {
    res += *w;
  }
  ASSERT_EQ(res, "helloworld");
}

TEST(Generator, Empty) {
  auto empty = []() -> Gen<std::string> {
    co_return;
  };
  std::string res;
  for (auto&& w : empty()) {
    res += w;
  }
  EXPECT_TRUE(res.empty());
}

TEST(Generator, Exception) {
  auto error_gen = []() -> Gen<std::string> {
    co_yield "hello";
    throw std::runtime_error("world");
  };
  std::string res;
  try {
    for (auto&& w : error_gen()) {
      res += w;
    }
  } catch (const std::runtime_error& exc) {
    res = exc.what();
  }
  ASSERT_EQ(res, "world");
}

TEST(Generator, EmptyException) {
  auto error_gen = []() -> Gen<std::string> {
    throw std::runtime_error("world");
    co_return;
  };
  std::string res;
  try {
    for (auto&& w : error_gen()) {
      res += w;
    }
  } catch (const std::runtime_error& exc) {
    res = exc.what();
  }
  ASSERT_EQ(res, "world");
}

TEST(Generator, Composition) {
  std::vector<int> vec = {1, 2, 3};
  for (int i = 0; auto x : coro::Loop(vec)) {
    ASSERT_EQ(i + 1, x);
    ++i;
  }
}
