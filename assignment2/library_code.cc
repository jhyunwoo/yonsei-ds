// library_code.cc

#include <iostream>
#include <vector>
#include <string>
#include <utility>

// Question 5: Find all duplicates in an array
std::vector<int> findDuplicates(std::vector<int>& nums) {
  std::vector<int> duplicates; // 중복된 숫자를 저장하고 반환하기 위한 배열
  int i = 0; // 현재 인덱스

  // 배열 안의 숫자를 숫자 - 1 인덱스에 있는 값과 swap 하여 정렬 
  // 만약 숫자 - 1 인덱스에 이미 같은 숫자가 있다면 중복된 값이므로 duplicates 배열에 추가 후 값을 0으로 변경
  // 만약 숫자 - 1 인덱스에 0이 있다면 중복된 값이 없으므로 넘어감
  while (i < nums.size()) {
    // 만약 인덱스 위치의 값이 0이라면 다음 인덱스로 넘어감
    if (nums[i] == 0) {
      i++;
      // 만약 배열 인덱스에 알맞은 값이 이미 들어있다면 다음 인덱스로 넘어감
    } else if (i + 1 == nums[i]) {
      i++;
      // 만약 현재 배열 인덱스에 있는 값과 해당 값의 배열 인덱스에 존재한 값이 같을 경우, 
      // 중복으로 숫자가 존재하는 것이므로 현재 배열 인덱스의 값을 0으로 변경하고 duplicates 배열에 추가
    } else if (nums[i] == nums[nums[i] - 1]) {
      nums[nums[i] - 1] = 0;
      duplicates.push_back(nums[i]);
      i++;
    } else {
      // 위 조건에 모두 해당하지 않는다면 현재 배열 인덱스의 값과 해당 값의 인덱스에 해당하는 값을 swap
      std::swap(nums[i], nums[nums[i] - 1]);
    }
  }
  
  // duplicates 배열 반환
  return duplicates;
}

// Question 6: k-th nearest obstacle queries
int abs(int a){
  return a < 0 ? -a : a;
}

class MaxHeap {
public:
  MaxHeap(int k) : maxSize(k) {}

  // heap에 값을 추가하고 heap 조건을 만족하도록 정렬하는 함수
  void insert(int val) {
    // 만약 heap의 크기가 최대 크기보다 작으면 값을 추가한 뒤 정렬 실행
    if (heap.size() < maxSize) {
      heap.push_back(val);
      afterPush(heap.size() - 1);
      // 만약 heap의 크기가 최대 크기와 같으면 값을 추가한 뒤 정렬 실행
    } else if (val < heap[0]) {
      heap[0] = val;
      afterChangeHead(0);
    }
  }

  // heap의 최대 크기와 같으면 최대 크기의 값을 반환, 아니면 -1 반환
  int head() const {
    if (heap.size() == maxSize)
    {
      return heap[0];
    }

    return -1;
  }

private:
  std::vector<int> heap; // heap 저장을 위한 배열
  int maxSize; // heap의 최대 크기

  // heap에 값을 추가한 뒤 정렬하는 함수  
  void afterPush(int idx) {
    while (idx > 0) {
      int parent = (idx - 1) / 2;
      if (heap[parent] >= heap[idx]) break;
      std::swap(heap[parent], heap[idx]);
      idx = parent;
    }
  }

  // heap의 최대 크기의 값을 변경한 뒤 정렬하는 함수
  void afterChangeHead(int idx) {
    const int n = heap.size();
  
    // 현재 인덱스와 왼쪽 자식 인덱스, 오른쪽 자식 인덱스를 비교하여 가장 큰 값을 찾음
    while (true) {
      int largest = idx;
      int left = 2 * idx + 1; 
      int right = 2 * idx + 2;

      // 왼쪽 자식 인덱스가 현재 인덱스보다 크면 왼쪽 자식 인덱스를 가장 큰 값으로 설정
      if (left < n && heap[left] > heap[largest])
      {
        largest = left;
      }
      // 오른쪽 자식 인덱스가 현재 인덱스보다 크면 오른쪽 자식 인덱스를 가장 큰 값으로 설정
      if (right < n && heap[right] > heap[largest])
      {
        largest = right;
      }

      if (largest == idx)
      {
        break;
      }

      // 현재 인덱스와 가장 큰 값을 가진 인덱스를 swap
      std::swap(heap[idx], heap[largest]);
      idx = largest;
    }
  }
};

