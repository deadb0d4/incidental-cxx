#include <exec/runner.hpp>

namespace exec {

namespace {

void CreateIfNotExists(const std::filesystem::path& path) {
  if (std::filesystem::exists(path)) {
    return;
  }
  std::ofstream fin(path);
  fin << std::endl;
}

}  // namespace

Runner::Runner(
    std::filesystem::path in,
    std::filesystem::path out,
    std::filesystem::path err)
    : in_file(std::move(in)),
      out_file(std::move(out)),
      err_file(std::move(err)) {
  CreateIfNotExists(in_file);
}

Runner::Runner(const std::filesystem::path& tmp)
    : in_file(tmp / "input.txt"),
      out_file(tmp / "out.txt"),
      err_file(tmp / "err.txt") {
  CreateIfNotExists(in_file);
}

void Runner::SetInput(std::istream& input) const {
  std::ofstream fout(in_file);
  for (std::string line; std::getline(input, line);) {
    fout << line << '\n';
  }
}

}  // namespace exec
