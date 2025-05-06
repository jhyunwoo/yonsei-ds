// test_runner.cc

#include <gtest/gtest.h>
#include "library_code.cc"

TEST(findDuplicates, Question5) {
  std::vector<int> nums;
  std::vector<int> rhs;
    
  // Example 1
  nums = std::vector<int>{4,3,2,7,8,2,3,1};
  rhs = std::vector<int>{3,2};
  EXPECT_EQ(findDuplicates(nums), rhs);

  // Example 2
  nums = std::vector<int>{1,1,2};
  rhs = std::vector<int>{1};
  EXPECT_EQ(findDuplicates(nums), rhs);

  // Example 3
  nums = std::vector<int>{1};
  rhs = std::vector<int>{};
  EXPECT_EQ(findDuplicates(nums), rhs);

  nums = std::vector<int>{9,9,8,8,7,7,6,6,5,5,4,4,3,3,2,2,1,1};
  rhs = std::vector<int>{7,9,6,8,5,4,3,2,1};
  EXPECT_EQ(findDuplicates(nums), rhs);

}

TEST(resultsArray, Question6) {
  std::vector<std::vector<int>> queries;
  int k;
  std::vector<int> rhs;

  // Example 1
  queries = std::vector<std::vector<int>>{std::vector<int>{1,2},std::vector<int>{3,4},std::vector<int>{2,3},std::vector<int>{-3,0}};
  k = 2;
  rhs = std::vector<int>{-1,7,5,3};
  EXPECT_EQ(resultsArray(queries, k), rhs);

  // Example 2
  queries = std::vector<std::vector<int>>{std::vector<int>{5,5},std::vector<int>{4,4},std::vector<int>{3,3}};
  k = 1;
  rhs = std::vector<int>{10,8,6};
  EXPECT_EQ(resultsArray(queries, k), rhs);

  // WRITE EXTRA TESTS
}

// TEST(VectorPQueue, Question7) {
//   using PQueue = VectorPQueue;
//
//   PQueue* q1 = new PQueue();
//   // Insert "ping" and then "pong" in *q1.
//   q1->enqueue(std::string("ping"));
//   q1->enqueue(std::string("pong"));
//   // Extract the minimum of *q1 twice.
//   EXPECT_EQ(q1->extractMin(), std::string("ping"));
//   EXPECT_EQ(q1->extractMin(), std::string("pong"));
//   // Now, *q1 should be empty
//
//   PQueue* q2 = new PQueue();
//   // Insert "pong" and then "ping" in *q2.
//   q2->enqueue(std::string("ping"));
//   q2->enqueue(std::string("pong"));
//   // Extract the minimum of *q2 twice.
//   EXPECT_EQ(q2->extractMin(), std::string("ping"));
//   EXPECT_EQ(q2->extractMin(), std::string("pong"));
//   // Now, *q2 should be empty.
//
//   // Test for merge
//   q1->enqueue(std::string("ping"));
//   q2->enqueue(std::string("pong"));
//   PQueue* q3 = PQueue::merge(q1, q2);
//   // Do not use q1 and q2 after here.
//   EXPECT_EQ(q3->extractMin(), std::string("ping"));
//   EXPECT_EQ(q3->extractMin(), std::string("pong"));
//   delete q3;
//   // Do not use q3 after here.
//
//   // WRITE EXTRA TESTS
// }
//
// TEST(HeapPQueue, Question7) {
//   using PQueue = HeapPQueue;
//
//   PQueue* q1 = new PQueue();
//   // Insert "ping" and then "pong" in *q1.
//   q1->enqueue(std::string("ping"));
//   q1->enqueue(std::string("pong"));
//   // Extract the minimum of *q1 twice.
//   EXPECT_EQ(q1->extractMin(), std::string("ping"));
//   EXPECT_EQ(q1->extractMin(), std::string("pong"));
//   // Now, *q1 should be empty
//
//   PQueue* q2 = new PQueue();
//   // Insert "pong" and then "ping" in *q2.
//   q2->enqueue(std::string("ping"));
//   q2->enqueue(std::string("pong"));
//   // Extract the minimum of *q2 twice.
//   EXPECT_EQ(q2->extractMin(), std::string("ping"));
//   EXPECT_EQ(q2->extractMin(), std::string("pong"));
//   // Now, *q2 should be empty.
//
//   // Test for merge
//   q1->enqueue(std::string("ping"));
//   q2->enqueue(std::string("pong"));
//   PQueue* q3 = PQueue::merge(q1, q2);
//   // Do not use q1 and q2 after here.
//   EXPECT_EQ(q3->extractMin(), std::string("ping"));
//   EXPECT_EQ(q3->extractMin(), std::string("pong"));
//   delete q3;
//   // Do not use q3 after here.
//
//   // WRITE EXTRA TESTS
// }

// Main entry for GoogleTest
int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
