#include <gtest/gtest.h>

#include <vector>
#include <string_view>

#include <argparse/parsers/unit_test1.hpp>
#include <argparse/parsers/unit_test2.hpp>
#include <argparse/parsers/unit_test3.hpp>

namespace {
using namespace std;
}  // namespace

TEST(UnitTest1, OnlyRequired) {
  // no args is OK for bools
  vector<string_view> args = {};
  auto parsed = argparse::parsers::UnitTest1(args);
  EXPECT_EQ(parsed.explicit_false, false);
  EXPECT_EQ(parsed.implicit_false, false);
  EXPECT_EQ(parsed.explicit_true, true);
  EXPECT_TRUE(parsed.positionals.empty());
}

TEST(UnitTest1, Smoke) {
  // simple input
  vector<string_view> args = {"-c", "--explicit_false"};
  auto parsed = argparse::parsers::UnitTest1(args);
  EXPECT_EQ(parsed.explicit_false, true);
  EXPECT_EQ(parsed.implicit_false, true);
  EXPECT_EQ(parsed.explicit_true, true);
  EXPECT_TRUE(parsed.positionals.empty());
}

TEST(UnitTest1, OnePositional) {
  // one positional
  vector<string_view> args = {"-c", "positional", "--explicit_false"};
  auto parsed = argparse::parsers::UnitTest1(args);
  EXPECT_EQ(parsed.explicit_false, true);
  EXPECT_EQ(parsed.implicit_false, true);
  EXPECT_EQ(parsed.explicit_true, true);
  EXPECT_EQ(parsed.positionals, (vector<string_view>{"positional"}));
}

TEST(UnitTest2, OnlyRequired) {
  // only required
  vector<string_view> args = {"-b", "1337", "kekw"};
  auto parsed = argparse::parsers::UnitTest2(args);
  EXPECT_EQ(parsed.flag1, 133);      // default
  EXPECT_EQ(parsed.flag2, nullopt);  // optional
  EXPECT_EQ(parsed.flag3, 1337);
  EXPECT_EQ(parsed.positionals, (vector<string_view>{"kekw"}));
}

TEST(UnitTest2, AllArguments) {
  // --flag1 is a short option for --flag1 1
  // -bbbbb is the append feature for int flags
  vector<string_view> args = {"--flag1", "--flag2", "10", "kekw", "-bbbbb"};
  auto parsed = argparse::parsers::UnitTest2(args);
  EXPECT_EQ(parsed.flag1, 1);   // default
  EXPECT_EQ(parsed.flag2, 10);  // optional
  EXPECT_EQ(parsed.flag3, 5);
  EXPECT_EQ(parsed.positionals, (vector<string_view>{"kekw"}));
}

TEST(UnitTest3, NoArgs) {
  vector<string_view> args = {};
  auto parsed = argparse::parsers::UnitTest3(args);
  EXPECT_EQ(parsed.int_list, nullopt);
  EXPECT_EQ(parsed.str_flag, nullopt);
  EXPECT_EQ(parsed.str_list, (vector<string_view>{"kek", "lol"}));
  EXPECT_TRUE(parsed.positionals.empty());
}

TEST(UnitTest3, Smoke) {
  // imitate `-a --int_list {1,-2, } positional` from tty
  // -a w/o value implies "" for str
  vector<string_view> args = {"-a", "--int_list", "1", "-2", "", "positional"};
  auto parsed = argparse::parsers::UnitTest3(args);
  EXPECT_EQ(parsed.int_list, (vector<int>{1, -2}));
  EXPECT_EQ(parsed.str_flag, "");
  EXPECT_EQ(parsed.str_list, (vector<string_view>{"kek", "lol"}));
  EXPECT_EQ(parsed.positionals, (vector<string_view>{"positional"}));
}

TEST(UnitTest3, Smoke2) {
  vector<string_view> args = {
      "-a", "kek", "positional", "--str_list", "x", "y", "t"};
  auto parsed = argparse::parsers::UnitTest3(args);
  EXPECT_EQ(parsed.int_list, nullopt);
  EXPECT_EQ(parsed.str_flag, "kek");
  EXPECT_EQ(parsed.str_list, (vector<string_view>{"x", "y", "t"}));
  EXPECT_EQ(parsed.positionals, (vector<string_view>{"positional"}));
}
