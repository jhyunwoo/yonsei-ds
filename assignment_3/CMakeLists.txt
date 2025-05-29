cmake_minimum_required(VERSION 3.10)
project(Assignment3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP True
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(runTests test_runner.cc library_code.cc)

target_link_libraries(runTests GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(runTests)
