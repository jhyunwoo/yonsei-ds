// stress_test.cc
// Stress testing and validation for the implementations

#include <iostream>
#include <sstream>
#include <chrono>
#include <random>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <iomanip>

// Include the implementations
#include "library_code.cc"

using namespace std;
using namespace std::chrono;

// Helper function to generate random strings
string generateRandomWord(int length, mt19937& gen) {
    string word;
    uniform_int_distribution<> dis('a', 'z');
    for (int i = 0; i < length; i++) {
        word += static_cast<char>(dis(gen));
    }
    return word;
}

// Stress test for Question 3
void stressTestQuestion3() {
    cout << "=== Stress Testing Question 3 ===" << endl;
    
    mt19937 gen(42);
    
    // Test different word lengths
    vector<int> wordLengths = {1, 5, 10};
    vector<int> wordCounts = {10, 100, 1000, 5000};
    
    for (int length : wordLengths) {
        for (int count : wordCounts) {
            cout << "Testing with " << count << " words of length " << length << "..." << endl;
            
            // Generate test case
            stringstream input;
            vector<string> words;
            
            // Generate words
            set<string> wordSet;
            while (wordSet.size() < count) {
                wordSet.insert(generateRandomWord(length, gen));
            }
            
            words.assign(wordSet.begin(), wordSet.end());
            
            // Pick random begin and end words
            uniform_int_distribution<> wordDis(0, count - 1);
            string beginWord = words[wordDis(gen)];
            string endWord = words[wordDis(gen)];
            
            input << beginWord << " " << endWord << " " << count << "\n";
            for (const string& w : words) {
                input << w << " ";
            }
            
            // Measure time
            auto start = high_resolution_clock::now();
            
            // Redirect cin
            stringstream output;
            streambuf* cinBackup = cin.rdbuf();
            streambuf* coutBackup = cout.rdbuf();
            cin.rdbuf(input.rdbuf());
            cout.rdbuf(output.rdbuf());
            
            question3();
            
            cin.rdbuf(cinBackup);
            cout.rdbuf(coutBackup);
            
            auto end = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(end - start);
            
            cout << "  Time: " << duration.count() << "ms" << endl;
            cout << "  Result: " << output.str();
            
            if (duration.count() > 1000) {
                cout << "  WARNING: Exceeded time limit!" << endl;
            }
            cout << endl;
        }
    }
}

// Stress test for Question 4
void stressTestQuestion4() {
    cout << "=== Stress Testing Question 4 ===" << endl;
    
    mt19937 gen(42);
    
    vector<pair<int, int>> testCases = {
        {10, 20},      // Small graph
        {100, 200},    // Medium graph
        {500, 1000},   // Large graph
        {1000, 1000}   // Maximum size
    };
    
    vector<int> discountCases = {0, 10, 100, 500};
    
    for (auto [n, m] : testCases) {
        for (int d : discountCases) {
            if (d > m) continue; // Skip invalid cases
            
            cout << "Testing with N=" << n << ", M=" << m << ", D=" << d << "..." << endl;
            
            // Generate test case
            stringstream input;
            input << n << " " << m << "\n";
            
            // Generate random edges
            uniform_int_distribution<> nodeDis(0, n - 1);
            uniform_int_distribution<> costDis(1, 100000);
            
            set<pair<int, int>> edges;
            while (edges.size() < m) {
                int u = nodeDis(gen);
                int v = nodeDis(gen);
                if (u != v && u < v) {
                    edges.insert({u, v});
                }
            }
            
            for (auto [u, v] : edges) {
                input << u << " " << v << " " << costDis(gen) << "\n";
            }
            input << d << "\n";
            
            // Measure time
            auto start = high_resolution_clock::now();
            
            // Redirect cin/cout
            stringstream output;
            streambuf* cinBackup = cin.rdbuf();
            streambuf* coutBackup = cout.rdbuf();
            cin.rdbuf(input.rdbuf());
            cout.rdbuf(output.rdbuf());
            
            question4();
            
            cin.rdbuf(cinBackup);
            cout.rdbuf(coutBackup);
            
            auto end = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(end - start);
            
            cout << "  Time: " << duration.count() << "ms" << endl;
            cout << "  Result: " << output.str();
            
            if (duration.count() > 1000) {
                cout << "  WARNING: Exceeded time limit!" << endl;
            }
            cout << endl;
        }
    }
}

