#pragma once

#include <string_view>
#include <vector>
#include <string>
#include <unordered_map>

#include <utils/variant.hpp>

namespace utils {
/// @brief: Simple command line args parser
///
/// int:        -[name][value]
/// bool:       -[name] (or nothing)
/// str:        -[name]=[value]
/// positional (string): [value] (w/o `-` in the beginning)
class ArgParser {
 public:
  using OptionValue = utils::Variant<std::string, int, bool>;

  std::vector<std::string> args;
  std::unordered_map<std::string, OptionValue> kwargs;

  ArgParser(int argc, char* argv[]);

 private:
  std::pair<std::string, OptionValue> ParseValue(
      const std::vector<std::string_view>& got);
};

}  // namespace utils
