#pragma once

#include <chrono>
#include <string>

namespace debug {

class [[nodiscard]] Lap {
 private:
  using TimePoint = std::chrono::steady_clock::time_point;

  TimePoint begin;

 public:
  Lap(std::string name = {});

  ~Lap() noexcept;

  Lap(const Lap&) = delete;

  Lap& operator=(const Lap&) = delete;

  Lap(Lap&& other) noexcept;

  operator bool() & noexcept;
};

}  // namespace debug
