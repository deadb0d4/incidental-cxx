project("incidental-cxx")

cmake_minimum_required(VERSION 3.22)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

if (ASAN)
  include(cmake/asan.cmake)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Release")
  include(cmake/benchmarks.cmake)
  include(cmake/release.cmake)
elseif (CMAKE_BUILD_TYPE STREQUAL "Debug")
  include(cmake/unit-tests.cmake)
  include(cmake/debug-flags.cmake)
endif()

# codegen run
include(cmake/codegen.cmake)

add_subdirectory(ulib)

# user-defined main
include(cmake/main.cmake)

# examples
include(cmake/examples.cmake)
