#include <gtest/gtest.h>

#include <string>

#include <debug/macros.hpp>

namespace {

void DontCallMe() {
  ASSERT_TRUE(false);
}

/// somehow only indirect call compiles
int Value() {
  DontCallMe();
  return 1;
}

}  // namespace

TEST(DebugLog, Lazy) {
  DOUT_IF(false) << Value();
}
