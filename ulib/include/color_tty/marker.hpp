#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <ostream>

namespace color_tty {

enum Color {
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
  Marker(std::vector<Color> colors);

  std::string Mark(std::string_view str) const;

 private:
  std::vector<Color> colors_;
};

}  // namespace color_tty
