#pragma once

#include <string>
#include <iostream>
#include <fstream>

#include <debug/debug.hpp>

#define DOUT_IF(on) \
  if (!on)          \
    ;               \
  else              \
    debug::Ostream()
#define DOUT DOUT_IF(debug::IsActive())

/// @brief: current file as a string
#define DFILE std::string(__FILE__)

/// @brief: current line number as a string
#define DLINE ":" + std::to_string(__LINE__)
