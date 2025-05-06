// library_code.cc

#include <iostream>
#include <vector>
#include <string>
#include <utility>

void printVector(std::vector<int> listData)
{
  for (int i=0; i<listData.size(); i++)
  {
    std::cout << listData[i] << " ";
  }
  std::cout << std::endl;
}

// Question 5: Find all duplicates in an array
std::vector<int> findDuplicates(std::vector<int>& nums) {
  std::vector<int> duplicates;

  int i=0;
  while (i<nums.size())
  {
    if (nums[i]==0)
    {
      i++;
    }else
    {
      if (i+1 == nums[i])
      {
        i++;
      }else
      {
        if (nums[i] == nums[nums[i]-1])
        {
          nums[nums[i]-1] = 0;
          duplicates.push_back(nums[i]);
          i++;
        }else
        {
          std::swap(nums[i], nums[nums[i] - 1]);
        }
      }
    }
  }

  return duplicates;
}



// Question 6: k-th nearest obstacle queries

class MaxHeap {
public:
  MaxHeap(int k) : maxSize(k) {}

  void insert(int val) {
    if (heap.size() < maxSize) {
      heap.push_back(val);
      heapifyUp(heap.size() - 1);
    } else if (val < heap[0]) {
      heap[0] = val;
      heapifyDown(0);
    }
  }

  int top() const {
    return heap.size() == maxSize ? heap[0] : -1;
  }

private:
  std::vector<int> heap;
  int maxSize;

  void heapifyUp(int idx) {
    while (idx > 0) {
      int parent = (idx - 1) / 2;
      if (heap[parent] >= heap[idx]) break;
      std::swap(heap[parent], heap[idx]);
      idx = parent;
    }
  }

  void heapifyDown(int idx) {
    int n = heap.size();
    while (true) {
      int largest = idx;
      int left = 2 * idx + 1;
      int right = 2 * idx + 2;

      if (left < n && heap[left] > heap[largest]) largest = left;
      if (right < n && heap[right] > heap[largest]) largest = right;

      if (largest == idx) break;

      std::swap(heap[idx], heap[largest]);
      idx = largest;
    }
  }
};

std::vector<int> resultsArray(std::vector<std::vector<int>>& queries, int k) {
  std::vector<int> results;
  MaxHeap heap(k);

  for (const auto& q : queries) {
    int dist = abs(q[0]) + abs(q[1]);
    heap.insert(dist);
    results.push_back(heap.top());
  }

  return results;
}

// // Question 7: Priority queue implementation
// class PQueue {
//   public:
//     virtual void enqueue(const std::string& elem) = 0;
//     virtual std::string extractMin() = 0;
//     virtual const std::string& min() const = 0;
// };
//
// // Question 7: Implementation 1: Unsorted Vector - VectorPQueue class
// class VectorPQueue : public PQueue {
//   public:
//     VectorPQueue();
//     ~VectorPQueue();
//     void enqueue(const std::string& elem) override;
//     std::string extractMin() override;
//     const std::string& min() const override;
//     static VectorPQueue* merge(VectorPQueue* one, VectorPQueue* two);
//   private:
//     // update the private section with the list of
//     // data members and helper methods needed to implement
//     // the vector-based version of the PQueue.
// };
//
// VectorPQueue::VectorPQueue() : PQueue() /* WRITE YOUR CODE (if necessary) */ {
//   // WRITE YOUR CODE
// }
//
// VectorPQueue::~VectorPQueue() {
//   // WRITE YOUR CODE
// }
//
// void VectorPQueue::enqueue(const std::string& elem) {
//   // WRITE YOUR CODE
// }
//
// std::string VectorPQueue::extractMin() {
//   // WRITE YOUR CODE
// }
//
// const std::string& VectorPQueue::min() const {
//   // WRITE YOUR CODE
// }
//
// VectorPQueue* VectorPQueue::merge(VectorPQueue* one, VectorPQueue* two) {
//   // WRITE YOUR CODE
// }
//
// // Question 7: Implementation 2: Binary Heap - HeapPQueue class
// class HeapPQueue : public PQueue {
//   public:
//     HeapPQueue();
//     ~HeapPQueue();
//     void enqueue(const std::string& elem) override;
//     std::string extractMin() override;
//     const std::string& min() const override;
//     static HeapPQueue* merge(HeapPQueue* one, HeapPQueue* two);
//   private:
//     // WRITE YOUR CODE (if necessary)
// };
//
// HeapPQueue::HeapPQueue() : PQueue() /* WRITE YOUR CODE (if necessary) */ {
//   // WRITE YOUR CODE
// }
//
// HeapPQueue::~HeapPQueue() {
//   // WRITE YOUR CODE
// }
//
// void HeapPQueue::enqueue(const std::string& elem) {
//   // WRITE YOUR CODE
// }
//
// std::string HeapPQueue::extractMin() {
//   // WRITE YOUR CODE
// }
//
// const std::string& HeapPQueue::min() const {
//   // WRITE YOUR CODE
// }
//
// HeapPQueue* HeapPQueue::merge(HeapPQueue* one, HeapPQueue* two) {
//   // WRITE YOUR CODE
// }
