#pragma once

#include <variant>
#include <type_traits>

#include <utils/none.hpp>

namespace utils {

namespace impl {

template <class T, class Variant, size_t i>
constexpr size_t GetVariantIndex() {
  if constexpr (std::is_same_v<T, std::variant_alternative_t<i, Variant>>) {
    return i;
  }
  if constexpr (std::variant_size_v < Variant >> i + 1) {
    return GetVariantIndex<T, Variant, i + 1>();
  }
  return std::variant_size_v<Variant>;
}

}  // namespace impl

/// @brief: variant sugar for non-duplicate types
template <typename... Types>
class Variant {
 private:
  using V = std::variant<None, Types...>;

 public:
  Variant() : value(None{}) {
  }

  template <typename T>
  Variant(T t) : value(std::move(t)) {
  }

  template <typename T>
  Variant<Types...>& operator=(T t) {
    value = std::move(t);
    return *this;
  }

  template <typename T>
  T* Get() {
    return get_if<impl::GetVariantIndex<T, V, 0>()>(&value);
  }

  template <typename Fn>
  void Apply(Fn&& fn) {
    std::visit(
        [vis = std::forward<Fn>(fn)](const auto& arg) {
          using T = std::decay_t<decltype(arg)>;
          if constexpr (std::is_same_v<T, None>) {
            return;
          } else {
            vis(arg);
          }
        },
        value);
  }

 private:
  V value;
};

}  // namespace utils
