file(GLOB_RECURSE sources "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_library(ulib ${sources})

include("${CMAKE_SOURCE_DIR}/cmake/robin_hood_header.cmake")
target_link_libraries(ulib PUBLIC robin_hood_hashmap)

include("${CMAKE_SOURCE_DIR}/cmake/eigen.cmake")
target_link_libraries(ulib PUBLIC Eigen3::Eigen)

include("${CMAKE_SOURCE_DIR}/cmake/boost.cmake")
target_include_directories(ulib PUBLIC ${Boost_INCLUDE_DIRS})

find_library(BOOST_CONTEXT_MT boost_context-mt HINTS ${Boost_LIBRARY_DIRS} REQUIRED)
target_link_libraries(ulib PUBLIC ${BOOST_CONTEXT_MT})

target_include_directories(ulib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
