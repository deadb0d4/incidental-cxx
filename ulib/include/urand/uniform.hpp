#pragma once

#include <optional>
#include <random>

#include <traits/concepts.hpp>
#include <debug/macros.hpp>

#include <debug/debug.hpp>

namespace urand {

namespace debug {

template <traits::Numerical T>
std::optional<T>& MockedUniformValue() {
  thread_local std::optional<T> value = std::nullopt;

  return value;
}

}  // namespace debug

/// @brief uniform on [min, max]
template <traits::Numerical T>
T Uniform(
    T min = std::numeric_limits<T>::min(),
    T max = std::numeric_limits<T>::max()) {
  if constexpr (::debug::IsActive()) {
    if (auto opt = debug::MockedUniformValue<T>()) {
      return *opt;
    }
  }
  /// TODO: add seed mock
  thread_local int64_t seed =
      std::chrono::steady_clock::now().time_since_epoch().count();
  thread_local std::mt19937 gen(seed);

  if constexpr (std::is_floating_point_v<T>) {
    return std::uniform_real_distribution<T>(min, max)(gen);
  } else if constexpr (std::is_integral_v<T>) {
    return std::uniform_int_distribution<T>(min, max)(gen);
  }
  __builtin_unreachable();
}

}  // namespace urand
