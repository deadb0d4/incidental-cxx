#include <debug/lap.hpp>

#include <iostream>

namespace debug {

Lap::Lap(std::string name) : begin(std::chrono::steady_clock::now()) {
  if (name.size()) {
    std::cout << "Entering lap: " << name << '\n';
  }
}

Lap::~Lap() noexcept {
  if (not *this) {
    return;
  }
  auto dur = std::chrono::steady_clock::now() - begin;
  std::cout
      << std::chrono::duration_cast<std::chrono::milliseconds>(dur).count()
      << "ms\n";
}

Lap::Lap(Lap&& other) noexcept {
  begin = other.begin;
  other.begin = {};
}

Lap::operator bool() & noexcept {
  return begin != TimePoint{};
}

}  // namespace debug
