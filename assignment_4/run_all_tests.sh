#!/bin/bash

# run_all_tests.sh - Comprehensive test runner for Assignment 4

echo "========================================"
echo "Data Structure Assignment 4 - Test Suite"
echo "========================================"
echo ""

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Function to run a command and check its exit status
run_test() {
    local test_name=$1
    local command=$2
    
    echo -e "${YELLOW}Running $test_name...${NC}"
    if eval $command; then
        echo -e "${GREEN}✓ $test_name passed${NC}"
        return 0
    else
        echo -e "${RED}✗ $test_name failed${NC}"
        return 1
    fi
}

# Check if we're in the correct directory
if [ ! -f "library_code.cc" ]; then
    echo -e "${RED}Error: library_code.cc not found. Please run this script from the assignment_4 directory.${NC}"
    exit 1
fi

# Build tests using CMake
echo -e "${YELLOW}Building tests with CMake...${NC}"
if [ ! -d "build" ]; then
    mkdir build
fi

cd build
cmake .. > /dev/null 2>&1
if [ $? -ne 0 ]; then
    echo -e "${RED}CMake configuration failed${NC}"
    cd ..
    exit 1
fi

make > /dev/null 2>&1
if [ $? -ne 0 ]; then
    echo -e "${RED}Build failed${NC}"
    cd ..
    exit 1
fi
cd ..

echo -e "${GREEN}Build successful${NC}\n"

# Run original test suite
echo "========================================"
echo "Running Original Test Suite"
echo "========================================"
cd build
./runTests
original_result=$?
cd ..
echo ""

# Build and run stress tests
echo "========================================"
echo "Running Stress Tests"
echo "========================================"
make -f Makefile stress_test > /dev/null 2>&1
if [ $? -eq 0 ]; then
    ./stress_test
    stress_result=$?
else
    echo -e "${RED}Failed to build stress tests${NC}"
    stress_result=1
fi
echo ""

# Build and run edge case tests (if gtest is available)
echo "========================================"
echo "Running Edge Case Tests"
echo "========================================"
make -f Makefile edge_case_tests > /dev/null 2>&1
if [ $? -eq 0 ]; then
    ./edge_case_tests
    edge_result=$?
else
    echo -e "${YELLOW}Google Test not found, skipping edge case tests${NC}"
    edge_result=0
fi
echo ""

# Summary
echo "========================================"
echo "Test Summary"
echo "========================================"

total_tests=0
passed_tests=0

if [ $original_result -eq 0 ]; then
    echo -e "${GREEN}✓ Original tests passed${NC}"
    ((passed_tests++))
else
    echo -e "${RED}✗ Original tests failed${NC}"
fi
((total_tests++))

if [ $stress_result -eq 0 ]; then
    echo -e "${GREEN}✓ Stress tests passed${NC}"
    ((passed_tests++))
else
    echo -e "${RED}✗ Stress tests failed${NC}"
fi
((total_tests++))

if [ -f "./edge_case_tests" ]; then
    if [ $edge_result -eq 0 ]; then
        echo -e "${GREEN}✓ Edge case tests passed${NC}"
        ((passed_tests++))
    else
        echo -e "${RED}✗ Edge case tests failed${NC}"
    fi
    ((total_tests++))
fi

echo ""
echo "Total: $passed_tests/$total_tests test suites passed"

# Clean up
echo ""
echo -e "${YELLOW}Cleaning up...${NC}"
make -f Makefile clean > /dev/null 2>&1

echo -e "${GREEN}Done!${NC}"
