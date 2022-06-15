// https://eli.thegreenplace.net/2012/08/02/parsing-expressions-by-precedence-climbing

#include <gtest/gtest.h>

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <unordered_map>

#include <coro/single_pass.hpp>

namespace {

using namespace std;

struct Token {
  variant<int, char> value;

  Token() : value() {
  }

  explicit Token(int x) : value(x) {
  }

  explicit Token(char x) : value(x) {
  }

  bool IsInt() const {
    return value.index() == 0;
  }

  bool IsChar() const {
    return value.index() == 1;
  }

  operator int() const {
    return get<0>(value);
  }

  operator char() const {
    return get<1>(value);
  }
};

coro::SinglePass<Token> Read(const string& str) {
  for (int l = 0; l < str.size();) {
    if (!isdigit(str[l])) {
      if (str[l] != ' ') {
        co_yield Token{str[l]};
      }
      ++l;
      continue;
    }
    int val = 0;
    int r = l;
    while (r < str.size() and isdigit(str[r])) {
      val = 10 * val + (str[r] - '0');
      ++r;
    }
    co_yield Token(val);
    l = r;
  }
}

struct Node {
  Token tok;
  Node* lhs;
  Node* rhs;
};

void Print(Node* node, string& str) {
  if (!node) {
    return;
  }
  if (node->tok.IsInt()) {
    str += to_string((int)node->tok);
    return;
  }
  str += "(";
  Print(node->lhs, str);
  str += (char)node->tok;
  Print(node->rhs, str);
  str += ")";
}

string ToString(Node* node) {
  string res;
  Print(node, res);
  return res;
}

int Apply(char op, int lhs, int rhs) {
  if (op == '+') {
    return lhs + rhs;
  }
  if (op == '-') {
    return lhs - rhs;
  }
  if (op == '*') {
    return lhs * rhs;
  }
  throw logic_error("unknown op: " + string(1, op));
}

int Eval(Node* node) {
  if (!node) {
    throw out_of_range("nullptr in Eval");
  }
  if (node->tok.IsInt()) {
    return node->tok;
  }
  int lhs = Eval(node->lhs);
  int rhs = Eval(node->rhs);
  return Apply(node->tok, lhs, rhs);
}

Node* Compute(coro::SinglePass<Token>& toks, int level = 1);

Node* ComputeNext(coro::SinglePass<Token>& toks) {
  if (toks.Get()->IsInt()) {
    auto p = new Node();
    p->tok = *toks.Get();
    toks.Next();
    p->lhs = p->rhs = nullptr;
    return p;
  }
  assert((char)*toks.Get() == '(');
  toks.Next();
  auto ret = Compute(toks);
  assert((char)*toks.Get() == ')');
  toks.Next();
  return ret;
}

unordered_map<char, int> prec_map = {
    {'+', 1},
    {'-', 1},
    {'*', 2},
    {')', 0},  // hacky
};

unordered_map<char, bool> ass_type_map = {
    {'+', true},
    {'-', true},
    {'*', false},
};

Node* Compute(coro::SinglePass<Token>& toks, int level) {
  auto res = ComputeNext(toks);
  while (1) {
    if (toks.Done()) {
      break;
    }
    const char op = *toks.Get();
    if (prec_map.at(op) < level) {
      break;
    }
    auto p = new Node();
    p->tok = *toks.Get();
    p->lhs = res;
    toks.Next();
    p->rhs = Compute(toks, prec_map.at(op) + ass_type_map.at(op));
    res = p;
  }
  return res;
}

void Check(const string& expr, int expected) {
  auto toks = Read(expr);
  auto node = Compute(toks);
  EXPECT_EQ(Eval(node), expected) << ToString(node);
}

}  // namespace

TEST(Compute, Number) {
  Check("12", 12);
}

TEST(Compute, Simple) {
  Check("1 * 2 * 3 - 4 * 5", -14);
}

TEST(Compute, Brackets) {
  Check("2 * (2 + 3) * 4 - (4 + 5) * 2", 22);
}
