#pragma once

#include <vector>
#include <string>
#include <string_view>

namespace utils {

std::vector<std::string_view> Split(std::string_view str, char delim);

template <class T>
std::string Join(T&& t, std::string_view delim) {
  std::string res;
  for (auto&& e : t) {
    if (res.size()) {
      res += delim;
    }
    res += e;
  }
  return res;
}

}  // namespace utils