std::vector<int> resultsArray(std::vector<std::vector<int>>& queries, int k) {
  std::vector<int> results; // 결과를 저장하기 위한 vector
  MaxHeap heap(k); // k번째 가까운 거리를 찾기 위한 MaxHeap

  for (const auto& q : queries) {
    // MaxHeap에 거리를 추가
    heap.insert(abs(q[0]) + abs(q[1]));
    // MaxHeap에서 k번째 가까운 거리를 반환
    results.push_back(heap.head());
  }

  // results 배열 반환
  return results;
}

// Question 7: Priority queue implementation
class PQueue {
public:
    virtual void enqueue(const std::string& elem) = 0; // 요소 추가
    virtual std::string extractMin() = 0; // 최소 요소 추출
    virtual const std::string& min() const = 0; // 최소 요소 반환
};

// Question 7: Implementation 1: Unsorted Vector - VectorPQueue class
class VectorPQueue : public PQueue {
public:
    VectorPQueue();
    ~VectorPQueue();
    void enqueue(const std::string& elem) override; // VectorPQueue 요소 추가 함수
    std::string extractMin() override; // VectorPQueue 최소 요소 추출 함수
    const std::string& min() const override; // VectorPQueue 최소 요소 반환 함수
    static VectorPQueue* merge(VectorPQueue* one, VectorPQueue* two); // VectorPQueue 합치기 함수

private:
    std::vector<std::string> queue; // 요소 저장을 위한 vector
    int findMinIndex() const; // 최소 요소 인덱스 찾기 함수
};

VectorPQueue::VectorPQueue() : PQueue() {
    queue = {}; // vector 초기화
}

VectorPQueue::~VectorPQueue() {
    queue.clear(); // vector 초기화
}

void VectorPQueue::enqueue(const std::string& elem) {
    queue.push_back(elem); // 요소 추가
}

std::string VectorPQueue::extractMin() {
  // 만약 queue가 비어있으면 빈 문자열 반환
  if (queue.empty()) {
    return "";
  }

  // 가장 작은 요소의 인덱스를 찾은 후 값을 저장
  std::string minElem = queue[findMinIndex()];
  // 가장 작은 요소의 인덱스를 제거
  queue.erase(queue.begin() + findMinIndex());
  // 가장 작은 요소 반환
  return minElem;
}

const std::string& VectorPQueue::min() const {
  // 가장 작은 요소 반환
  return queue[findMinIndex()];
}

// VectorPQueue 합치기 함수
VectorPQueue* VectorPQueue::merge(VectorPQueue* first, VectorPQueue* second) {
  // 합친 큐 생성
  VectorPQueue* mergedQueue = new VectorPQueue();
  // 합친 큐에 첫 번째 큐 추가
  mergedQueue->queue = first->queue;
  // 합친 큐에 두 번째 큐 추가
  mergedQueue->queue.insert(mergedQueue->queue.end(), second->queue.begin(), second->queue.end());
  // 합친 큐 반환
  return mergedQueue;
}

// 가장 작은 요소의 인덱스를 찾는 함수
int VectorPQueue::findMinIndex() const {
  // 가장 작은 요소의 인덱스 초기화
  int minIndex = 0;
  // 가장 작은 요소의 인덱스 찾기
  for (int i = 0; i < queue.size(); i++) {
    if (queue[i] < queue[minIndex]) {
      minIndex = i;
    }
  }
  // 가장 작은 요소의 인덱스 반환
  return minIndex;
}


// Question 7: Implementation 2: Binary Heap - HeapPQueue class
class HeapPQueue : public PQueue {
public:
    HeapPQueue();
    ~HeapPQueue();
    void enqueue(const std::string& elem) override; // HeapPQueue 요소 추가 함수
    std::string extractMin() override; // HeapPQueue 최소 요소 추출 함수
    const std::string& min() const override; // HeapPQueue 최소 요소 반환 함수
    static HeapPQueue* merge(HeapPQueue* one, HeapPQueue* two); // HeapPQueue 합치기 함수
    bool isEmpty() const; // HeapPQueue 비어있는지 확인 함수
    int size() const; // HeapPQueue 크기 반환 함수

private:
    static const int initialCapacity = 10; // 초기 heap 크기
    std::string* queue; // heap 저장을 위한 배열
    int capacity; // heap 크기
    int queueSize; // heap 크기
    void expandCapacity(); // heap 크기 증가 함수
    void bubbleUp(int idx); // heap 정렬 함수
    void bubbleDown(int idx); // heap 정렬 함수
};

