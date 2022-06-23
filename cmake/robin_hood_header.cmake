FetchContent_Declare(robin_hood_hashmap
    GIT_REPOSITORY https://github.com/deadb0d4/robin-hood-header.git
    GIT_TAG        origin/develop
    GIT_SHALLOW    TRUE
    GIT_PROGRESS   TRUE)


FetchContent_MakeAvailable(
    robin_hood_hashmap)
