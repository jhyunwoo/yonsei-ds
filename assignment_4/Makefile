# Makefile for Assignment 4 tests

CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra
LDFLAGS = -pthread

# Google Test flags (if installed system-wide)
GTEST_FLAGS = -lgtest -lgtest_main

# Targets
all: stress_test edge_case_tests

# Stress test executable
stress_test: stress_test.cc library_code.cc
	$(CXX) $(CXXFLAGS) -o stress_test stress_test.cc $(LDFLAGS)

# Edge case tests with Google Test
edge_case_tests: edge_case_tests.cc library_code.cc
	$(CXX) $(CXXFLAGS) -o edge_case_tests edge_case_tests.cc $(GTEST_FLAGS) $(LDFLAGS)

# Run stress tests
run_stress: stress_test
	./stress_test

# Run edge case tests
run_edge: edge_case_tests
	./edge_case_tests

# Clean build artifacts
clean:
	rm -f stress_test edge_case_tests

.PHONY: all clean run_stress run_edge
