#pragma once

#include <tuple>
#include <coro/single_pass.hpp>

namespace utils {

template <class... Types>
coro::SinglePass<std::tuple<const Types*...>> Zip(
    coro::SinglePass<Types>... gens) {
  while ((!gens.Done() or ...)) {
    co_yield std::make_tuple(gens.Get()...);
    (..., gens.Next());
  }
}

}  // namespace utils
