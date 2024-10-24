#pragma once

#include <iterator>
#include <concepts>
#include <type_traits>

namespace traits {

template <typename P>
concept IsPointer = std::is_pointer_v<P>;

template <typename R>
concept IsReference = std::is_reference_v<R>;

template <typename S>
concept IsSimple = !IsPointer<S> && !IsReference<S>;

template <class T>
concept Numerical = std::is_floating_point_v<T> || std::is_integral_v<T>;

template <typename T, typename Value>
concept RefersTo =
    std::is_same_v<std::remove_cvref_t<T>, std::remove_cvref_t<Value>>;

template <typename T, typename Value = decltype(*T().begin())>
concept Iterable = requires(T t) {
  { t.begin() };

  { t.end() };

  { *t.begin() } -> RefersTo<Value>;
};

}  // namespace traits
