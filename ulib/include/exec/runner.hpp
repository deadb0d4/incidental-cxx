#pragma once

#include <filesystem>
#include <fstream>
#include <string>
#include <tuple>
#include <type_traits>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

namespace exec {

namespace impl {
struct OpenedFile {
  OpenedFile(const char* name, const char* mode) : fptr(fopen(name, mode)) {
  }

  ~OpenedFile() {
    Invalidate();
  }

  int Fd() const {
    return fileno(fptr);
  }

  void DupTo(int tar) {
    if (dup2(Fd(), tar) == -1) {
      throw std::runtime_error("dup2 error");
    }
    Invalidate();
  }

  void Invalidate() {
    if (fptr) {
      fclose(fptr);
    }
    fptr = nullptr;
  }

  FILE* fptr;
};
}  // namespace impl

template <std::convertible_to<std::string>... T>
auto Cmd(T&&... t) {
  return std::make_tuple(std::string(std::forward<T>(t))...);
}

template <class T>
concept String = std::is_same_v<T, std::string>;

/// @brief: this class captures input, output and error text files.
/// Then, it binds `std{in,out,err}` to these files and runs executables
///
/// TODO: Separate waiting and starting job
struct Runner {
  const std::filesystem::path in_file;
  const std::filesystem::path out_file;
  const std::filesystem::path err_file;

  /// @brief set filenames manually
  Runner(
      std::filesystem::path in,
      std::filesystem::path out,
      std::filesystem::path err);

  /// @brief initializes files with `tmp` / `{input,out,err}.txt`
  Runner(const std::filesystem::path& tmp = "/tmp");

  /// @brief writes content of `input` line-by-line to the input file
  void SetInput(std::istream& input) const;

  /// @brief run a command with flags specified by `args`
  template <String... T>
  int Run(const std::tuple<T...>& args) const {
    auto pid = fork();
    if (pid == -1) {
      throw std::runtime_error("fork error");
    }
    if (pid == 0) {
      {
        impl::OpenedFile(in_file.c_str(), "r").DupTo(STDIN_FILENO);
        impl::OpenedFile(out_file.c_str(), "w").DupTo(STDOUT_FILENO);
        impl::OpenedFile(err_file.c_str(), "w").DupTo(STDERR_FILENO);
      }
      std::apply(
          [name = std::get<0>(args).c_str()](const auto&... t) {
            execl(name, t.c_str()..., (char*)0);
          },
          args);
      exit(1);
    }
    int status;
    if (waitpid(pid, &status, 0) == -1) {
      throw std::runtime_error("waitpid error");
    }
    if (WIFEXITED(status)) {
      return WEXITSTATUS(status);
    }
    throw std::runtime_error("!wifexited");
  }
};

}  // namespace exec
