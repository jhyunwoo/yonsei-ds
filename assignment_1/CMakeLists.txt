cmake_minimum_required(VERSION 3.10)
project(Assignment1)

set(CMAKE_CXX_STANDARD 17)

# Add GoogleTest (if installed via package manager)
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Add your sources
add_executable(runTests test_runner.cc)

# Link libraries
target_link_libraries(runTests ${GTEST_LIBRARIES} pthread)
