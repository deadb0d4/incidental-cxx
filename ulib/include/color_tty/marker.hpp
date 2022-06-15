#pragma once

#include <string>
#include <vector>
#include <ostream>

namespace color_tty {

enum class Color {
  kRed,
  kYellow,
  kGreen,
  kCyan,
  kMagenta,
  kUnderline,
  kBlink,
  kBold,
};

class Marker {
 public:
  class Result {
   public:
    friend std::ostream& operator<<(std::ostream& os, const Result& ms);

   private:
    friend class Marker;

    Result(std::string text, std::vector<Color> colors);

    std::vector<Color> colors_;
    std::string text_;
  };

  Marker(std::vector<Color> colors);

  Result Mark(const std::string& str);

 private:
  std::vector<Color> colors_;
};

}  // namespace color_tty
