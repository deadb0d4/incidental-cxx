#pragma once

#include <filesystem>
#include <coro/single_pass.hpp>

namespace utils {

void CreateIfNotExists(const std::filesystem::path& path);

coro::SinglePass<std::string_view> ReadLines(const std::filesystem::path& p);

}  // namespace utils
