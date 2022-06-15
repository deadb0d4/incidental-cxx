include(CTest)
enable_testing()

FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG origin/main)
FetchContent_MakeAvailable(
    googletest)

set(UNIT_TEST_DIR "${CMAKE_SOURCE_DIR}/unit-tests")
file(GLOB_RECURSE TESTS "${UNIT_TEST_DIR}/*.cpp")
foreach(i ${TESTS})
  file(RELATIVE_PATH name ${UNIT_TEST_DIR} ${i})
  cmake_path(REMOVE_EXTENSION name)
  string(REPLACE "/" "_" name ${name})
  set(test_bin "test_${name}")

  add_executable(${test_bin} ${i})
  target_link_libraries(${test_bin} gtest_main ulib)
  add_test(NAME ${test_bin} COMMAND ${test_bin})
endforeach()
