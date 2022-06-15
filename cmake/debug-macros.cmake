add_compile_definitions(ULIB_DEBUG_BUILD)
message(STATUS "Turning ON debug logging")

set(DEBUG_LOG_FILENAME "${CMAKE_CURRENT_BINARY_DIR}/debug.log")
add_compile_definitions(ULIB_DEBUG_LOG_FILE="${DEBUG_LOG_FILENAME}")
message(STATUS "Debug log file is ${DEBUG_LOG_FILENAME}")
