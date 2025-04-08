// library_code.cc

#include <iostream>
#include <vector>


// Question 7: Linked List Implementation
/**
 * Linked List를 구현하기 위한 Node Class
 */
class Node {
  public:
    int val; // 값 저장하는 변수
    Node* next; // 다음 노드 주소를 저장하는 포인터
    Node(int x) : val(x), next(nullptr) {} // Head 노드 생성자
    Node( int x, Node* next) : val(x), next(next) {} // 일반 노드 생성자
};

class MyLinkedList {
public:
    // 첫 번째 노드 주소를 저장하는 노드
    Node* head;
    MyLinkedList() {
        // 생성자에서 head를 nullptr로 초기화
        head = nullptr;
    }

    // 입력한 인덱스에 존재하는 값을 반환하는 함수
    int get(const int index) const {
        // 노드를 가리키는 포인터를 저장하는 변수
      Node* current = head;

        // 인덱스에 존재하는 값을 찾기 위해 다음 노드의 포인터를 찾는 과정을 index 번 반복
        for(int i=0; i<index; i++){
            // 만약 다음 노드가 존재하지 않을 경우 -1을 반환
            if(current == nullptr){
                return -1;
            }
            // 다음 노드 포인터를 current에 저장
            current = current->next;
        }
        // index 번째 노드의 값을 반환
        return current->val;
    }

    // 리스트의 맨 앞에 노드를 추가하는 함수
    void addAtHead(const int val) {
        // 새로운 노드 생성
        Node* newNode = new Node(val, head);
        // 맨 앞에 노드를 추가
        head = newNode;
    }

    // 리스트의 맨 뒤에 노드를 추가하는 함수
    void addAtTail(const int val) {
        // 새로운 노드 생성
        Node* newNode = new Node(val);
        // 만약 첫 번째 노드가 없다면 첫 번째 노드에 새로 만든 노드를 연결
        if (head == nullptr) {
            head = newNode;
            return;
        }

        Node* curr = head; // 노드 포인터를 저장하는 변수
        // 마지막 노드가 나올 때 까지 반복
        while (curr->next != nullptr) {
            curr = curr->next; // 다음 노드의 주소 저장
        }
        // 마지막 노드의 next에 새로 만든 노드를 연결
        curr->next = newNode;
    }
    
    void addAtIndex(const int index, const int val) {
        // 만약 노드를 추가하는 index가 0일경우 addAtHead를 호출
        if (index == 0) {
            addAtHead(val);
            return;
        }
        // 노드를 추가할 위치 하나 전의 노드 주소를 저장할 변수
        Node* prev = head;
        // 노드를 추가할 위치 하나 전의 노드를 찾아서 주소를 저장
        for(int i = 0; i < index - 1; ++i){
            if(prev == nullptr) return;
            prev = prev->next;
        }
        // 만약 이전 노드가 없을 경우 함수 종료
        if (prev == nullptr) return;
        // 새로운 노드 생성
        Node* newNode = new Node(val, prev->next);
        // 이전 노드의 next에 새로 만든 노드를 연결
        prev->next = newNode;
    }
    
    void deleteAtIndex(const int index) {
        // 만약 연결된 노드가 존재하지 않을 경우 함수 종료
        if (head == nullptr) return;

        // 만약 지울 노드가 첫 번째 일 경우 head를 다음 노드로 변경
        if (index == 0) {
            Node* toDelete = head;
            head = head->next;
            delete toDelete;
            return;
        }

        // 지울 노드 하나 전의 노드 포인터를 저장할 변수
        Node* prev = head;
        // 지울 노드 하나 전의 노드의 주소를 찾아서 저장
        for (int i = 0; i < index - 1; i++) {
            // 노드가 없을 경우 종료
            if (prev == nullptr || prev->next == nullptr) return;
            prev = prev->next;
        }

        // 지울 노드의 주소
        Node* toDelete = prev->next;
        // 만약 지울 노드가 없을 경우 종료
        if (toDelete == nullptr) return;
        // 지울 노드 하나 전의 노드가 가르키는 주소를 지울 노드의 다음 노드 주소로 변경
        prev->next = toDelete->next;
        // 지울 노드 메모리 해제
        delete toDelete;
    }
};

/**
 * Your MyLinkedList object will be instantiated and called as such:
 * MyLinkedList* obj = new MyLinkedList();
 * int param_1 = obj->get(index);
 * obj->addAtHead(val);
 * obj->addAtTail(val);
 * obj->addAtIndex(index,val);
 * obj->deleteAtIndex(index);
 */

