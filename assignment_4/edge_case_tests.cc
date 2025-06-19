// edge_case_tests.cc
// Additional test cases for extreme edge cases

#include <gtest/gtest.h>
#include "library_code.cc"

#include <cctype>
#include <chrono>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <random>
#include <algorithm>

// Utility function to run a test with given input and expected output
std::string runTestWithInput(void (*testFunction)(), const std::string& input) {
    // Backup cin and cout
    std::streambuf* cin_buffer = std::cin.rdbuf();
    std::streambuf* cout_buffer = std::cout.rdbuf();
    
    // Create input and output streams
    std::stringstream inputStream(input);
    std::stringstream outputStream;
    
    // Redirect cin and cout
    std::cin.rdbuf(inputStream.rdbuf());
    std::cout.rdbuf(outputStream.rdbuf());
    
    // Run the test function
    testFunction();
    
    // Restore cin and cout
    std::cin.rdbuf(cin_buffer);
    std::cout.rdbuf(cout_buffer);
    
    // Get the output
    std::string result = outputStream.str();
    // Remove trailing whitespace
    while (!result.empty() && std::isspace(result.back())) {
        result.pop_back();
    }
    
    return result;
}

// ===================== Question 3 Edge Cases =====================

TEST(Question3EdgeCases, NoPathExists) {
    // Test when no transformation path exists (disconnected components)
    std::string input = "cat dog 4\nhat mat sat bat";
    std::string output = runTestWithInput(question3, input);
    EXPECT_EQ(output, "0");
}

TEST(Question3EdgeCases, BeginWordEqualsEndWord) {
    // Although problem states beginWord != endWord, test graceful handling
    std::string input = "hit hit 3\nhot dot dog";
    std::string output = runTestWithInput(question3, input);
    EXPECT_EQ(output, "1");  // The word itself counts as 1
}

TEST(Question3EdgeCases, SingleLetterWords) {
    // Test with single letter words
    std::string input = "a z 24\nb c d e f g h i j k l m n o p q r s t u v w x y";
    std::string output = runTestWithInput(question3, input);
    EXPECT_EQ(output, "0");  // z is not in wordList
}

TEST(Question3EdgeCases, MaximumWordLength) {
    // Test with maximum length words (10 characters)
    std::string input = "abcdefghij abcdefghik 3\nabcdefghij abcdefghik abcdefghil";
    std::string output = runTestWithInput(question3, input);
    EXPECT_EQ(output, "2");  // Direct transformation exists
}

TEST(Question3EdgeCases, LargeWordList) {
    // Test with maximum N (5000 words)
    std::stringstream input;
    input << "aaaa zzzz 5000\n";
    
    // Generate 5000 4-letter words
    std::vector<std::string> words;
    for (int i = 0; i < 5000; i++) {
        std::string word = "aaaa";
        int num = i;
        for (int j = 3; j >= 0; j--) {
            word[j] = 'a' + (num % 26);
            num /= 26;
        }
        words.push_back(word);
    }
    
    // Make sure zzzz is in the list
    words[4999] = "zzzz";
    
    // Add some intermediate words to create a path
    words[0] = "aaaa";
    words[1] = "aaab";
    words[2] = "aabb";
    words[3] = "abbb";
    words[4] = "bbbb";
    // ... path continues through other transformations
    
    for (const auto& word : words) {
        input << word << " ";
    }
    
    std::string output = runTestWithInput(question3, input.str());
    // The actual result depends on the specific path available
    EXPECT_NE(output, "");  // Should produce some output
}

TEST(Question3EdgeCases, AllWordsIdentical) {
    // Test when all words in wordList are identical
    std::string input = "hit hot 5\nhot hot hot hot hot";
    std::string output = runTestWithInput(question3, input);
    EXPECT_EQ(output, "2");  // hit -> hot
}

TEST(Question3EdgeCases, CircularPath) {
    // Test with circular transformations
    std::string input = "hit cog 6\nhot dot dog log cog hog";
    std::string output = runTestWithInput(question3, input);
    EXPECT_EQ(output, "5");  // hit -> hot -> dot -> dog -> cog
}

// ===================== Question 4 Edge Cases =====================

TEST(Question4EdgeCases, NoEdges) {
    // Test with no edges (disconnected graph)
    std::string input = "5 0\n2";
    std::string output = runTestWithInput(question4, input);
    EXPECT_EQ(output, "-1");
}

TEST(Question4EdgeCases, AlreadyAtDestination) {
    // Test when start equals destination
    std::string input = "1 0\n0";
    std::string output = runTestWithInput(question4, input);
    EXPECT_EQ(output, "0");
}

TEST(Question4EdgeCases, ZeroCostEdges) {
    // Test with zero cost edges
    std::string input = "3 2\n0 1 0\n1 2 0\n5";
    std::string output = runTestWithInput(question4, input);
    EXPECT_EQ(output, "0");
}

