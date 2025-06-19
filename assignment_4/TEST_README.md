# Assignment 4 - Test Suite Documentation

## Overview
This directory contains comprehensive test suites for Assignment 4, including:
- Original test cases from the assignment
- Edge case tests for extreme scenarios
- Stress tests for performance validation
- Memory usage analysis

## Files Created

### Test Files
1. **edge_case_tests.cc** - Google Test based edge case tests
   - Tests for boundary conditions
   - Tests for invalid inputs
   - Tests for maximum constraints
   - Performance tests with large inputs

2. **stress_test.cc** - Standalone stress testing program
   - Validation tests for correctness
   - Stress tests with randomly generated large inputs
   - Memory usage analysis
   - Performance benchmarking

3. **Test data files** in `testdata/edge_cases/`
   - Additional test cases for extreme scenarios

### Build Files
1. **Makefile** - For building stress tests and edge case tests
2. **CMakeLists_with_edge_cases.txt** - Extended CMake configuration
3. **run_all_tests.sh** - Bash script to run all test suites

## How to Run Tests

### Option 1: Run All Tests (Recommended)
```bash
chmod +x run_all_tests.sh
./run_all_tests.sh
```

This will:
- Build all test executables
- Run original test suite
- Run stress tests
- Run edge case tests (if Google Test is available)
- Provide a summary of results

### Option 2: Run Individual Test Suites

#### Original Tests
```bash
mkdir -p build
cd build
cmake ..
make
./runTests
```

#### Stress Tests
```bash
make -f Makefile stress_test
./stress_test
```

#### Edge Case Tests (requires Google Test)
```bash
make -f Makefile edge_case_tests
./edge_case_tests
```

## Test Coverage

### Question 3 - Word Transformation Sequence
- **Edge Cases Tested:**
  - No path exists (disconnected components)
  - Begin word equals end word
  - Single letter words
  - Maximum word length (10 characters)
  - Maximum word list size (5000 words)
  - All words identical
  - Circular paths

- **Stress Tests:**
  - Various word lengths (1, 5, 10)
  - Various list sizes (10, 100, 1000, 5000)
  - Random word generation
  - Performance validation (< 1 second)

### Question 4 - Minimum Cost to Reach City With Discounts
- **Edge Cases Tested:**
  - No edges (disconnected graph)
  - Already at destination
  - Zero cost edges
  - Maximum cost edges (100,000)
  - Maximum discounts (500)
  - More discounts than edges
  - Complete graphs
  - Odd costs with integer division

- **Stress Tests:**
  - Various graph sizes (10-1000 nodes)
  - Various edge counts (20-1000 edges)
  - Various discount counts (0-500)
  - Random graph generation
  - Performance validation (< 1 second)

### Question 5 - Valve Control
- **Edge Cases Tested:**
  - No positive flow valves
  - Single valve with maximum flow
  - All valves have positive flow (15 valves)
  - Linear path (worst case for movement)
  - Just enough time to open one valve
  - Maximum bitmask states (15 valves)

- **Stress Tests:**
  - Various graph sizes (10-70 nodes)
  - Various positive flow valve counts (5-15)
  - Random graph generation
  - Performance validation (< 3 seconds)

## Memory Usage Analysis

The stress test program includes memory usage estimates for maximum inputs:

- **Question 3**: ~100MB (worst case with 5000² adjacency list)
- **Question 4**: ~4MB (1000 nodes × 501 discounts)
- **Question 5**: ~275MB (memoization table with 2^15 states)

## Performance Benchmarks

All implementations are tested to ensure they meet the time constraints:
- Question 3: < 1 second
- Question 4: < 1 second
- Question 5: < 3 seconds

## Debugging Tips

1. If a test fails, check the output for specific error messages
2. Use the stress test program to identify performance bottlenecks
3. Run individual test cases to isolate issues
4. Check memory usage if the program crashes on large inputs

## Common Issues and Solutions

1. **CMake build fails**: Make sure you have CMake 3.10+ installed
2. **Google Test not found**: Edge case tests will be skipped, but other tests will run
3. **Time limit exceeded**: Check algorithm complexity and optimize bottlenecks
4. **Segmentation fault**: Usually indicates stack overflow or out-of-bounds access

## Adding New Tests

To add new test cases:

1. For file-based tests: Add `.in` and `.out` files to `testdata/`
2. For programmatic tests: Add new test functions to `edge_case_tests.cc`
3. For stress tests: Modify parameters in `stress_test.cc`

## Conclusion

This comprehensive test suite ensures your implementations are:
- Correct for all edge cases
- Efficient for maximum constraints
- Robust against invalid inputs
- Well-optimized for performance

Run all tests before submission to ensure full correctness!
