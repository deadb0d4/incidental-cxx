#include <gtest/gtest.h>

#include <vector>
#include <algorithm>

#include <urand/uniform.hpp>

using namespace std;

namespace {

auto Poll2() {
  vector<bool> got(2, false);
  for (int t = 0; t < 100; ++t) {
    got[urand::Uniform<int>(0, (int)got.size() - 1)] = true;
  }
  return got;
}

}  // namespace

TEST(Uniform, SanityWithoutMock) {
  auto got = Poll2();
  EXPECT_TRUE(all_of(begin(got), end(got), [](bool f) {
    return f;
  })) << "bug OR extremely bad luck";
}

TEST(Uniform, SanityWithMock) {
  urand::debug::MockedUniformValue<int>() = 1;
  auto got = Poll2();

  EXPECT_FALSE(got[0]);
  EXPECT_TRUE(got[1]);
}
