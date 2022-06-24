################################################################################

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    FetchContent_Declare(googlebenchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_SHALLOW    TRUE
        GIT_PROGRESS   TRUE
        GIT_TAG        master)
        FetchContent_MakeAvailable(googlebenchmark)
    set(BENCHMARK_ENABLE_TESTING OFF  CACHE BOOL "" FORCE)
    set(BENCHMARK_ENABLE_LTO     TRUE)
endif()

################################################################################

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_SHALLOW    TRUE
        GIT_PROGRESS   TRUE
        GIT_TAG        origin/main)
    FetchContent_MakeAvailable(googletest)
endif()

################################################################################

find_package(Boost 1.74.0 REQUIRED)
find_library(BOOST_CONTEXT_MT boost_context-mt HINTS ${Boost_LIBRARY_DIRS} REQUIRED)

################################################################################

FetchContent_Declare(robin_hood_hashmap
    GIT_REPOSITORY https://github.com/deadb0d4/robin-hood-header.git
    GIT_TAG        origin/develop
    GIT_SHALLOW    TRUE
    GIT_PROGRESS   TRUE)
FetchContent_MakeAvailable(
    robin_hood_hashmap)

################################################################################

# # NOTE: Currently unused

# set(EIGEN_BUILD_DOC OFF)
# set(BUILD_TESTING   OFF)
# set(EIGEN_BUILD_PKGCONFIG OFF)
# FetchContent_Declare(
#   Eigen
#   GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
#   GIT_TAG        master
#   GIT_SHALLOW    TRUE
#   GIT_PROGRESS   TRUE)
# FetchContent_MakeAvailable(Eigen)

################################################################################

# # NOTE: Currently unused

# set(FMT_TEST OFF CACHE INTERNAL "Disabling fmt tests" FORCE)
# FetchContent_Declare(
#         fmt
#         GIT_REPOSITORY https://github.com/fmtlib/fmt.git
#         GIT_SHALLOW    TRUE
#         GIT_PROGRESS   TRUE
#         GIT_TAG        origin/master)
# # one option for target is fmt::fmt
# FetchContent_MakeAvailable(fmt)
