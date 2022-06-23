set(CODEGEN_FILES_DIR "${CMAKE_BINARY_DIR}/code-generated")

message(STATUS "Running codegen in ${CODEGEN_FILES_DIR}")

execute_process(COMMAND bash -c "python3 stash/run_codegen.py \
--codegen_build_dir=${CODEGEN_FILES_DIR} \
--build_type=${CMAKE_BUILD_TYPE}")

file(GLOB_RECURSE sources "${CODEGEN_FILES_DIR}/*.cpp")
add_library(codegen ${sources})
target_include_directories(codegen PUBLIC "${CODEGEN_FILES_DIR}")

execute_process(COMMAND bash -c "grep \
-Ril \
--include=\*.{cpp,hpp} \
'' \
${CODEGEN_FILES_DIR} \
| xargs -L 1 \
clang-format -i")
