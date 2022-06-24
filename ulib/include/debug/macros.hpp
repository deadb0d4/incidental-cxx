#pragma once

#include <string>

#include <debug/debug.hpp>

namespace debug::impl {

class OutCatcher {
 public:
  inline OutCatcher(std::ostream& os) : os_(os) {
  }

  inline ~OutCatcher() {
    os_ << "\n";
  }

  friend const OutCatcher& operator<<(const OutCatcher& s, auto&& t) {
    s.os_ << std::forward<decltype(t)>(t);
    return s;
  }

 private:
  std::ostream& os_;
};

}  // namespace debug::impl

#define DOUT_IF(on) \
  if (!on)          \
    ;               \
  else              \
    debug::impl::OutCatcher(debug::Ostream())

#define DOUT DOUT_IF(debug::IsActive())

#define d0ut DOUT << __FILE__ << ':' << __LINE__ << '\n'\