HeapPQueue::HeapPQueue() : PQueue(), queue(new std::string[initialCapacity]), capacity(initialCapacity), queueSize(0) {
}

HeapPQueue::~HeapPQueue() {
    delete[] queue;
}

void HeapPQueue::enqueue(const std::string& elem) {
  // 만약 heap 크기가 최대 크기와 같으면 heap 크기 증가
  if (queueSize == capacity) {
    expandCapacity();
  }
  // heap에 요소 추가
  queue[queueSize] = elem;
  // heap 정렬
  bubbleUp(queueSize);
  // heap 크기 증가
  queueSize++;
}

std::string HeapPQueue::extractMin() {
  // 만약 heap이 비어있으면 빈 문자열 반환
  if (isEmpty()) {
    return "";
  }
  // 가장 작은 요소 반환
  std::string min = queue[0];
  // 가장 작은 요소 제거
  queue[0] = queue[queueSize - 1];
  // heap 크기 감소
  queueSize--;
  // heap 정렬
  bubbleDown(0);
  // 가장 작은 요소 반환
  return min;
}

// 가장 작은 요소 반환
const std::string& HeapPQueue::min() const {
  return queue[0];
}

HeapPQueue* HeapPQueue::merge(HeapPQueue* one, HeapPQueue* two) {
  HeapPQueue* result = new HeapPQueue();

    // 새로운 크기 계산
    int totalSize = one->queueSize + two->queueSize;
    result->capacity = (result->capacity > totalSize) ? result->capacity : totalSize;    
    delete[] result->queue;
    
    // 새로운 heap 배열 할당
    result->queue = new std::string[result->capacity];

    // 요소 복사
    for (int i = 0; i < one->queueSize; i++) {
        result->queue[i] = one->queue[i];
    }
    for (int i = 0; i < two->queueSize; i++) {
        result->queue[one->queueSize + i] = two->queue[i];
    }

    result->queueSize = totalSize;

    // bubbleDown 함수를 사용하여 정렬
    for (int i = (result->queueSize - 2) / 2; i >= 0; i--) {
        result->bubbleDown(i);
    }

    // 메모리 해제
    if (one == two) {
        delete one;
    } else {
        delete one;
        delete two;
    }

    return result;
}

bool HeapPQueue::isEmpty() const {
  // heap이 비어있으면 true 반환
  return queueSize == 0;
}

int HeapPQueue::size() const {
  // heap 크기 반환
  return queueSize;
}

void HeapPQueue::expandCapacity() {
  // heap 크기 증가
  capacity *= 2;
  // 새로운 heap 생성
  std::string* newElems = new std::string[capacity];
  // 새로운 heap에 기존 heap 추가
  for (int i = 0; i < queueSize; i++) {
    newElems[i] = std::move(queue[i]);
  }
  // 기존 heap 삭제
  delete[] queue;
  // 새로운 heap 배열 할당
  queue = newElems;
}

void HeapPQueue::bubbleUp(int idx) {
  // 만약 인덱스가 0보다 크면 반복
  while (idx > 0) {
    // 부모 인덱스 계산
    int parent = (idx - 1) / 2;
    // 만약 현재 인덱스의 요소가 부모 인덱스의 요소보다 작으면 교환
    if (queue[idx] < queue[parent]) {
      std::swap(queue[idx], queue[parent]);
      idx = parent;
    } else {
      break;
    }
  }
}

void HeapPQueue::bubbleDown(int idx) {
  // 만약 인덱스가 0보다 크면 반복
  while (true) {
    // 왼쪽 자식 인덱스 계산
    int left = 2 * idx + 1;
    // 오른쪽 자식 인덱스 계산
    int right = 2 * idx + 2;
    // 가장 작은 인덱스 초기화
    int smallest = idx;

    // 왼쪽 자식 인덱스가 현재 인덱스보다 작으면 왼쪽 자식 인덱스를 가장 작은 인덱스로 설정
    if (left < queueSize && queue[left] < queue[smallest]) {
      smallest = left;
    }
    // 오른쪽 자식 인덱스가 현재 인덱스보다 작으면 오른쪽 자식 인덱스를 가장 작은 인덱스로 설정
    if (right < queueSize && queue[right] < queue[smallest]) {
      smallest = right;
    }

    // 만약 가장 작은 인덱스가 현재 인덱스와 같지 않으면 교환
    if (smallest != idx) {
      std::swap(queue[idx], queue[smallest]);
      idx = smallest;
    } else {
      break;
    }
  }
}
