#include <utils/str.hpp>

namespace utils {

std::vector<std::string_view> Split(std::string_view str, char delim) {
  std::vector<std::string_view> res;
  for (size_t l = 0; l < str.size();) {
    if (str[l] == delim) {
      ++l;
      continue;
    }
    size_t r = l;
    while (r < str.size() and str[r] != delim) {
      ++r;
    }
    res.push_back(str.substr(l, r - l));
    l = r + 1;
  }
  return res;
}

}  // namespace utils