TEST(Question4EdgeCases, MaximumCostEdges) {
    // Test with maximum cost edges (100000)
    std::string input = "2 1\n0 1 100000\n0";
    std::string output = runTestWithInput(question4, input);
    EXPECT_EQ(output, "100000");
}

TEST(Question4EdgeCases, MaximumDiscounts) {
    // Test with maximum discounts (500)
    std::string input = "2 1\n0 1 1000\n500";
    std::string output = runTestWithInput(question4, input);
    EXPECT_EQ(output, "500");  // Using one discount: 1000/2 = 500
}

TEST(Question4EdgeCases, MoreDiscountsThanEdges) {
    // Test when D > M (more discounts than edges)
    std::string input = "3 2\n0 1 100\n1 2 200\n10";
    std::string output = runTestWithInput(question4, input);
    EXPECT_EQ(output, "150");  // 100/2 + 200/2 = 50 + 100 = 150
}

TEST(Question4EdgeCases, CompleteGraph) {
    // Test with complete graph (maximum edges)
    std::stringstream input;
    int n = 30;  // Smaller for testing
    int m = n * (n - 1) / 2;
    input << n << " " << m << "\n";
    
    // Add all possible edges
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            input << i << " " << j << " " << (i + j + 1) << "\n";
        }
    }
    input << "15";  // Some discounts
    
    std::string output = runTestWithInput(question4, input.str());
    // Direct edge from 0 to n-1 exists with cost = 0 + (n-1) + 1 = n
    // With discount: n/2
    EXPECT_NE(output, "-1");  // Path should exist
}

TEST(Question4EdgeCases, OddCostWithDiscount) {
    // Test odd costs with discount (integer division)
    std::string input = "2 1\n0 1 999\n1";
    std::string output = runTestWithInput(question4, input);
    EXPECT_EQ(output, "499");  // 999/2 = 499 (integer division)
}

// ===================== Question 5 Edge Cases =====================

TEST(Question5EdgeCases, NoPositiveFlowValves) {
    // Test when all valves have zero flow
    std::string input = "3\nAA 0 2 BB CC\nBB 0 2 AA CC\nCC 0 2 AA BB";
    std::string output = runTestWithInput(question5, input);
    EXPECT_EQ(output, "0");
}

TEST(Question5EdgeCases, SingleValveMaxFlow) {
    // Test with single valve having maximum flow rate (30)
    std::string input = "2\nAA 0 1 BB\nBB 30 1 AA";
    std::string output = runTestWithInput(question5, input);
    // Move to BB (1 min), open BB (1 min), flow for 28 minutes = 30 * 28 = 840
    EXPECT_EQ(output, "840");
}

TEST(Question5EdgeCases, AllValvesPositiveFlow) {
    // Test when many valves have positive flow (up to 15)
    std::stringstream input;
    input << "16\n";
    input << "AA 0 15 ";
    for (int i = 1; i <= 15; i++) {
        input << "V" << std::setfill('0') << std::setw(2) << i << " ";
    }
    input << "\n";
    
    // Add 15 valves with positive flow
    for (int i = 1; i <= 15; i++) {
        input << "V" << std::setfill('0') << std::setw(2) << i << " " << i << " 1 AA\n";
    }
    
    std::string output = runTestWithInput(question5, input.str());
    EXPECT_NE(output, "0");  // Should be able to open some valves
}

TEST(Question5EdgeCases, LinearPath) {
    // Test with linear path (worst case for movement)
    std::stringstream input;
    int n = 30;
    input << n << "\n";
    
    // Create a linear chain: AA -> V01 -> V02 -> ... -> V29
    input << "AA 0 1 V01\n";
    for (int i = 1; i < n - 1; i++) {
        std::string curr = "V" + std::to_string(i < 10 ? 0 : i/10) + std::to_string(i % 10);
        std::string next = "V" + std::to_string((i+1) < 10 ? 0 : (i+1)/10) + std::to_string((i+1) % 10);
        std::string prev = (i == 1) ? "AA" : "V" + std::to_string((i-1) < 10 ? 0 : (i-1)/10) + std::to_string((i-1) % 10);
        
        // Only valve at position 15 has positive flow
        int flow = (i == 15) ? 20 : 0;
        input << curr << " " << flow << " 2 " << prev << " " << next << "\n";
    }
    
    // Last valve
    std::string last = "V" + std::to_string((n-1) < 10 ? 0 : (n-1)/10) + std::to_string((n-1) % 10);
    std::string prev = "V" + std::to_string((n-2) < 10 ? 0 : (n-2)/10) + std::to_string((n-2) % 10);
    input << last << " 0 1 " << prev << "\n";
    
    std::string output = runTestWithInput(question5, input.str());
    // Need to travel 15 steps to reach the valve, then 1 to open = 16 minutes used
    // Remaining 14 minutes of flow at rate 20 = 280
    EXPECT_GT(std::stoi(output), 0);
}