// Stress test for Question 5
void stressTestQuestion5() {
    cout << "=== Stress Testing Question 5 ===" << endl;
    
    mt19937 gen(42);
    
    vector<int> testSizes = {10, 30, 50, 70};
    vector<int> flowValveCounts = {5, 10, 15};
    
    for (int n : testSizes) {
        for (int k : flowValveCounts) {
            if (k > n - 1) continue;
            
            cout << "Testing with N=" << n << ", K=" << k << " positive flow valves..." << endl;
            
            // Generate test case
            stringstream input;
            input << n << "\n";
            
            // Generate valve names
            vector<string> valveNames;
            valveNames.push_back("AA");
            for (int i = 1; i < n; i++) {
                string name;
                name += 'A' + ((i-1) / 26);
                name += 'A' + ((i-1) % 26);
                valveNames.push_back(name);
            }
            
            // Assign flow rates (first k valves after AA have positive flow)
            vector<int> flowRates(n, 0);
            uniform_int_distribution<> flowDis(1, 30);
            for (int i = 1; i <= k && i < n; i++) {
                flowRates[i] = flowDis(gen);
            }
            
            // Create a connected graph (ensure all valves are reachable)
            for (int i = 0; i < n; i++) {
                input << valveNames[i] << " " << flowRates[i] << " ";
                
                // Number of connections (between 1 and min(15, n-1))
                int numConnections = min(3 + (i % 5), min(15, n - 1));
                input << numConnections << " ";
                
                // Connect to other valves
                set<int> connected;
                // Always connect to next valve for connectivity
                if (i < n - 1) connected.insert((i + 1) % n);
                if (i > 0) connected.insert((i - 1 + n) % n);
                
                // Add random connections
                uniform_int_distribution<> connDis(0, n - 1);
                while (connected.size() < numConnections) {
                    int target = connDis(gen);
                    if (target != i) {
                        connected.insert(target);
                    }
                }
                
                for (int j : connected) {
                    input << valveNames[j] << " ";
                }
                input << "\n";
            }
            
            // Measure time
            auto start = high_resolution_clock::now();
            
            // Redirect cin/cout
            stringstream output;
            streambuf* cinBackup = cin.rdbuf();
            streambuf* coutBackup = cout.rdbuf();
            cin.rdbuf(input.rdbuf());
            cout.rdbuf(output.rdbuf());
            
            question5();
            
            cin.rdbuf(cinBackup);
            cout.rdbuf(coutBackup);
            
            auto end = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(end - start);
            
            cout << "  Time: " << duration.count() << "ms" << endl;
            cout << "  Result: " << output.str();
            
            if (duration.count() > 3000) {
                cout << "  WARNING: Exceeded time limit!" << endl;
            }
            cout << endl;
        }
    }
}

