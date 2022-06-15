#pragma once

#include <cstdint>

#include <coro/single_pass.hpp>

namespace bitmask {

/// @brief: subset of [kBits - 1]_0 integers encoded by int32_t
struct Bitmask {
  static constexpr int kBits = 32;

  int mask = 0;

  int Count() const;

  void Add(int key);

  void Del(int key);

  bool Has(int key) const;

  coro::SinglePass<int> KeysLess(int bound) const;

  bool operator==(const Bitmask& other) const noexcept;
};

coro::SinglePass<Bitmask> Subsets(Bitmask m);

bitmask::Bitmask RandomSubset(bitmask::Bitmask mask);

bitmask::Bitmask Full(int len);

}  // namespace bitmask