TEST(Question5EdgeCases, TimeJustEnough) {
    // Test when we have just enough time to open one valve
    std::string input = "30\nAA 0 1 BB\n";
    // Add 28 intermediate valves with 0 flow
    for (int i = 0; i < 28; i++) {
        if (i == 0) {
            input += "BB 0 1 CC\n";
        } else if (i == 27) {
            input += "ZZ 25 1 YY\n";
        } else {
            char curr = 'C' + i - 1;
            char next = 'C' + i;
            if (curr > 'Z') curr = 'A';
            if (next > 'Z') next = 'A';
            input += std::string(1, curr) + std::string(1, curr) + " 0 1 " + 
                     std::string(1, next) + std::string(1, next) + "\n";
        }
    }
    
    std::string output = runTestWithInput(question5, input);
    // If it takes 29 minutes to reach ZZ and 1 minute to open, no flow time remains
    EXPECT_EQ(output, "0");
}

TEST(Question5EdgeCases, MaxBitMaskStates) {
    // Test with exactly 15 positive flow valves (maximum for bitmask)
    std::stringstream input;
    input << "16\nAA 0 15 ";
    
    // Create valve names
    std::vector<std::string> valves;
    valves.push_back("AA");
    for (int i = 0; i < 15; i++) {
        valves.push_back("V" + std::string(1, 'A' + i));
    }
    
    // AA connects to all others
    for (int i = 1; i < 16; i++) {
        input << valves[i] << " ";
    }
    input << "\n";
    
    // Each valve has flow = 2 and connects only to AA
    for (int i = 1; i < 16; i++) {
        input << valves[i] << " 2 1 AA\n";
    }
    
    std::string output = runTestWithInput(question5, input.str());
    // Can open multiple valves, each taking 2 minutes (1 to move, 1 to open)
    EXPECT_GT(std::stoi(output), 0);
}

// Performance tests
TEST(PerformanceTests, Question3LargeInput) {
    // Generate large input for Question 3
    std::stringstream input;
    input << "aaaa zzzz 5000\n";
    
    // Generate connected chain of words
    for (int i = 0; i < 5000; i++) {
        std::string word = "aaaa";
        // Create systematic variations
        word[i % 4] = 'a' + (i % 26);
        input << word << " ";
    }
    input << "zzzz"; // Make sure end word is in list
    
    auto start = std::chrono::high_resolution_clock::now();
    std::string output = runTestWithInput(question3, input.str());
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    EXPECT_LT(duration.count(), 1000); // Should complete within 1 second
}

TEST(PerformanceTests, Question4LargeInput) {
    // Generate large input for Question 4
    std::stringstream input;
    int n = 1000, m = 1000;
    input << n << " " << m << "\n";
    
    // Create edges
    std::mt19937 gen(42); // Fixed seed for reproducibility
    std::uniform_int_distribution<> node_dist(0, n-1);
    std::uniform_int_distribution<> cost_dist(1, 100000);
    
    for (int i = 0; i < m; i++) {
        int u = node_dist(gen);
        int v = node_dist(gen);
        while (u == v) v = node_dist(gen);
        input << u << " " << v << " " << cost_dist(gen) << "\n";
    }
    input << "500"; // Maximum discounts
    
    auto start = std::chrono::high_resolution_clock::now();
    std::string output = runTestWithInput(question4, input.str());
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    EXPECT_LT(duration.count(), 1000); // Should complete within 1 second
}

TEST(PerformanceTests, Question5LargeInput) {
    // Generate large input for Question 5
    std::stringstream input;
    int n = 70; // Maximum nodes
    input << n << "\n";
    
    // Generate valve names
    std::vector<std::string> names;
    for (int i = 0; i < n; i++) {
        std::string name;
        name += 'A' + (i / 26);
        name += 'A' + (i % 26);
        names.push_back(name);
    }
    names[0] = "AA"; // Ensure start valve is AA
    
    // Create a connected graph
    for (int i = 0; i < n; i++) {
        // Flow rate: first 15 valves have positive flow
        int flow = (i > 0 && i <= 15) ? i : 0;
        
        // Connect to 3-5 other valves
        int connections = 3 + (i % 3);
        input << names[i] << " " << flow << " " << connections << " ";
        
        // Connect to nearby valves
        for (int j = 0; j < connections; j++) {
            int target = (i + j + 1) % n;
            input << names[target] << " ";
        }
        input << "\n";
    }
    
    auto start = std::chrono::high_resolution_clock::now();
    std::string output = runTestWithInput(question5, input.str());
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    EXPECT_LT(duration.count(), 3000); // Should complete within 3 seconds
}

// Main entry point
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
