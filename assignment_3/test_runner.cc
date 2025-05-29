// test_runner.cc
#include "library_code.h"

#include <algorithm>
#include <gtest/gtest.h>
#include <random>

// Helper function to generate unique random integers within a range
std::vector<int> generateUniqueRandomKeys(int count, int min_val, int max_val) {
  std::set<int> unique_keys{};
  std::random_device rd{};
  std::mt19937 gen{rd()};
  std::uniform_int_distribution<> distrib{min_val, max_val};

  // Ensure count does not exceed the possible range of unique values
  long long range_size{static_cast<long long>(max_val - min_val + 1)};
  if (count > range_size) {
    count = range_size;
  }
  if (count < 0)
    count = 0;

  while (unique_keys.size() < static_cast<size_t>(count)) {
    unique_keys.insert(distrib(gen));
  }

  std::vector<int> keys{unique_keys.begin(), unique_keys.end()};
  // Shuffle the unique keys to make the insertion order random for BST/AVL
  std::shuffle(keys.begin(), keys.end(), gen);

  return keys;
}

// Helper function to generate preorder and inorder traversals for a random BST
// Note: Random tree generation relies on a correctly implemented insertAVL.
bool generateRandomBSTTraversals(int node_count, int min_val, int max_val,
                                 std::vector<int> &preorder,
                                 std::vector<int> &inorder) {
  preorder.clear();
  inorder.clear();

  if (node_count <= 0)
    return false;

  std::vector<int> keys{generateUniqueRandomKeys(node_count, min_val, max_val)};
  if (keys.empty())
    return false; // Could not generate keys

  // Build a temporary BST
  Node *temp_root{nullptr};
  for (int key : keys) {
    temp_root = insertAVL(temp_root, key);
    if (!temp_root) {
      deleteTree(temp_root);
      return false;
    }
  }

  getPreorder(temp_root, preorder);
  getInorder(temp_root, inorder);

  deleteTree(temp_root);

  return !preorder.empty() && !inorder.empty();
}

// ---------------------------------------------
//                  Problem 3
// ---------------------------------------------

TEST(splitBST, Example1) {
  int target{2};
  std::vector<int> preorder{4, 2, 1, 3, 6, 5, 7};
  std::vector<int> inorder{1, 2, 3, 4, 5, 6, 7};

  std::vector<int> expected_preorder1{2, 1};
  std::vector<int> expected_inorder1{1, 2};
  std::vector<int> expected_preorder2{4, 3, 6, 5, 7};
  std::vector<int> expected_inorder2{3, 4, 5, 6, 7};

  SplitResult result{solveSplitBST(target, preorder, inorder)};
  EXPECT_EQ(result.preorder1, expected_preorder1);
  EXPECT_EQ(result.inorder1, expected_inorder1);
  EXPECT_EQ(result.preorder2, expected_preorder2);
  EXPECT_EQ(result.inorder2, expected_inorder2);
}

TEST(splitBST, Example2) {
  int target{4};
  std::vector<int> preorder{2, -3};
  std::vector<int> inorder{-3, 2};

  std::vector<int> expected_preorder1{2, -3};
  std::vector<int> expected_inorder1{-3, 2};
  std::vector<int> expected_preorder2{};
  std::vector<int> expected_inorder2{};

  SplitResult result{solveSplitBST(target, preorder, inorder)};
  EXPECT_EQ(result.preorder1, expected_preorder1);
  EXPECT_EQ(result.inorder1, expected_inorder1);
  EXPECT_EQ(result.preorder2, expected_preorder2);
  EXPECT_EQ(result.inorder2, expected_inorder2);
}

TEST(splitBST, RandomLarge) {
  const int NODE_COUNT{10000}; // Constraint: [1, 10000]
  const int MIN_VAL{-100000};  // Constraint: [-10^5, 10^5]
  const int MAX_VAL{100000};

  const int NUM_TESTS{100};

  for (int i{0}; i < NUM_TESTS; ++i) {
    std::vector<int> preorder{}, inorder{};
    ASSERT_TRUE(generateRandomBSTTraversals(NODE_COUNT, MIN_VAL, MAX_VAL,
                                            preorder, inorder));
    ASSERT_EQ(preorder.size(), NODE_COUNT);
    ASSERT_EQ(inorder.size(), NODE_COUNT);

    // Generate a random target value
    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::uniform_int_distribution<> distrib{MIN_VAL, MAX_VAL};
    int target{distrib(gen)};

    ASSERT_NO_THROW({
      SplitResult result{solveSplitBST(target, preorder, inorder)};

      // Optional basic checks:
      EXPECT_EQ(result.preorder1.size(), result.inorder1.size());
      EXPECT_EQ(result.preorder2.size(), result.inorder2.size());
      EXPECT_EQ(result.inorder1.size() + result.inorder2.size(), NODE_COUNT);
    });
  }
}

// Add your own test cases here (Create a new test case under the test suite named splitBST)

// ---------------------------------------------
//                  Problem 4
// ---------------------------------------------