// Question 8: kth character after operations
char kthCharacter(long long k, std::vector<int>& operations) {
    long long pos = k - 1; // 리스트에서 사용하기 위한 index로 변환

    char result = 'a'; // 결과 저장을 위한 변수

    int count = 0;
    while (pos>=1)
    {
        if (pos%2==1 && operations[count]==1)
        {
            if (result =='z')
            {
                result = 'a';
            }else
            {
                result++;
            }
        }
        // operation에 의해 배열이 2배씩 증가하므로 pos를 2로 나누어 k번째 문자가 복사된 과정을 역추적
        pos /= 2;
        count++;
    }

    return result;
}

// Question 9: Median of two sorted arrays
int INTMAX = 2147483647;
int INTMIN = -2147483648;

double findMedianSortedArrays(std::vector<int>& nums1, std::vector<int>& nums2) {
    // 첫 번째 배열이 더 작은 사이즈일 경우를 가정하고 이진 탐색을 진행하기 위해 nums1과 nums2의 크기를 비교
    if (nums1.size() > nums2.size())
    {
        // 만약 nums1의 크기가 더 클 경우 재귀 호출을 통해 배열의 순서를 바꿈
        return findMedianSortedArrays(nums2, nums1);
    }

    int x = nums1.size(); // nums1의 크기
    int y = nums2.size(); // nums2의 크기

    // 이진 탐색 기준점
    int low = 0;
    int high = x;

    // 이진탐색 알고리즘을 활용하여 중앙값을 찾음
    while (low <= high)
    {
        // 탐색 기준점 계산
        int partitionX = (low + high) / 2;
        int partitionY = (x + y + 1) / 2 - partitionX;

        // 탐색 결과 값을 저장
        int maxLX = partitionX == 0 ? INTMIN : nums1[partitionX-1];
        int minRX = partitionX == x ? INTMAX : nums1[partitionX];
        int maxLY = partitionY == 0 ? INTMIN : nums2[partitionY-1];
        int minRY = partitionY == y ? INTMAX : nums2[partitionY];

        // 이진탐색 조건을 만족할 경우
        if (maxLX <= minRY && maxLY <= minRX)
        {
            // 배열의 크기의 합이 짝수일 경우
            if ((x+y)%2==0)
            {
                return (std::max(maxLX, maxLY) + std::min(minRX, minRY)) / 2.0;
            }
            // 배열의 크기의 합이 홀수일 경우
            return std::max(maxLX, maxLY);
        }

        // 이진탐색 조건을 만족하지 못한 경우
        if (maxLX > minRY) // 탐색 기준점이 너무 오른쪽에 있을 경우
        {
            high = partitionX-1; // 탐색 기준점을 왼쪽으로 한 칸 이동
        }else // 탐색 기준점이 너무 왼쪽에 있을 경우
        {
            low = partitionX+1; // 탐색 기준점을 오른쪽으로 한 칸 이동
        }
    }

    // 불가능한 경우
    return 0.0;
}

// Question 10: Reverse linked list k nodes
// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};
    
ListNode* reverseKGroup(ListNode* head, int k) {
    // 연결된 노드가 없거나 k가 1일 경우 바로 반환
    if (head == nullptr || k == 1) return head;

    // 기존 노드를 저장하기 위한 dummy 노드 생성
    ListNode dummy(0);
    dummy.next = head;
    ListNode* prevGroupEnd = &dummy;

    while (true) {
        // 그룹의 끝을 찾음
        ListNode* endNode = prevGroupEnd;
        for (int i = 0; i < k && endNode != nullptr; i++) {
            endNode = endNode->next;
        }
        if (endNode == nullptr) break; // k개가 안 되면 종료

        // 그룹 시작과 다음 그룹의 시작 저장
        ListNode* groupStartNode = prevGroupEnd->next;
        ListNode* nextGroupStartNode = endNode->next;

        // k개 노드 뒤집기
        ListNode* prevNode = nextGroupStartNode;
        ListNode* currentNode = groupStartNode;
        while (currentNode != nextGroupStartNode) {
            ListNode* tmpNode = currentNode->next;
            currentNode->next = prevNode;
            prevNode = currentNode;
            currentNode = tmpNode;
        }

        // 이전 그룹과 연결
        prevGroupEnd->next = endNode;
        prevGroupEnd = groupStartNode;
    }

    return dummy.next;
}


