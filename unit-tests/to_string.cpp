#include <gtest/gtest.h>

#include <type_traits>
#include <concepts>
#include <string>
#include <debug/to_string.hpp>
#include <coro/single_pass.hpp>
#include <uhash/map.hpp>

namespace {

template <typename T>
concept HasToString = requires(const T& t) {
  { debug::ToString(t) } -> std::same_as<std::string>;
};

using namespace std;

template <HasToString T>
void Check(const T& t, string expected) {
  EXPECT_EQ(debug::ToString(t), expected);
}

coro::SinglePass<int> Iota(int start, int len) {
  for (int i = 0; i < len; ++i) {
    co_yield (start + i);
  }
}

}  // namespace

TEST(ToString, Primitives) {
  {
    Check(1337, "1337");
  }
  {
    Check(true, "true");
    Check(false, "false");
  }
  {
    string str = "lulw";
    Check(str, str);
  }
}

TEST(ToString, Tuples) {
  {
    tuple<int, bool, string> t = {
        1337,
        false,
        "kekw",
    };
    Check(t, "{1337, false, kekw}");
  }
  {
    pair<string, tuple<int, bool>> p = {"kekw", {18, true}};
    // Check(p, "{kekw, {18, true}}");
  }
}

TEST(ToString, SinglePass) {
  // generator is special because it will be modified
  // and method implicitly implies it
  EXPECT_EQ(debug::ToString(Iota(1, 5)), "{1, 2, 3, 4, 5}");
}

TEST(ToString, Containers) {
  {
    vector<pair<int, string>> vec = {
        {1, "one"},
        {2, "two"},
    };
    Check(vec, "{{1, one}, {2, two}}");
  }
  {
    uhash::Map<pair<int, int>, int> mp = {{{1, 2}, 3}};
    Check(mp, "{{{1, 2}, 3}}");
  }
}