TEST(convertBSTToCDLL, Example1) {
  std::vector<int> preorder{4, 2, 1, 3, 5};
  std::vector<int> inorder{1, 2, 3, 4, 5};

  std::vector<int> expected_successor_traversal{1, 2, 3, 4, 5};
  std::vector<int> expected_predecessor_traversal{1, 5, 4, 3, 2};

  CDLLResult result{solveBSTToCDLL(preorder, inorder)};

  EXPECT_EQ(result.successor_traversal, expected_successor_traversal);
  EXPECT_EQ(result.predecessor_traversal, expected_predecessor_traversal);
}

TEST(convertBSTToCDLL, Example2) {
  std::vector<int> preorder{2, 1, 4, 3};
  std::vector<int> inorder{1, 2, 3, 4};

  std::vector<int> expected_successor_traversal{1, 2, 3, 4};
  std::vector<int> expected_predecessor_traversal{1, 4, 3, 2};

  CDLLResult result{solveBSTToCDLL(preorder, inorder)};

  EXPECT_EQ(result.successor_traversal, expected_successor_traversal);
  EXPECT_EQ(result.predecessor_traversal, expected_predecessor_traversal);
}

TEST(convertBSTToCDLL, RandomLarge) {
  const int NODE_COUNT{10000}; // Constraint: [1, 10000]
  const int MIN_VAL{-100000};  // Constraint: [-10^5, 10^5]
  const int MAX_VAL{100000};

  const int NUM_TESTS{100};

  for (int i{0}; i < NUM_TESTS; ++i) {
    std::vector<int> preorder{}, inorder{};
    ASSERT_TRUE(generateRandomBSTTraversals(NODE_COUNT, MIN_VAL, MAX_VAL,
                                            preorder, inorder));
    ASSERT_EQ(preorder.size(), NODE_COUNT);
    ASSERT_EQ(inorder.size(), NODE_COUNT);

    ASSERT_NO_THROW({
      CDLLResult result{solveBSTToCDLL(preorder, inorder)};

      // Optional basic checks:
      EXPECT_EQ(result.successor_traversal.size(), NODE_COUNT);
      EXPECT_EQ(result.predecessor_traversal.size(), NODE_COUNT);

      // Main checks:
      std::vector<int> sorted_inorder{inorder};
      EXPECT_EQ(result.successor_traversal, sorted_inorder);
      std::reverse(sorted_inorder.begin() + 1, sorted_inorder.end());
      EXPECT_EQ(result.predecessor_traversal, sorted_inorder);
    });
  }
}

// Add your own test cases here (Create a new test case under the test suite named convertBSTToCDLL)

// ---------------------------------------------
//                  Problem 5
// ---------------------------------------------

TEST(insertAVL, Example1) {
  std::vector<int> keys{10, 20, 30};
  std::vector<int> expected_preorder{20, 10, 30};
  std::vector<int> expected_inorder{10, 20, 30};

  AVLResult result{solveInsertAVL(keys)};

  EXPECT_EQ(result.preorder, expected_preorder);
  EXPECT_EQ(result.inorder, expected_inorder);
}

TEST(insertAVL, Example2) {
  std::vector<int> keys{9, 5, 10, 0, 6, 11, -1, 1, 2};
  std::vector<int> expected_preorder{9, 1, 0, -1, 5, 2, 6, 10, 11};
  std::vector<int> expected_inorder{-1, 0, 1, 2, 5, 6, 9, 10, 11};

  AVLResult result{solveInsertAVL(keys)};

  EXPECT_EQ(result.preorder, expected_preorder);
  EXPECT_EQ(result.inorder, expected_inorder);
}

TEST(insertAVL, RandomLarge) {
  const int KEY_COUNT{10000}; // Constraint: [1, 10000]
  const int MIN_VAL{-100000}; // Constraint: [-10^5, 10^5]
  const int MAX_VAL{100000};

  const int NUM_TESTS{100};

  for (int i{0}; i < NUM_TESTS; ++i) {
    std::vector<int> keys{
        generateUniqueRandomKeys(KEY_COUNT, MIN_VAL, MAX_VAL)};
    ASSERT_EQ(keys.size(), KEY_COUNT);

    ASSERT_NO_THROW({
      AVLResult result{solveInsertAVL(keys)};

      // Optional basic checks:
      EXPECT_EQ(result.preorder.size(), KEY_COUNT);
      EXPECT_EQ(result.inorder.size(), KEY_COUNT);

      // Main checks:
      std::vector<int> sorted_keys{keys};
      std::sort(sorted_keys.begin(), sorted_keys.end());
      EXPECT_EQ(result.inorder, sorted_keys);
    });
  }
}

TEST(splitBST, SingleNode_TargetLess) {
  int target = 0;
  std::vector<int> preorder{5}, inorder{5};
  // 모두 target보다 큼 → left는 비어있고 right에 원본이 전부
  SplitResult r = solveSplitBST(target, preorder, inorder);
  EXPECT_TRUE(r.preorder1.empty());
  EXPECT_TRUE(r.inorder1.empty());
  EXPECT_EQ(r.preorder2, std::vector<int>({5}));
  EXPECT_EQ(r.inorder2, std::vector<int>({5}));
}

