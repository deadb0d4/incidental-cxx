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

  class MarkedOstream {
   public:
    ~MarkedOstream();

    friend const MarkedOstream& operator<<(const MarkedOstream& s, auto&& t) {
      s.os_ << std::forward<decltype(t)>(t);
      return s;
    }

   private:
    friend class Marker;

    MarkedOstream(size_t count, std::ostream& os);

    size_t count_;
    std::ostream& os_;
  };

  MarkedOstream On(std::ostream& os) const;

 private:
  std::vector<Color> colors_;
};

}  // namespace color_tty
