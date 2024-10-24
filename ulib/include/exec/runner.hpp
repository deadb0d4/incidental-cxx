#pragma once

#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <optional>
#include <type_traits>
#include <memory>

#include <utils/fs.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

namespace exec {

namespace impl {

class OpenedFile {
 public:
  OpenedFile(const char* name, const char* mode);

  ~OpenedFile();

  int Fd() const;

  void DupTo(int tar);

  void Invalidate();

 private:
  FILE* fptr;
};

}  // namespace impl

class ChildProc {
 public:
  explicit ChildProc(pid_t p);

  ~ChildProc();

  ChildProc(const ChildProc&) = delete;

  ChildProc& operator=(const ChildProc&) = delete;

  ChildProc(ChildProc&& proc) noexcept;

  ChildProc& operator=(ChildProc&& proc) noexcept;

  int BlockingWait() const;

 private:
  mutable pid_t pid;
};

struct Job {
  std::optional<std::filesystem::path> in_file{};
  std::optional<std::filesystem::path> out_file{};
  std::optional<std::filesystem::path> err_file{};

  virtual ~Job() = default;

  void SetInputFile(std::filesystem::path p);

  void SetOutFile(std::filesystem::path p);

  void SetErrFile(std::filesystem::path p);

  [[nodiscard]] virtual ChildProc Run() const = 0;
};

template <class TupleType>
class CmdJob final : public Job {
 public:
  CmdJob(TupleType t) : args(std::move(t)) {
  }

  [[nodiscard]] virtual ChildProc Run() const override {
    auto pid = fork();
    if (pid == -1) {
      throw std::runtime_error("fork error");
    }
    if (pid == 0) {
      if (in_file) {
        utils::CreateIfNotExists(*in_file);
        impl::OpenedFile(in_file->c_str(), "r").DupTo(STDIN_FILENO);
      }
      if (out_file) {
        impl::OpenedFile(out_file->c_str(), "w").DupTo(STDOUT_FILENO);
      }
      if (err_file) {
        impl::OpenedFile(err_file->c_str(), "w").DupTo(STDERR_FILENO);
      }
      std::apply(
          [name = std::get<0>(args).c_str()](const auto&... t) {
            execl(name, t.c_str()..., (char*)0);
          },
          args);
      exit(1);
    }
    return ChildProc{pid};
  }

 private:
  const TupleType args;
};

template <std::convertible_to<std::string>... T>
auto Cmd(T&&... t) {
  auto args = std::make_tuple(std::string(std::forward<T>(t))...);
  return std::make_unique<CmdJob<decltype(args)>>(std::move(args));
}

}  // namespace exec
