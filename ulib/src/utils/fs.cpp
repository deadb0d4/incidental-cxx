#include <utils/fs.hpp>

#include <fstream>

namespace utils {

void CreateIfNotExists(const std::filesystem::path& path) {
  if (std::filesystem::exists(path)) {
    return;
  }
  std::ofstream fin(path);
  fin << std::endl;
}

coro::SinglePass<std::string_view> ReadLines(const std::filesystem::path& p) {
  std::ifstream f(p);
  for (std::string line; std::getline(f, line);) {
    co_yield line;
  }
}

}  // namespace utils
