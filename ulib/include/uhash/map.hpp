#pragma once

// kinda risky to use it, but let's give it a try
#include <robin_hood/robin_hood.h>

#include <uhash/hasher.hpp>

namespace uhash {

template <class K, class V>
using Map = robin_hood::unordered_map<K, V, uhash::Hasher>;

template <class K>
using Set = robin_hood::unordered_set<K, uhash::Hasher>;

}  // namespace uhash
