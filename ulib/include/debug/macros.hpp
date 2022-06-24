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

/// @brief :<LINE>
///
/// beware name collisions
#define d0ut DOUT << ':' << __LINE__ << ' '
