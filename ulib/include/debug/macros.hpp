#pragma once

#include <string>
#include <iostream>
#include <fstream>

namespace debug {

inline constexpr bool Active() {
  // return ULIB_MACRO_DEBUG_ON;
#ifdef ULIB_DEBUG_BUILD
  return true;
#else
  return false;
#endif
}

inline std::ostream& Ostream() {
#ifdef ULIB_DEBUG_LOG_FILE
  static std::ofstream debug_stream(ULIB_DEBUG_LOG_FILE);
#else
  auto& debug_stream = std::cerr;
#endif
  return debug_stream;
}

#define DOUT_IF(on) \
  if (!on)          \
    ;               \
  else              \
    debug::Ostream()

#define DOUT DOUT_IF(debug::Active())

/// @brief: current file as a string
#define DFILE std::string(__FILE__)

/// @brief: current line number as a string
#define DLINE ":" + std::to_string(__LINE__)

}  // namespace debug