// Validation tests - ensure correctness on known cases
void validationTests() {
    cout << "=== Running Validation Tests ===" << endl;
    
    // Question 3 validation
    {
        cout << "\nQuestion 3 Validation:" << endl;
        
        // Test case 1: Simple path
        {
            stringstream input("hit cog 6\nhot dot dog lot log cog");
            stringstream output;
            streambuf* cinBackup = cin.rdbuf();
            streambuf* coutBackup = cout.rdbuf();
            cin.rdbuf(input.rdbuf());
            cout.rdbuf(output.rdbuf());
            
            question3();
            
            cin.rdbuf(cinBackup);
            cout.rdbuf(coutBackup);
            
            string result = output.str();
            result.erase(result.find_last_not_of(" \n\r\t") + 1);
            cout << "  Test 1 (hit->cog): " << (result == "5" ? "PASS" : "FAIL") << " (got " << result << ")" << endl;
        }
        
        // Test case 2: No path
        {
            stringstream input("hit cog 5\nhot dot dog lot log");
            stringstream output;
            streambuf* cinBackup = cin.rdbuf();
            streambuf* coutBackup = cout.rdbuf();
            cin.rdbuf(input.rdbuf());
            cout.rdbuf(output.rdbuf());
            
            question3();
            
            cin.rdbuf(cinBackup);
            cout.rdbuf(coutBackup);
            
            string result = output.str();
            result.erase(result.find_last_not_of(" \n\r\t") + 1);
            cout << "  Test 2 (no path): " << (result == "0" ? "PASS" : "FAIL") << " (got " << result << ")" << endl;
        }
    }
    
    // Question 4 validation
    {
        cout << "\nQuestion 4 Validation:" << endl;
        
        // Test case 1: With discount
        {
            stringstream input("5 5\n0 1 4\n2 1 3\n1 4 11\n3 2 3\n3 4 2\n1");
            stringstream output;
            streambuf* cinBackup = cin.rdbuf();
            streambuf* coutBackup = cout.rdbuf();
            cin.rdbuf(input.rdbuf());
            cout.rdbuf(output.rdbuf());
            
            question4();
            
            cin.rdbuf(cinBackup);
            cout.rdbuf(coutBackup);
            
            string result = output.str();
            result.erase(result.find_last_not_of(" \n\r\t") + 1);
            cout << "  Test 1 (with discount): " << (result == "9" ? "PASS" : "FAIL") << " (got " << result << ")" << endl;
        }
        
        // Test case 2: No path
        {
            stringstream input("4 2\n0 1 3\n2 3 2\n0");
            stringstream output;
            streambuf* cinBackup = cin.rdbuf();
            streambuf* coutBackup = cout.rdbuf();
            cin.rdbuf(input.rdbuf());
            cout.rdbuf(output.rdbuf());
            
            question4();
            
            cin.rdbuf(cinBackup);
            cout.rdbuf(coutBackup);
            
            string result = output.str();
            result.erase(result.find_last_not_of(" \n\r\t") + 1);
            cout << "  Test 2 (no path): " << (result == "-1" ? "PASS" : "FAIL") << " (got " << result << ")" << endl;
        }
    }
    
    // Question 5 validation
    {
        cout << "\nQuestion 5 Validation:" << endl;
        
        // Test case: Simple valve system
        {
            stringstream input("10\nAA 0 3 DD II BB\nBB 13 2 CC AA\nCC 2 2 DD BB\nDD 20 3 CC AA EE\nEE 3 2 FF DD\nFF 0 2 EE GG\nGG 0 2 FF HH\nHH 22 1 GG\nII 0 2 AA JJ\nJJ 21 1 II");
            stringstream output;
            streambuf* cinBackup = cin.rdbuf();
            streambuf* coutBackup = cout.rdbuf();
            cin.rdbuf(input.rdbuf());
            cout.rdbuf(output.rdbuf());
            
            question5();
            
            cin.rdbuf(cinBackup);
            cout.rdbuf(coutBackup);
            
            string result = output.str();
            result.erase(result.find_last_not_of(" \n\r\t") + 1);
            cout << "  Test (valve system): " << (result == "1651" ? "PASS" : "FAIL") << " (got " << result << ")" << endl;
        }
    }
}

// Memory usage test
void memoryUsageTest() {
    cout << "\n=== Memory Usage Analysis ===" << endl;
    
    // Test Question 3 with maximum input
    {
        cout << "\nQuestion 3 - Maximum input (5000 words):" << endl;
        cout << "  Word list: 5000 × 10 chars = ~50KB" << endl;
        cout << "  Adjacency list: worst case 5000² edges = ~100MB" << endl;
        cout << "  BFS structures: ~20KB" << endl;
        cout << "  Total estimate: ~100MB" << endl;
    }
    
    // Test Question 4 with maximum input
    {
        cout << "\nQuestion 4 - Maximum input (1000 nodes, 500 discounts):" << endl;
        cout << "  Distance array: 1000 × 501 × 4 bytes = ~2MB" << endl;
        cout << "  Adjacency list: 1000 edges × ~16 bytes = ~16KB" << endl;
        cout << "  Priority queue: worst case ~2MB" << endl;
        cout << "  Total estimate: ~4MB" << endl;
    }
    
    // Test Question 5 with maximum input
    {
        cout << "\nQuestion 5 - Maximum input (70 nodes, 15 important):" << endl;
        cout << "  Distance matrix: 70² × 4 bytes = ~20KB" << endl;
        cout << "  Memoization: 70 × 30 × 2^15 × 4 bytes = ~275MB" << endl;
        cout << "  Graph structures: ~10KB" << endl;
        cout << "  Total estimate: ~275MB" << endl;
    }
}

int main() {
    cout << "====================================" << endl;
    cout << "Data Structure Assignment 4 - Stress Test" << endl;
    cout << "====================================" << endl;
    
    // Run validation tests first
    validationTests();
    
    // Run stress tests
    cout << "\n\n";
    stressTestQuestion3();
    cout << "\n";
    stressTestQuestion4();
    cout << "\n";
    stressTestQuestion5();
    
    // Memory usage analysis
    memoryUsageTest();
    
    cout << "\n====================================" << endl;
    cout << "All tests completed!" << endl;
    cout << "====================================" << endl;
    
    return 0;
}
