#include <debug/to_string.hpp>

namespace debug {

template <>
std::string ToString(const bool& f) {
  return (f ? "true" : "false");
}

}  // namespace debug
