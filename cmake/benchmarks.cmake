set(BENCHMARK_ENABLE_TESTING off)

FetchContent_Declare(googlebenchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_SHALLOW    TRUE
    GIT_PROGRESS   TRUE
    GIT_TAG        master)
FetchContent_MakeAvailable(
    googlebenchmark)

set(BENCHMARK_ENABLE_LTO true)

set(BENCH_DIR "${CMAKE_SOURCE_DIR}/benchmarks")
file(GLOB_RECURSE BENCHS "${BENCH_DIR}/*.cpp")
foreach(i ${BENCHS})
  file(RELATIVE_PATH name ${BENCH_DIR} ${i})
  cmake_path(REMOVE_EXTENSION name)
  string(REPLACE "/" "_" name ${name})
  set(bench_bin "benchmark_${name}")

  add_executable(${bench_bin} ${i})
  target_link_libraries(${bench_bin} benchmark_main ulib)
endforeach()
