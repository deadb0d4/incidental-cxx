execute_process(COMMAND bash -c "./stash/init_main.sh")

add_executable(main main.cc)
target_link_libraries(main ulib)
