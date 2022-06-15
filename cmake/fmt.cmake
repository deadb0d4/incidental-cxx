set(FMT_TEST FF CACHE INTERNAL "Disabling fmt tests")
FetchContent_Declare(
        fmt
        GIT_REPOSITORY  https://github.com/fmtlib/fmt.git
        GIT_TAG         origin/master)
# one option for target is fmt::fmt
FetchContent_MakeAvailable(fmt)
