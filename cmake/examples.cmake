set(EXAMPLES_DIR "${CMAKE_SOURCE_DIR}/examples")

file(GLOB_RECURSE EXAMPLES
  "${EXAMPLES_DIR}/*.cc"
  "${EXAMPLES_DIR}/*.cpp")

foreach(i ${EXAMPLES})
  file(RELATIVE_PATH name ${EXAMPLES_DIR} ${i})
  cmake_path(REMOVE_EXTENSION name)
  string(REPLACE "/" "_" name ${name})
  set(example_bin "example_${name}")

  add_executable(${example_bin} ${i})
  target_link_libraries(${example_bin} ulib)
endforeach()
