#include <chrono>
#include <debug/lap.hpp>

#include <iostream>

namespace debug {

namespace {
using namespace std::chrono;
}  // namespace

Lap::Lap(std::string name) : begin(steady_clock::now()) {
  if (name.size()) {
    std::cout << "Entering lap: " << name << '\n';
  }
}

Lap::~Lap() noexcept {
  if (not*this) {
    return;
  }
  auto dur = steady_clock::now() - begin;
  std::cout << duration_cast<microseconds>(dur).count() / 1000. << "ms\n";
}

Lap::Lap(Lap&& other) noexcept {
  begin = other.begin;
  other.begin = {};
}

Lap::operator bool() & noexcept {
  return begin != TimePoint{};
}

}  // namespace debug
