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

  // 모든 값이 중복일 때
  nums = std::vector<int>{9,9,8,8,7,7,6,6,5,5,4,4,3,3,2,2,1,1};
  rhs = std::vector<int>{7,9,6,8,5,4,3,2,1};
  EXPECT_EQ(findDuplicates(nums), rhs);

  // 모든 값이 중복이 아닐 때
  nums.clear();
  for (int i = 1; i <= 100000; i++) {
    nums.push_back(i);
  }
  rhs = {};
  EXPECT_EQ(findDuplicates(nums), rhs);

  // 중복 값이 첫 번째에 존재할 때
  nums = {1,1,2,3,4,5};
  rhs = {1};
  EXPECT_EQ(findDuplicates(nums), rhs);

  // 종복 값이 마지막에 존재할 때
  nums = {1,2,3,4,5,5};
  rhs = {5};
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
  // 거리가 증가하는 경우
  queries.clear();
  for (int i = 1; i <= 1000; i++) {
    queries.push_back({i, 0});
  }
  k = 1000;
  rhs.clear();
  for (int i = 1; i <= 1000; i++) {
    if (i < k) rhs.push_back(-1);
    else rhs.push_back(i);
  }
  EXPECT_EQ(resultsArray(queries, k), rhs);

  // 거리가 감소하는 경우
  queries.clear();
  for (int i = 1000; i >= 1; i--) {
    queries.push_back({i, 0});
  }
  k = 1;
  rhs.clear();
  for (int i = 1000; i >= 1; i--) {
    rhs.push_back(i);
  }
  EXPECT_EQ(resultsArray(queries, k), rhs);

  // 모든 거리가 같은 경우
  queries = {{1,2},{-1,-2},{2,1},{-2,-1}};
  k = 3;
  rhs = {-1,-1,3,3};
  EXPECT_EQ(resultsArray(queries, k), rhs);

  // 모든 값이 음수인 경우
  queries = {{-1,-1},{-2,-2},{-3,-3}};
  k = 2;
  rhs = {-1,4,4};
  EXPECT_EQ(resultsArray(queries, k), rhs);

  // K 값이 클 경우
  queries.clear();
  for (int i = 0; i < 1000000000; i++) {
    queries.push_back({i, 0});
  }
  k = 1000000000;
  rhs.clear();
  for (int i = 0; i < 1000000000; i++) {
    if (i < k - 1) {
      rhs.push_back(-1);
    }
    else {
      rhs.push_back(i);
    }
  }
  EXPECT_EQ(resultsArray(queries, k), rhs);
}

TEST(VectorPQueue, Question7) {
  using PQueue = VectorPQueue;

  PQueue* q1 = new PQueue();
  // Insert "ping" and then "pong" in *q1.
  q1->enqueue(std::string("ping"));
  q1->enqueue(std::string("pong"));
  // Extract the minimum of *q1 twice.
  EXPECT_EQ(q1->extractMin(), std::string("ping"));
  EXPECT_EQ(q1->extractMin(), std::string("pong"));
  // Now, *q1 should be empty

  PQueue* q2 = new PQueue();
  // Insert "pong" and then "ping" in *q2.
  q2->enqueue(std::string("ping"));
  q2->enqueue(std::string("pong"));
  // Extract the minimum of *q2 twice.
  EXPECT_EQ(q2->extractMin(), std::string("ping"));
  EXPECT_EQ(q2->extractMin(), std::string("pong"));
  // Now, *q2 should be empty.

  // Test for merge
  q1->enqueue(std::string("ping"));
  q2->enqueue(std::string("pong"));
  PQueue* q3 = PQueue::merge(q1, q2);
  // Do not use q1 and q2 after here.
  EXPECT_EQ(q3->extractMin(), std::string("ping"));
  EXPECT_EQ(q3->extractMin(), std::string("pong"));
  delete q3;
  // Do not use q3 after here.

  // 올바른 순서로 정렬되는지 확인
  PQueue* wrightOrder = new PQueue();
  wrightOrder->enqueue("c");
  wrightOrder->enqueue("b");
  wrightOrder->enqueue("a");
  EXPECT_EQ(wrightOrder->extractMin(), "a");
  EXPECT_EQ(wrightOrder->extractMin(), "b");
  EXPECT_EQ(wrightOrder->extractMin(), "c");
  delete wrightOrder;

  // 중복 값 처리
  PQueue* dupQ = new PQueue();
  dupQ->enqueue("same");
  dupQ->enqueue("same");
  EXPECT_EQ(dupQ->extractMin(), "same");
  EXPECT_EQ(dupQ->extractMin(), "same");
  delete dupQ;

  // 대량 삽입 후 정렬 확인
  PQueue* bigQ = new PQueue();
  for (int i = 10000; i >= 1; i--) {
    bigQ->enqueue(std::to_string(i));
  }
  EXPECT_EQ(bigQ->extractMin(), "1");
  EXPECT_EQ(bigQ->extractMin(), "10");
  EXPECT_EQ(bigQ->extractMin(), "100");
  EXPECT_EQ(bigQ->extractMin(), "1000");
  EXPECT_EQ(bigQ->extractMin(), "10000");
  
  delete bigQ;

  // 다수 병합 후 정렬 확인
  PQueue* a = new PQueue();
  PQueue* b = new PQueue();
  for (int i = 0; i < 5000; i++) {
    a->enqueue(std::to_string(2 * i));
    b->enqueue(std::to_string(2 * i + 1));
  }
  PQueue* merged = PQueue::merge(a, b);
  EXPECT_EQ(merged->extractMin(), "0");
  EXPECT_EQ(merged->extractMin(), "1");
  EXPECT_EQ(merged->extractMin(), "10");
  EXPECT_EQ(merged->extractMin(), "100");
  EXPECT_EQ(merged->extractMin(), "1000");
  EXPECT_EQ(merged->extractMin(), "1001");
  
  delete merged;
}

