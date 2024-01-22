# Load GoogleTest
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

add_executable(slot_map_test slot_map_test.cpp)

target_link_libraries(slot_map_test
    GTest::GTest
    slot_map)

add_test(example_gtest slot_map_test)
