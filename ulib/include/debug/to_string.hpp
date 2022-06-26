#pragma once

#include <sstream>
#include <string>
#include <tuple>
#include <utility>
#include <traits/concepts.hpp>
#include <coro/single_pass.hpp>

namespace debug {

template <class T>
std::string ToString(const T& t);

template <traits::Numerical T>
std::string ToString(const T& t) {
  return std::to_string(t);
}

template <>
std::string ToString(const bool& f);

template <class T>
requires requires(const T& t) {
  std::string(t);
}
std::string ToString(const T& str) {
  return std::string(str);
}

template <traits::Iterable T>
requires(!std::convertible_to<std::string, T>) std::string ToString(const T& t);

template <class... T>
std::string ToString(const std::tuple<T...>& t);

template <
    class T,
    class F = decltype(T().first),
    class S = decltype(T().second)>
requires std::convertible_to<T, std::pair<F, S>> std::string ToString(
    const T& p) {
  return "{" + ToString(p.first) + ", " + ToString(p.second) + '}';
}

namespace impl {

template <class... T>
void _tuple_print(std::ostream& os, const std::tuple<T...>& _tup);

}  // namespace impl

template <class T>
std::string ToString(coro::SinglePass<T>&& t);

template <class... T>
std::string ToString(const std::tuple<T...>& t) {
  std::stringstream ss;
  impl::_tuple_print(ss, t);
  return ss.str();
}

template <traits::Iterable T>
requires(!std::convertible_to<std::string, T>) std::string
    ToString(const T& t) {
  return ToString(coro::Loop(t));
}

template <class T>
std::string ToString(coro::SinglePass<T>&& t) {
  std::string res;
  for (const auto& e : t) {
    if (res.empty()) {
      res = "{";
    } else {
      res += ", ";
    }
    res += ToString(e);
  }
  if (res.empty()) {
    res = "{}";
  } else {
    res += "}";
  }
  return res;
}

namespace impl {

template <class TupType, size_t... I>
void _tuple_print(
    std::ostream& os,
    const TupType& _tup,
    std::index_sequence<I...>) {
  os << '{';
  (..., (os << (I == 0 ? "" : ", ") << ToString(std::get<I>(_tup))));
  os << '}';
}

template <class... T>
void _tuple_print(std::ostream& os, const std::tuple<T...>& _tup) {
  _tuple_print(os, _tup, std::make_index_sequence<sizeof...(T)>());
}

}  // namespace impl

template <class T>
std::string ToString(const T& t) {
  static_assert(sizeof(T) == 0, "failed to specialize");
}

}  // namespace debug
