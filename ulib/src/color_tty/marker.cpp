#include <color_tty/marker.hpp>

#include <unordered_map>

namespace color_tty {

namespace {

static const std::unordered_map<Color, std::string> kTags = {
    {Color::kRed, "\033[91m"},
    {Color::kYellow, "\033[33m"},
    {Color::kGreen, "\033[32m"},
    {Color::kCyan, "\033[36m"},
    {Color::kMagenta, "\033[35m"},
    {Color::kUnderline, "\033[4m"},
    {Color::kBlink, "\33[5m"},
    {Color::kBold, "\033[1m"},
};

constexpr const auto* const kEndTag = "\033[0m";

}  // namespace

Marker::Marker(std::vector<Color> colors) : colors_(std::move(colors)) {
}

std::string Marker::Mark(std::string_view str) const {
  std::string res;
  for (auto c : colors_) {
    res += kTags.at(c);
  }
  res += str;
  for (auto _ : colors_) {
    res += kEndTag;
  }
  return res;
}

Marker::MarkedOstream::MarkedOstream(size_t count, std::ostream& os)
    : count_(count), os_(os) {
}

Marker::MarkedOstream::~MarkedOstream() {
  while (count_--) {
    os_ << kEndTag;
  }
}

Marker::MarkedOstream Marker::On(std::ostream& os) const {
  for (auto c : colors_) {
    os << kTags.at(c);
  }
  return MarkedOstream(colors_.size(), os);
}

}  // namespace color_tty