TEST(splitBST, SingleNode_TargetEqual) {
  int target = 5;
  std::vector<int> preorder{5}, inorder{5};
  // target과 같으므로 left에 노드가 모두 남음
  SplitResult r = solveSplitBST(target, preorder, inorder);
  EXPECT_EQ(r.preorder1, std::vector<int>({5}));
  EXPECT_EQ(r.inorder1,  std::vector<int>({5}));
  EXPECT_TRUE(r.preorder2.empty());
  EXPECT_TRUE(r.inorder2.empty());
}

TEST(splitBST, AllLessOrEqual) {
  int target = 10;
  std::vector<int> preorder{8, 3, 1, 6, 4, 10, 14},
                   inorder{1, 3, 4, 6, 8, 10, 14};

  SplitResult r = solveSplitBST(target, preorder, inorder);

  // 올바른 분할 결과: ≤10 노드들
  std::vector<int> expected_pre1{8, 3, 1, 6, 4, 10};
  std::vector<int> expected_in1{1, 3, 4, 6, 8, 10};
  // >10 노드들
  std::vector<int> expected_pre2{14};
  std::vector<int> expected_in2{14};

  EXPECT_EQ(r.preorder1, expected_pre1);
  EXPECT_EQ(r.inorder1,  expected_in1);
  EXPECT_EQ(r.preorder2, expected_pre2);
  EXPECT_EQ(r.inorder2,  expected_in2);
}

TEST(splitBST, AllGreater) {
  int target = -1;
  std::vector<int> preorder{8, 3, 1, 6, 4, 10, 14},
                   inorder{1, 3, 4, 6, 8, 10, 14};
  // target이 최소값보다 작으므로 left는 빈 트리
  SplitResult r = solveSplitBST(target, preorder, inorder);
  EXPECT_TRUE(r.preorder1.empty());
  EXPECT_TRUE(r.inorder1.empty());
  EXPECT_EQ(r.inorder2, inorder);
}

// ========== convertBSTToCDLL 특수 케이스 ==========
TEST(convertBSTToCDLL, SingleNode) {
  std::vector<int> preorder{42}, inorder{42};
  CDLLResult r = solveBSTToCDLL(preorder, inorder);
  // successor, predecessor 모두 {42}
  EXPECT_EQ(r.successor_traversal, std::vector<int>({42}));
  EXPECT_EQ(r.predecessor_traversal, std::vector<int>({42}));
}

TEST(convertBSTToCDLL, TwoNode_SkewedLeft) {
  // BST:   2
  //       /
  //      1
  std::vector<int> preorder{2,1}, inorder{1,2};
  CDLLResult r = solveBSTToCDLL(preorder, inorder);
  // 순환 리스트: 1 <-> 2 <-> 1
  EXPECT_EQ(r.successor_traversal, std::vector<int>({1,2}));
  EXPECT_EQ(r.predecessor_traversal, std::vector<int>({1,2}));
}

TEST(convertBSTToCDLL, TwoNode_SkewedRight) {
  // BST: 1
  //       \
  //        2
  std::vector<int> preorder{1,2}, inorder{1,2};
  CDLLResult r = solveBSTToCDLL(preorder, inorder);
  EXPECT_EQ(r.successor_traversal, std::vector<int>({1,2}));
  EXPECT_EQ(r.predecessor_traversal, std::vector<int>({1,2}));
}

// ========== insertAVL 특수 케이스 ==========
TEST(insertAVL, StrictlyIncreasing) {
  // 우측으로만 치우치도록 삽입 → 매번 RR 회전 발생
  std::vector<int> keys{1,2,3,4,5};
  AVLResult r = solveInsertAVL(keys);
  // 결과 트리 높이를 최소화하면 루트는 2 혹은 3이 될 수 있지만,
  // 우리의 구현은 LL/RR/LR/RL 표준에 따라 3이 루트가 됨
  std::vector<int> expected_inorder = keys;
  EXPECT_EQ(r.inorder, expected_inorder);
  // 중위 순회만 검증
}

TEST(insertAVL, StrictlyDecreasing) {
  // 좌측으로만 치우치도록 삽입 → 매번 LL 회전 발생
  std::vector<int> keys{5,4,3,2,1};
  AVLResult r = solveInsertAVL(keys);
  std::vector<int> expected_inorder = keys;
  std::reverse(expected_inorder.begin(), expected_inorder.end());
  EXPECT_EQ(r.inorder, expected_inorder);
}

TEST(insertAVL, TwoNodes) {
  // 두 개만 삽입 → 균형 유지, 루트는 첫 번째 키
  std::vector<int> keys{10, 5};
  AVLResult r = solveInsertAVL(keys);
  EXPECT_EQ(r.preorder.size(), 2);
  EXPECT_EQ(r.inorder, std::vector<int>({5,10}));
}

// Add your own test cases here (Create a new test case under the test suite named insertAVL)
