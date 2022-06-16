#include <color_tty/marker.hpp>

#include <unordered_map>

namespace color_tty {

Marker::Marker(std::vector<Color> colors) : colors_(std::move(colors)) {
}

std::string Marker::Mark(std::string_view str) const {
  static const std::unordered_map<Color, std::string> tags = {
      {Color::kRed, "\033[91m"},
      {Color::kYellow, "\033[33m"},
      {Color::kGreen, "\033[32m"},
      {Color::kCyan, "033[36m"},
      {Color::kMagenta, "\033[35m"},
      {Color::kUnderline, "\033[4m"},
      {Color::kBlink, "\33[5m"},
      {Color::kBold, "\033[1m"},
  };
  static const std::string end_tag = "\033[0m";
  std::string res;
  for (auto c : colors_) {
    res += tags.at(c);
  }
  res += str;
  for (auto _ : colors_) {
    res += end_tag;
  }
  return res;
}

}  // namespace color_tty
