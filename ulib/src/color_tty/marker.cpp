#include <color_tty/marker.hpp>

#include <unordered_map>

namespace color_tty {

std::ostream& operator<<(std::ostream& os, const Marker::Result& ms) {
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

  for (auto c : ms.colors_) {
    os << tags.at(c);
  }
  os << ms.text_;
  for (auto _ : ms.colors_) {
    os << end_tag;
  }
  return os;
}

Marker::Result::Result(std::string text, std::vector<Color> colors)
    : text_(std::move(text)), colors_(std::move(colors)) {
}

Marker::Marker(std::vector<Color> colors) : colors_(std::move(colors)) {
}

Marker::Result Marker::Mark(const std::string& str) {
  return Marker::Result(str, colors_);
}

}  // namespace color_tty
