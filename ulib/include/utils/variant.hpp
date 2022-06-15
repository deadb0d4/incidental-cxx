#pragma once

#include <variant>
#include <type_traits>

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

template <typename... Types>
class Variant {
 public:
  Variant() : value() {
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
  bool Is() const {
    return value.index() == impl::GetVariantIndex<T, V, 0>();
  }

  template <typename T>
  T& Get() {
    return get<impl::GetVariantIndex<T, V, 0>()>(value);
  }

 private:
  using V = std::variant<Types...>;

  V value;
};

}  // namespace utils
