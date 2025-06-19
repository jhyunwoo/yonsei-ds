// test_runner.cc

#include <gtest/gtest.h>
#include "library_code.cc"

#include <cctype>
#include <chrono>
#include <fstream>
#include <sstream>
#include <streambuf>


// Trim all leading & trailing whitespaces
inline void strip(std::string& s) {
  s.erase(s.begin(),
          std::find_if_not(s.begin(), s.end(), ::isspace));
  s.erase(std::find_if_not(s.rbegin(), s.rend(), ::isspace).base(),
          s.end());
}


// Assignment 4 test wrapper
class Assignment4Test : public ::testing::TestWithParam<std::pair<std::string, std::string>> {
protected:
  std::stringstream input;
  std::stringstream output;
  std::streambuf* cin_buffer;
  std::streambuf* cout_buffer;

  std::string out_golden;

  void SetUp() override {
    // Backup cin and cout
    cin_buffer = std::cin.rdbuf();
    cout_buffer = std::cout.rdbuf();

    // Get test file names from parameters
    auto [input_file, output_file] = GetParam();

    // Read input file content
    std::ifstream ifs(input_file);
    ASSERT_TRUE(ifs) << "Failed to open input file: " << input_file;
    input << ifs.rdbuf();
    input.clear();

    // Redirect std::cin
    std::cin.rdbuf(input.rdbuf());

    // Read expected output
    std::ifstream ofs(output_file);
    ASSERT_TRUE(ofs) << "Failed to open output file: " << output_file;
    std::ostringstream buffer;
    buffer << ofs.rdbuf();
    out_golden = buffer.str();

    // Redirect std::cout
    std::cout.rdbuf(output.rdbuf());

    // Close file descriptors
    ifs.close();
    ofs.close();
  }

  void TearDown() override {
    std::cin.rdbuf(cin_buffer);
    std::cout.rdbuf(cout_buffer);
  }

  std::string GetOutput() {
    std::string result = output.str();
    strip(result);
    return result;
  }
};

class Question3TestFixture : public Assignment4Test {};
class Question4TestFixture : public Assignment4Test {};
class Question5TestFixture : public Assignment4Test {};


/* ========== Question 3 ========== */
TEST_P(Question3TestFixture, Question3Test) {
  auto start = std::chrono::high_resolution_clock::now();
  question3();
  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  EXPECT_LT(duration.count(), 1000);
  EXPECT_EQ(GetOutput(), out_golden);
}

// Question 3 test suite
INSTANTIATE_TEST_SUITE_P(
  Question3TestParam,
  Question3TestFixture,
  ::testing::Values(
    std::make_pair("../testdata/q3/q3-test1.in", "../testdata/q3/q3-test1.out"),  // 1 pt
    std::make_pair("../testdata/q3/q3-test2.in", "../testdata/q3/q3-test2.out"),  // 1 pt
    std::make_pair("../testdata/q3/q3-test3.in", "../testdata/q3/q3-test3.out"),  // 2 pts
    std::make_pair("../testdata/q3/q3-test4.in", "../testdata/q3/q3-test4.out"),  // 2 pts
    std::make_pair("../testdata/q3/q3-test5.in", "../testdata/q3/q3-test5.out"),  // 2 pts
    std::make_pair("../testdata/q3/q3-test6.in", "../testdata/q3/q3-test6.out")   // 2 pts
  )
);


/* ========== Question 4 ========== */
TEST_P(Question4TestFixture, Question4Test) {
  auto start = std::chrono::high_resolution_clock::now();
  question4();
  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  EXPECT_LT(duration.count(), 1000);
  EXPECT_EQ(GetOutput(), out_golden);
}

// Question 4 test suite
INSTANTIATE_TEST_SUITE_P(
  Question4TestParam,
  Question4TestFixture,
  ::testing::Values(
    std::make_pair("../testdata/q4/q4-test1.in", "../testdata/q4/q4-test1.out"),  // 2 pts
    std::make_pair("../testdata/q4/q4-test2.in", "../testdata/q4/q4-test2.out"),  // 2 pts
    std::make_pair("../testdata/q4/q4-test3.in", "../testdata/q4/q4-test3.out"),  // 2 pts
    std::make_pair("../testdata/q4/q4-test4.in", "../testdata/q4/q4-test4.out"),  // 2 pts
    std::make_pair("../testdata/q4/q4-test5.in", "../testdata/q4/q4-test5.out"),  // 2 pts
    std::make_pair("../testdata/q4/q4-test6.in", "../testdata/q4/q4-test6.out"),  // 2 pts
    std::make_pair("../testdata/q4/q4-test7.in", "../testdata/q4/q4-test7.out")   // 2 pts
  )
);


/* ========== Question 5 ========== */
TEST_P(Question5TestFixture, Question5Test) {
  auto start = std::chrono::high_resolution_clock::now();
  question5();
  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  EXPECT_LT(duration.count(), 3000);
  EXPECT_EQ(GetOutput(), out_golden);
}

// Question 5 test suite
INSTANTIATE_TEST_SUITE_P(
  Question5TestParam,
  Question5TestFixture,
  ::testing::Values(
    std::make_pair("../testdata/q5/q5-test1.in", "../testdata/q5/q5-test1.out"),  // 3 pts
    std::make_pair("../testdata/q5/q5-test2.in", "../testdata/q5/q5-test2.out"),  // 3 pts
    std::make_pair("../testdata/q5/q5-test3.in", "../testdata/q5/q5-test3.out"),  // 3 pts
    std::make_pair("../testdata/q5/q5-test4.in", "../testdata/q5/q5-test4.out"),  // 3 pts
    std::make_pair("../testdata/q5/q5-test5.in", "../testdata/q5/q5-test5.out"),  // 3 pts
    std::make_pair("../testdata/q5/q5-test6.in", "../testdata/q5/q5-test6.out")   // 3 pts
  )
);


// Main entry for GoogleTest
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
