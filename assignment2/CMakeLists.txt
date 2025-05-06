cmake_minimum_required(VERSION 3.10)
project(Assignment1)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(runTests test_runner.cc)
target_link_libraries(runTests gtest_main pthread)

include(GoogleTest)
gtest_discover_tests(runTests)
