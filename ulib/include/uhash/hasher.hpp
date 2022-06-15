#pragma once

#include <functional>
#include <type_traits>
#include <traits/concepts.hpp>
#include <boost/functional/hash.hpp>

namespace uhash {

namespace traits {

template <typename T>
concept HasStdHash = requires(const T& t, std::hash<T> h) {
  { h(t) } -> std::same_as<size_t>;
};

}  // namespace traits

struct Hasher {
 private:
  template <class TupType, size_t... I>
  size_t HashTuple(const TupType& _tup, std::index_sequence<I...>) const {
    size_t seed = 0;
    (..., boost::hash_combine(seed, std::get<I>(_tup)));
    return seed;
  }

 public:
  template <class T>
  size_t operator()(const T& t) const;

  template <traits::HasStdHash T>
  size_t operator()(const T& t) const {
    return std::hash<T>()(t);
  }

  template <::traits::IsSimple F, ::traits::IsSimple S>
  size_t operator()(const std::pair<F, S>& p) const {
    size_t seed = 0;
    boost::hash_combine(seed, (*this)(p.first));
    boost::hash_combine(seed, (*this)(p.second));
    return seed;
  }

  template <::traits::IsSimple... T>
  size_t operator()(const std::tuple<T...>& t) const {
    return HashTuple(t, std::make_index_sequence<sizeof...(T)>());
  }
};

template <class T>
size_t Hasher::operator()(const T& t) const {
  static_assert(sizeof(T) == 0, "failed to specialize");
}

}  // namespace uhash
