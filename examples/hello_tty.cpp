#include <iostream>

#include <color_tty/marker.hpp>

int main() {
  color_tty::Marker greeter{{color_tty::kBlink, color_tty::kMagenta}};

  greeter.On(std::cout) << "Hello, tty" << '\n';
  return 0;
}
