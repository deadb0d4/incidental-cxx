#include <utils/dumb_arg_parser.hpp>

#include <utils/str.hpp>

namespace utils {

ArgParser::ArgParser(int argc, char* argv[]) {
  for (int i = 1; i < argc; ++i) {
    auto tok = std::string_view(argv[i]);
    if (tok[0] != '-') {
      args.emplace_back(tok);
      continue;
    }
    auto got = Split(tok.substr(1), '=');
    if (auto [_, ok] = kwargs.insert(ParseValue(got)); not ok) {
      throw std::runtime_error("duplicate kwargs: " + std::string(tok));
    }
  }
}

std::pair<std::string, ArgParser::OptionValue> ArgParser::ParseValue(
    const std::vector<std::string_view>& got) {
  if (got.size() > 1) {
    std::string res;
    for (int i = 1; i < got.size(); ++i) {
      if (res.size()) {
        res += '=';
      }
      res += got[i];
    }
    return make_pair(std::string(got[0]), res);
  }
  if (auto tok = got[0]; isdigit(tok.back())) {
    std::string key;
    for (int pos = 0; pos < tok.size(); ++pos) {
      if (!isalpha(tok[pos])) {
        return make_pair(std::move(key), stoi(std::string(tok.substr(pos))));
      }
      key += tok[pos];
    }
  }
  return make_pair(std::string(got[0]), true);
}

}  // namespace utils
