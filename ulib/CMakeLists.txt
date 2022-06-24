# sources
file(GLOB_RECURSE sources "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_library(ulib ${sources})

# using hash map
target_link_libraries(ulib PUBLIC robin_hood_hashmap)

# using boost
target_include_directories(ulib PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(ulib PUBLIC ${BOOST_CONTEXT_MT})

# using codegen
target_link_libraries(ulib PUBLIC codegen)

# including headers
target_include_directories(ulib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
