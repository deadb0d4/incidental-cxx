#include <bitmask/bitmask.hpp>

#include <urand/uniform.hpp>
#include <debug/macros.hpp>

namespace bitmask {

int Bitmask::Count() const {
  return __builtin_popcount(mask);
}

void Bitmask::Add(int key) {
  mask |= (1 << key);
}

void Bitmask::Del(int key) {
  mask ^= (mask & (1 << key));
}

bool Bitmask::Has(int key) const {
  return (mask >> key) & 1;
}

coro::SinglePass<int> Bitmask::KeysLess(int bound) const {
  // copy if this is rvalue
  auto copy = *this;
  for (int b = 0; b < bound; ++b) {
    if (copy.Has(b)) {
      co_yield b;
    }
  }
}

bool Bitmask::operator==(const Bitmask& other) const noexcept {
  return mask == other.mask;
}

coro::SinglePass<Bitmask> Subsets(Bitmask m) {
  if (Full(m.Count()) == m) {
    for (int n = 0; n <= m.mask; ++n) {
      co_yield Bitmask{n};
    }
    co_return;
  }
  for (int n = m.mask; n; n = (n - 1) & m.mask) {
    co_yield Bitmask{n};
  }
  co_yield Bitmask{0};
}

bitmask::Bitmask RandomSubset(bitmask::Bitmask mask) {
  auto seed = bitmask::Bitmask{urand::Uniform(0, (1 << mask.Count()) - 1)};
  bitmask::Bitmask res;
  for (int i = 0; auto x : mask.KeysLess(bitmask::Bitmask::kBits)) {
    if (seed.Has(i)) {
      res.Add(x);
    }
    ++i;
  }
  return res;
}

bitmask::Bitmask Full(int len) {
  return {(1 << len) - 1};
}

}  // namespace bitmask