TEST(HeapPQueue, Question7) {
  using PQueue = HeapPQueue;

  PQueue* q1 = new PQueue();
  // Insert "ping" and then "pong" in *q1.
  q1->enqueue(std::string("ping"));
  q1->enqueue(std::string("pong"));
  // Extract the minimum of *q1 twice.
  EXPECT_EQ(q1->extractMin(), std::string("ping"));
  EXPECT_EQ(q1->extractMin(), std::string("pong"));
  // Now, *q1 should be empty

  PQueue* q2 = new PQueue();
  // Insert "pong" and then "ping" in *q2.
  q2->enqueue(std::string("ping"));
  q2->enqueue(std::string("pong"));
  // Extract the minimum of *q2 twice.
  EXPECT_EQ(q2->extractMin(), std::string("ping")); 
  EXPECT_EQ(q2->extractMin(), std::string("pong"));
  // Now, *q2 should be empty.

  // Test for merge
  q1->enqueue(std::string("ping"));
  q2->enqueue(std::string("pong"));
  PQueue* q3 = PQueue::merge(q1, q2);
  // Do not use q1 and q2 after here.
  EXPECT_EQ(q3->extractMin(), std::string("ping"));
  EXPECT_EQ(q3->extractMin(), std::string("pong"));
  delete q3;
  // Do not use q3 after here.

  // WRITE EXTRA TESTS

  // 대량 삽입 후 정렬 확인
  PQueue* bigQ = new PQueue();
  VectorPQueue* bigQTest = new VectorPQueue();
  for (int i = 10000; i >= 1; i--) {
    bigQ->enqueue(std::to_string(i));
    bigQTest->enqueue(std::to_string(i));
  }
  for (int i = 1; i <= 10000; i++) {
    EXPECT_EQ(bigQ->extractMin(), bigQTest->extractMin());
  }
  delete bigQ;
  delete bigQTest;

  // 중복 값 처리
  PQueue* dupQ = new PQueue();
  VectorPQueue* dupQTest = new VectorPQueue();
  for (int i = 0; i < 1000; i++) {
    dupQ->enqueue("same");
    dupQTest->enqueue("same");
  }
  for (int i = 0; i < 1000; i++) {
    EXPECT_EQ(dupQ->extractMin(), dupQTest->extractMin());
  }
  delete dupQ;
  delete dupQTest;

  // 병합 후 정렬 확인
  PQueue* a = new PQueue();
  PQueue* b = new PQueue();
  VectorPQueue* aTest = new VectorPQueue();
  VectorPQueue* bTest = new VectorPQueue();
  for (int i = 0; i < 5000; i++) {
    a->enqueue(std::to_string(2 * i));
    aTest->enqueue(std::to_string(2 * i));
    b->enqueue(std::to_string(2 * i + 1));
    bTest->enqueue(std::to_string(2 * i + 1));
  }
  PQueue* merged = PQueue::merge(a, b);
  VectorPQueue* mergedTest = VectorPQueue::merge(aTest, bTest);
  for (int i = 0; i < 10000; i++) {
    EXPECT_EQ(merged->extractMin(), mergedTest->extractMin());
  }
  delete merged;
  delete mergedTest;

  // 사전순 정렬 확인
  PQueue* alphaQ = new PQueue();
  VectorPQueue* alphaQTest = new VectorPQueue();
  alphaQ->enqueue("z");
  alphaQTest->enqueue("z");
  alphaQ->enqueue("a");
  alphaQTest->enqueue("a");
  alphaQ->enqueue("m");
  alphaQTest->enqueue("m");
  alphaQ->enqueue("b");
  alphaQTest->enqueue("b");
  EXPECT_EQ(alphaQ->extractMin(), alphaQTest->extractMin());
  EXPECT_EQ(alphaQ->extractMin(), alphaQTest->extractMin());
  EXPECT_EQ(alphaQ->extractMin(), alphaQTest->extractMin());
  EXPECT_EQ(alphaQ->extractMin(), alphaQTest->extractMin());
  delete alphaQ;
  delete alphaQTest;

  // 6. 본인 스스로 merge 할 경우 확인
  PQueue* selfQ = new PQueue();
  VectorPQueue* selfQTest = new VectorPQueue();
  selfQ->enqueue("x");
  selfQ->enqueue("y");
  selfQTest->enqueue("x");
  selfQTest->enqueue("y");

  PQueue* doubled = PQueue::merge(selfQ, selfQ);
  VectorPQueue* doubledTest = VectorPQueue::merge(selfQTest, selfQTest);
  EXPECT_EQ(doubled->extractMin(), doubledTest->extractMin());
  EXPECT_EQ(doubled->extractMin(), doubledTest->extractMin());
  EXPECT_EQ(doubled->extractMin(), doubledTest->extractMin());
  EXPECT_EQ(doubled->extractMin(), doubledTest->extractMin());
  delete doubled;
  delete doubledTest;
}

// Main entry for GoogleTest
int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
