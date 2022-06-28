#include <exec/runner.hpp>

namespace exec {

namespace impl {

OpenedFile::OpenedFile(const char* name, const char* mode)
    : fptr(fopen(name, mode)) {
}

OpenedFile::~OpenedFile() {
  Invalidate();
}

int OpenedFile::Fd() const {
  return fileno(fptr);
}

void OpenedFile::DupTo(int tar) {
  if (dup2(Fd(), tar) == -1) {
    throw std::runtime_error("dup2 error");
  }
  Invalidate();
}

void OpenedFile::Invalidate() {
  if (fptr) {
    fclose(fptr);
  }
  fptr = nullptr;
}

}  // namespace impl

ChildProc::ChildProc(pid_t p) : pid{p} {
}

ChildProc::~ChildProc() {
  if (pid != 0) {
    std::terminate();
  }
}

ChildProc::ChildProc(ChildProc&& proc) noexcept {
  pid = 0;
  std::swap(proc.pid, pid);
}

ChildProc& ChildProc::operator=(ChildProc&& proc) noexcept {
  if (pid != 0) {
    std::terminate();
  }
  std::swap(pid, proc.pid);
  return *this;
}

int ChildProc::BlockingWait() const {
  pid_t p = 0;
  std::swap(p, pid);
  if (p == 0) {
    throw std::runtime_error("wait for zero pid");
  }
  int status;
  if (waitpid(p, &status, 0) == -1) {
    throw std::runtime_error("waitpid error");
  }
  if (WIFEXITED(status)) {
    return WEXITSTATUS(status);
  }
  throw std::runtime_error("!wifexited");
}

void Job::SetInputFile(std::filesystem::path p) {
  in_file = std::move(p);
}

void Job::SetOutFile(std::filesystem::path p) {
  out_file = std::move(p);
}

void Job::SetErrFile(std::filesystem::path p) {
  err_file = std::move(p);
}

}  // namespace exec
