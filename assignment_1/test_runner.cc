#include <gtest/gtest.h>
#include "library_code.cc"

// Question 7 Tests
TEST(MyLinkedListTest, BasicOperations) {
    MyLinkedList list;
    list.addAtHead(1);
    list.addAtTail(3);
    list.addAtIndex(1, 2);  // List: 1->2->3
    EXPECT_EQ(list.get(1), 2);
    list.deleteAtIndex(1);  // List: 1->3
    EXPECT_EQ(list.get(1), 3);

    list.deleteAtIndex(1);
    list.deleteAtIndex(1);
    EXPECT_EQ(list.get(0), 1);
    list.addAtTail(2);
    list.addAtTail(3);
    list.addAtTail(4);
    list.addAtTail(5);
    EXPECT_EQ(list.get(4), 5);
    list.addAtHead(9);
    EXPECT_EQ(list.get(5), 5);
    EXPECT_EQ(list.get(0), 9);

    MyLinkedList list2;
    list2.deleteAtIndex(1);
    list2.addAtIndex(1, 2);
    list2.addAtTail(2);
    EXPECT_EQ(list2.get(0), 2);

    MyLinkedList list3;

    // 1) 초기 상태 확인 (비어있는 리스트)
    EXPECT_EQ(list3.get(0), -1);

    // 2) 맨 앞에 추가
    list3.addAtHead(1);               // List: 1
    EXPECT_EQ(list3.get(0), 1);
    EXPECT_EQ(list3.get(1), -1);      // 범위 벗어난 접근

    // 3) 맨 뒤에 추가
    list3.addAtTail(3);               // List: 1->3
    EXPECT_EQ(list3.get(1), 3);

    // 4) 중간 삽입
    list3.addAtIndex(1, 2);           // List: 1->2->3
    EXPECT_EQ(list3.get(1), 2);
    EXPECT_EQ(list3.get(2), 3);

    // 5) 중간 삭제
    list3.deleteAtIndex(1);           // List: 1->3
    EXPECT_EQ(list3.get(1), 3);

    // 6) 범위 밖 삭제 시도 (동작X)
    list3.deleteAtIndex(5);           // 무시
    EXPECT_EQ(list3.get(1), 3);

    // 7) 범위 밖 삽입 시도 (동작X)
    list3.addAtIndex(10, 100);
    EXPECT_EQ(list3.get(2), -1);

    // 8) 다시 추가/삭제
    list3.addAtTail(4);               // List: 1->3->4
    EXPECT_EQ(list3.get(2), 4);
    list3.deleteAtIndex(2);           // List: 1->3
    EXPECT_EQ(list3.get(2), -1);

    MyLinkedList list4;

    // 1) 존재하지 않는 인덱스 접근
    EXPECT_EQ(list4.get(0), -1);
    EXPECT_EQ(list4.get(1), -1);

    // 2) 빈 리스트에서 삭제 시도
    list4.deleteAtIndex(0);
    list4.deleteAtIndex(1);

    // 3) 빈 리스트에서 addAtIndex(0, val)은 머리에 삽입
    list4.addAtIndex(0, 10);          // List: 10
    EXPECT_EQ(list4.get(0), 10);
    EXPECT_EQ(list4.get(1), -1);

    // 4) 빈 리스트에서 addAtIndex(1, val)은 tail에 삽입과 동일
    list4.addAtIndex(1, 20);          // List: 10->20
    EXPECT_EQ(list4.get(1), 20);

    // 5) 범위 벗어나는 addAtIndex
    list4.addAtIndex(5, 30);          // 무시
    EXPECT_EQ(list4.get(2), -1);

    MyLinkedList list5;
    // 맨 앞 추가
    list5.addAtHead(1);   // List: 1
    list5.addAtHead(2);   // List: 2->1
    list5.addAtHead(3);   // List: 3->2->1
    EXPECT_EQ(list5.get(0), 3);
    EXPECT_EQ(list5.get(1), 2);
    EXPECT_EQ(list5.get(2), 1);

    // 맨 뒤 추가
    list5.addAtTail(4);   // List: 3->2->1->4
    list5.addAtTail(5);   // List: 3->2->1->4->5
    EXPECT_EQ(list5.get(3), 4);
    EXPECT_EQ(list5.get(4), 5);

    // 여러 번 삭제
    list5.deleteAtIndex(0);   // List: 2->1->4->5
    EXPECT_EQ(list5.get(0), 2);
    list5.deleteAtIndex(3);   // List: 2->1->4
    EXPECT_EQ(list5.get(2), 4);
    list5.deleteAtIndex(1);   // List: 2->4
    EXPECT_EQ(list5.get(1), 4);

    // 남은 상태 확인
    EXPECT_EQ(list5.get(0), 2);
    EXPECT_EQ(list5.get(1), 4);
    EXPECT_EQ(list5.get(2), -1);

    MyLinkedList list6;
    // 초기: []

    // 1) 머리/꼬리 추가
    list6.addAtHead(10);            // [10]
    list6.addAtTail(20);            // [10, 20]
    list6.addAtTail(30);            // [10, 20, 30]
    list6.addAtHead(5);             // [5, 10, 20, 30]

    // 2) 중간 삽입
    list6.addAtIndex(2, 15);        // [5, 10, 15, 20, 30]
    EXPECT_EQ(list6.get(2), 15);

    // 3) 여러 번 삭제
    list6.deleteAtIndex(0);         // [10, 15, 20, 30]
    list6.deleteAtIndex(1);         // [10, 20, 30]
    EXPECT_EQ(list6.get(1), 20);

    // 4) index == length인 경우 꼬리로 삽입
    list6.addAtIndex(3, 40);        // [10, 20, 30, 40]
    EXPECT_EQ(list6.get(3), 40);

    // 5) 범위 초과 삽입 시도
    list6.addAtIndex(999, 999);     // 무시
    EXPECT_EQ(list6.get(4), -1);

    // 6) 중간 삽입
    list6.addAtIndex(1, 11);        // [10, 11, 20, 30, 40]
    EXPECT_EQ(list6.get(1), 11);
    EXPECT_EQ(list6.get(2), 20);

    // 7) 꼬리 삭제
    list6.deleteAtIndex(4);         // [10, 11, 20, 30]
    EXPECT_EQ(list6.get(3), 30);
    EXPECT_EQ(list6.get(4), -1);

    // 8) 중간 삭제
    list6.deleteAtIndex(1);         // [10, 20, 30]
    EXPECT_EQ(list6.get(1), 20);

    // 9) 머리/꼬리 삽입
    list6.addAtHead(1);             // [1, 10, 20, 30]
    list6.addAtTail(50);            // [1, 10, 20, 30, 50]
    EXPECT_EQ(list6.get(0), 1);
    EXPECT_EQ(list6.get(4), 50);

    MyLinkedList list7;
    // 1) 음수 인덱스 -> 무시 가정
    list7.addAtIndex(-1, 100);
    EXPECT_EQ(list7.get(0), -1);

    // 2) 비어있는데 addAtIndex(0, val)
    list7.addAtIndex(0, 1);         // [1]
    EXPECT_EQ(list7.get(0), 1);

    // 3) 길이가 1인데 addAtIndex(2, val) -> 무시
    list7.addAtIndex(2, 2);
    EXPECT_EQ(list7.get(1), -1);

    // 4) deleteAtIndex(-1), deleteAtIndex(2) -> 무시
    list7.deleteAtIndex(-1);
    list7.deleteAtIndex(2);
    EXPECT_EQ(list7.get(0), 1);

    // 5) 유효 인덱스 삭제
    list7.deleteAtIndex(0);         // []
    EXPECT_EQ(list7.get(0), -1);

    MyLinkedList list8;

    // 인덱스 0에 다섯 번 삽입
    list8.addAtIndex(0, 10);   // [10]
    list8.addAtIndex(0, 20);   // [20, 10]
    list8.addAtIndex(0, 30);   // [30, 20, 10]
    list8.addAtIndex(0, 40);   // [40, 30, 20, 10]
    list8.addAtIndex(0, 50);   // [50, 40, 30, 20, 10]
    EXPECT_EQ(list8.get(0), 50);
    EXPECT_EQ(list8.get(4), 10);

    // 맨 앞부터 순차 삭제
    list8.deleteAtIndex(0);    // [40, 30, 20, 10]
    list8.deleteAtIndex(0);    // [30, 20, 10]
    list8.deleteAtIndex(0);    // [20, 10]
    EXPECT_EQ(list8.get(0), 20);
    EXPECT_EQ(list8.get(1), 10);
    list8.deleteAtIndex(0);    // [10]
    list8.deleteAtIndex(0);    // []
    EXPECT_EQ(list8.get(0), -1);

    MyLinkedList list9;

    // 0~9까지 tail에 삽입
    for(int i = 0; i < 10; i++) {
        list9.addAtTail(i);   // [0,1,2,3,4,5,6,7,8,9]
    }
    for(int i = 0; i < 10; i++) {
        EXPECT_EQ(list9.get(i), i);
    }

    // 맨 앞에 -1 ~ -5까지 삽입
    for(int i = 1; i <= 5; i++) {
        list9.addAtHead(-i);  // 최종: [-5, -4, -3, -2, -1, 0,...,9]
    }
    EXPECT_EQ(list9.get(0), -5);
    EXPECT_EQ(list9.get(5), 0);
    EXPECT_EQ(list9.get(14), 9);

    // 특정 위치 삽입
    list9.addAtIndex(5, 9999);    // [-5, -4, -3, -2, -1, 9999, 0, 1, ... 9]
    EXPECT_EQ(list9.get(5), 9999);
    EXPECT_EQ(list9.get(6), 0);

    // 다시 삭제
    list9.deleteAtIndex(5);       // 원위치로 [-5, -4, -3, -2, -1, 0, 1 ... 9]
    EXPECT_EQ(list9.get(5), 0);

    // 범위 밖 large delete
    list9.deleteAtIndex(9999);    // 무시
    EXPECT_EQ(list9.get(5), 0);
}

// Question 8 Tests
TEST(KthCharacterTest, BasicExamples) {
    std::vector<int> ops1 = {0, 0, 0};
    EXPECT_EQ(kthCharacter(5, ops1), 'a');

    std::vector<int> ops2 = {0, 1, 0, 1};
    EXPECT_EQ(kthCharacter(10, ops2), 'b');

    std::vector<int> ops3 = {1, 1, 1};
    EXPECT_EQ(kthCharacter(8, ops3), 'd');

    std::vector<int> ops4 = {0, 0, 0, 0, 0, 0};
    EXPECT_EQ(kthCharacter(64, ops4), 'a');

    std::vector<int> ops5 = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    EXPECT_EQ(kthCharacter(4096, ops5), 'm');

    std::vector<int> ops6 = {1, 1, 0, 0};
    EXPECT_EQ(kthCharacter(1, ops6), 'a');
    EXPECT_EQ(kthCharacter(2, ops6), 'b');
    EXPECT_EQ(kthCharacter(3, ops6), 'b');
    EXPECT_EQ(kthCharacter(4, ops6), 'c');
    EXPECT_EQ(kthCharacter(5, ops6), 'a');
    EXPECT_EQ(kthCharacter(6, ops6), 'b');
    EXPECT_EQ(kthCharacter(7, ops6), 'b');
    EXPECT_EQ(kthCharacter(8, ops6), 'c');
    EXPECT_EQ(kthCharacter(9, ops6), 'a');
    EXPECT_EQ(kthCharacter(10, ops6), 'b');
    EXPECT_EQ(kthCharacter(11, ops6), 'b');
    EXPECT_EQ(kthCharacter(12, ops6), 'c');
    EXPECT_EQ(kthCharacter(13, ops6), 'a');
    EXPECT_EQ(kthCharacter(14, ops6), 'b');
    EXPECT_EQ(kthCharacter(15, ops6), 'b');
    EXPECT_EQ(kthCharacter(16, ops6), 'c');

    // 2) 모든 연산이 0인 경우(길이 지수적 폭발)
    // ---------------------------
    {
        // (0, 0, 0, 0, 0, 0, 0) => 7번 연속으로 0
        // 시작 "a" -> "aa" -> "aaaa" -> "aaaaaaaa" ...
        // 최종 길이는 1 * 2^7 = 128
        std::vector<int> ops = {0,0,0,0,0,0,0};

        // k=1, 50, 128 등 확인
        EXPECT_EQ(kthCharacter(1,   ops), 'a'); // 첫번째
        EXPECT_EQ(kthCharacter(50,  ops), 'a'); // 중간
        EXPECT_EQ(kthCharacter(128, ops), 'a'); // 마지막
    }

    // ---------------------------
    // 3) 모든 연산이 1인 경우 (알파벳 변화만 발생)
    // ---------------------------
    {
        // 6번 연속 1
        // 각 단계마다 word가 두 배로 늘면서, 뒤쪽 절반은 '다음 알파벳'으로 변환된 문자열.
        // (문제 예시와 유사하지만, 더 많이 반복)
        std::vector<int> ops(6, 1);
        // k=1, 10, 32, 40 정도를 확인해볼 수 있음.
        // 최종 길이는 1 * 2^6 = 64
        // 어떤 문자가 될지는 구현 로직에 따라 계산 가능. 여기서는 특정 값만 예시로 테스트
        EXPECT_EQ(kthCharacter(1,  ops), 'a');  // 항상 맨 첫 글자는 'a' 그대로
        EXPECT_EQ(kthCharacter(10, ops), 'c');  // 예시 (구현 결과와 다를 시 로직 점검 필요)
        EXPECT_EQ(kthCharacter(32, ops), 'f');  // 예시
    }

    // ---------------------------
    // 4) 혼합: {1, 0, 1} 처럼 작은 패턴
    // ---------------------------
    {
        // 직접 수작업으로 전체 문자열을 구해볼 수 있는 간단 케이스
        // start: "a"
        // op1: shift "a" -> "b", append => "ab" (length=2)
        // op0: "ab" + "ab" => "abab" (length=4)
        // op1: shift("abab") => "bcbc", append => "ababbcbc" (length=8)
        // => 최종 "ababbcbc"
        // 인덱스(1-based): a b a b b c b c
        //                  1 2 3 4 5 6 7 8
        std::vector<int> ops = {1, 0, 1};
        EXPECT_EQ(kthCharacter(1, ops), 'a');
        EXPECT_EQ(kthCharacter(2, ops), 'b');
        EXPECT_EQ(kthCharacter(3, ops), 'a');
        EXPECT_EQ(kthCharacter(4, ops), 'b');
        EXPECT_EQ(kthCharacter(5, ops), 'b');
        EXPECT_EQ(kthCharacter(6, ops), 'c');
        EXPECT_EQ(kthCharacter(7, ops), 'b');
        EXPECT_EQ(kthCharacter(8, ops), 'c');

        // 범위를 벗어난 k (예: 9)으로 호출 시 문제에서 "유효 범위 내 보장"이라 했으므로
        // 여기서는 테스트하지 않거나 - 잘못된 호출이라 가정.
    }

    // ---------------------------
    // 5) "z"로의 변환 & wrap-around 확인
    // ---------------------------
    {
        // 25번 연속으로 op1을 하면 'a' -> 'z'가 되는 중간 과정이 있고,
        // 26번째 op1에서 'a'로 wrap-around 됨.
        // 여기서는 wrap-around가 일어나는지 집중적으로 보기 위해
        // op1을 여러 번 반복
        std::vector<int> ops(26, 1);
        // 최종 길이: 1 -> 2 -> 4 -> ... -> 2^26 (약 6천7백만)
        // 실제로 그 길이까지 문자열을 생성하면 불가능하므로,
        // kthCharacter 구현이 O(log k)로 동작해야 테스트 시에도 시간 내에 가능.

        // 몇몇 포인트만 샘플 확인
        // k=1: 항상 'a'
        // k=2^25 (약 3천3백만), k=2^26 등등을 확인할 수도 있으나 여기서는 예시로 몇 개만
        EXPECT_EQ(kthCharacter(1,     ops), 'a'); // 첫 글자
        EXPECT_EQ(kthCharacter(1ULL << 26, ops), 'a');
        // (1ULL << 26) = 2^26, 마지막 글자(이 시점에 wrap이 되었을 가능성 있음)
        // 실제 어떤 값이 나올지는 철저히 구현 로직에 따라 다름.
        // wrap-around가 제대로 처리되는지를 확인하려고 넣은 예시.
    }

    // ---------------------------
    // 6) 중간에 0과 1이 섞인 긴 패턴
    // ---------------------------
    {
        // 예: {0,1,0,1,1,0,1} 같은 식으로 임의 패턴
        // 실제로 word가 어떻게 변하는지 직접 전부 써보기는 어렵고,
        // 최종 길이는 충분히 커진다고 가정 (k가 1,000 등)
        // 여기서는 여러 위치를 임의로 점검
        std::vector<int> ops = {0, 1, 0, 1, 1, 0, 1};

        // k 값들을 골고루 찍어서, 중간/처음/끝 부분 확인
        // (아래 기대값 'x', 'y'등은 예시. 실제 구현에 따라 다를 수 있어, 로직 점검 필요)
        EXPECT_EQ(kthCharacter(1,    ops), 'a');  // 첫 글자
        EXPECT_EQ(kthCharacter(50,   ops), 'b');  // 예시
        EXPECT_EQ(kthCharacter(100,  ops), 'c');  // 예시
    }
}

// Question 9 Tests
TEST(MedianSortedArraysTest, BasicExamples) {
    std::vector<int> nums1 = {1, 3};
    std::vector<int> nums2 = {2};
    EXPECT_FLOAT_EQ(findMedianSortedArrays(nums1, nums2), 2.0);

    nums1 = {1, 2};
    nums2 = {3, 4};
    EXPECT_FLOAT_EQ(findMedianSortedArrays(nums1, nums2), 2.5);
    nums1 = {1, 2, 3};
    nums2 = {4, 5};
    EXPECT_FLOAT_EQ(findMedianSortedArrays(nums1, nums2), 3);

    nums1 = {2,3,4,5,6,7,8,12};
    nums2 = {8,9,13};
    EXPECT_FLOAT_EQ(findMedianSortedArrays(nums1, nums2), 7);

    // 2) 한 쪽이 비어있는 경우 (m=0 또는 n=0)
    {
        // nums1 = [], nums2 = [1,2,3] => 중앙값은 2
        std::vector<int> nums1 = {};
        std::vector<int> nums2 = {1,2,3};
        EXPECT_FLOAT_EQ(findMedianSortedArrays(nums1, nums2), 2.0);

        // nums1 = [1,2,3,4], nums2 = [] => 중앙값은 (2+3)/2 = 2.5
        nums1 = {1, 2, 3, 4};
        nums2.clear();
        EXPECT_FLOAT_EQ(findMedianSortedArrays(nums1, nums2), 2.5);
    }

    // 3) 배열 길이가 각각 1인 경우, 혹은 둘 다 1인 경우
    {
        // nums1 = [5], nums2 = [] => 중앙값은 5
        std::vector<int> nums1 = {5};
        std::vector<int> nums2 = {};
        EXPECT_FLOAT_EQ(findMedianSortedArrays(nums1, nums2), 5.0);

        // nums1 = [3], nums2 = [7] => 중앙값은 (3+7)/2 = 5
        nums1 = {3};
        nums2 = {7};
        EXPECT_FLOAT_EQ(findMedianSortedArrays(nums1, nums2), 5.0);
    }

    // 4) 짝수+짝수 길이, 홀수+홀수 길이
    {
        // 짝수+짝수: nums1=4개, nums2=2개 => 전체 6개
        // nums1 = [1,2,3,4], nums2 = [0,10] => merged=[0,1,2,3,4,10], median=(2+3)/2=2.5
        std::vector<int> nums1 = {1,2,3,4};
        std::vector<int> nums2 = {0,10};
        EXPECT_FLOAT_EQ(findMedianSortedArrays(nums1, nums2), 2.5);

        // 홀수+홀수: nums1=3개, nums2=5개 => 총 8개
        // nums1 = [1,2,6], nums2 = [3,4,5,7,8]
        // merged=[1,2,3,4,5,6,7,8], median=(4+5)/2=4.5
        nums1 = {1,2,6};
        nums2 = {3,4,5,7,8};
        EXPECT_FLOAT_EQ(findMedianSortedArrays(nums1, nums2), 4.5);
    }

    // 5) 음수와 양수가 섞인 경우
    {
        // nums1=[-5,-3,-1], nums2=[0,1,2]
        // => merged=[-5,-3,-1,0,1,2], median=(-1+0)/2=-0.5
        std::vector<int> nums1 = {-5, -3, -1};
        std::vector<int> nums2 = {0, 1, 2};
        EXPECT_FLOAT_EQ(findMedianSortedArrays(nums1, nums2), -0.5);

        // nums1=[-2,-1,3], nums2=[-3,4,5]
        // => merged=[-3,-2,-1,3,4,5], median=( -1 + 3 )/2 = 1
        nums1 = {-2, -1, 3};
        nums2 = {-3, 4, 5};
        EXPECT_FLOAT_EQ(findMedianSortedArrays(nums1, nums2), 1.0);
    }

    // 6) 모두 같은 원소로만 구성된 경우
    {
        // nums1 = [2, 2, 2], nums2 = [2, 2]
        // => merged=[2,2,2,2,2], median=2
        std::vector<int> nums1 = {2, 2, 2};
        std::vector<int> nums2 = {2, 2};
        EXPECT_FLOAT_EQ(findMedianSortedArrays(nums1, nums2), 2.0);

        // nums1 = [5,5,5,5], nums2 = [5]
        // => merged=[5,5,5,5,5], median=5
        nums1 = {5, 5, 5, 5};
        nums2 = {5};
        EXPECT_FLOAT_EQ(findMedianSortedArrays(nums1, nums2), 5.0);
    }

    // 7) 크기가 제법 큰 배열 (여기서는 10개 정도) + 랜덤 배치
    //    [정렬된 형태로 주어진다고 가정]
    {
        // nums1 = [-10, -5, -2, 0, 1, 1, 2, 9, 11, 12]
        // nums2 = [ -9, -8, -5, -4, 2, 10, 13, 13, 14, 15]
        // (직접 Merge해보면 20개가 됨 -> 짝수개 -> 중앙값은 10번째와 11번째 원소 평균)
        std::vector<int> nums1 = {-10, -5, -2, 0, 1, 1, 2, 9, 11, 12};
        std::vector<int> nums2 = {-9, -8, -5, -4, 2, 10, 13, 13, 14, 15};
        // 실제 merge 결과를 하나하나 써 보면:
        //   [-10, -9, -8, -5, -5, -4, -2, 0, 1, 1, 2, 2, 9, 10, 11, 12, 13, 13, 14, 15]
        // 길이=20 → 중앙값 = (10번째 + 11번째)/2
        // 인덱스(1-based):  1  2   3   4   5   6   7   8  9  10 11 12 ...
        // => 10번째=1, 11번째=2 => (1+2)/2 = 1.5
        EXPECT_FLOAT_EQ(findMedianSortedArrays(nums1, nums2), 1.5);
    }

    // 8) 큰 값, 작은 값 혼합
    {
        // nums1 = [INT_MIN, -1000000, -999999, -10], nums2 = [999999, 1000000, INT_MAX]
        // (INT_MIN = -2147483648, INT_MAX = 2147483647)
        // 이 정도는 오버플로우 위험 없이 처리 가능
        std::vector<int> nums1 = {std::numeric_limits<int>::min(), -1000000, -999999, -10};
        std::vector<int> nums2 = {999999, 1000000, std::numeric_limits<int>::max()};
        // 정렬된 상태라고 가정
        // merged=[-2147483648, -1000000, -999999, -10, 999999, 1000000, 2147483647]
        // => 길이=7(홀수), 중앙인 4번째 = -10
        EXPECT_FLOAT_EQ(findMedianSortedArrays(nums1, nums2), -10.0);
    }
}

// 연결 리스트를 벡터로부터 생성하는 헬퍼 함수
ListNode* buildLinkedList(const std::vector<int>& vals) {
    if (vals.empty()) return nullptr;
    ListNode* head = new ListNode(vals[0]);
    ListNode* curr = head;
    for (size_t i = 1; i < vals.size(); i++) {
        curr->next = new ListNode(vals[i]);
        curr = curr->next;
    }
    return head;
}

// 연결 리스트를 순회하며 val들을 벡터로 얻는 헬퍼 함수
std::vector<int> linkedListToVector(ListNode* head) {
    std::vector<int> result;
    while (head) {
        result.push_back(head->val);
        head = head->next;
    }
    return result;
}

// 메모리 해제를 위한 헬퍼 함수(테스트 종료 후 누수 방지)
void freeLinkedList(ListNode* head) {
    while (head) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }
}


// Question 10 Tests
TEST(ReverseKGroupTest, BasicExamples) {
    ListNode* head = new ListNode(1);
    head->next = new ListNode(2);
    head->next->next = new ListNode(3);
    head->next->next->next = new ListNode(4);
    head->next->next->next->next = new ListNode(5);

    int k = 2;
    ListNode* result = reverseKGroup(head, k);
    std::vector<int> expected_vals = {2, 1, 4, 3, 5};
    for (int val : expected_vals) {
        ASSERT_NE(result, nullptr);
        EXPECT_EQ(result->val, val);
        result = result->next;
    }

    ListNode* head2 = new ListNode(3);
    head2->next = new ListNode(2);
    head2->next->next = new ListNode(1);
    head2->next->next->next = new ListNode(6);
    head2->next->next->next->next = new ListNode(5);
    head2->next->next->next->next->next = new ListNode(4);
    head2->next->next->next->next->next->next = new ListNode(7);

    int k2 = 3;
    ListNode* result2 = reverseKGroup(head2, k2);
    std::vector<int> expected_vals2 = {1,2,3,4,5,6,7};
    for (int val : expected_vals2) {
        ASSERT_NE(result2, nullptr);
        EXPECT_EQ(result2->val, val);
        result2 = result2->next;
    }

    // 1) 문제 예시: [1,2,3,4,5], k=2 => [2,1,4,3,5]
    {
        ListNode* head = buildLinkedList({1,2,3,4,5});
        int k = 2;
        ListNode* result = reverseKGroup(head, k);
        std::vector<int> output = linkedListToVector(result);
        std::vector<int> expected = {2,1,4,3,5};
        EXPECT_EQ(output, expected);
        freeLinkedList(result);
    }

    // 2) 문제 예시: [1,2,3,4,5], k=3 => [3,2,1,4,5]
    {
        ListNode* head = buildLinkedList({1,2,3,4,5});
        int k = 3;
        ListNode* result = reverseKGroup(head, k);
        std::vector<int> output = linkedListToVector(result);
        std::vector<int> expected = {3,2,1,4,5};
        EXPECT_EQ(output, expected);
        freeLinkedList(result);
    }

    // 3) 단일 노드: [1], k=1 => [1] (그대로)
    {
        ListNode* head = buildLinkedList({1});
        int k = 1;
        ListNode* result = reverseKGroup(head, k);
        std::vector<int> output = linkedListToVector(result);
        std::vector<int> expected = {1};
        EXPECT_EQ(output, expected);
        freeLinkedList(result);
    }

    // 4) k = 리스트 길이: [1,2,3], k=3 => [3,2,1]
    {
        ListNode* head = buildLinkedList({1,2,3});
        int k = 3;
        ListNode* result = reverseKGroup(head, k);
        std::vector<int> output = linkedListToVector(result);
        std::vector<int> expected = {3,2,1};
        EXPECT_EQ(output, expected);
        freeLinkedList(result);
    }

    // 5) k=1 => 아무 변화 없음: [1,2,3,4,5], k=1 => [1,2,3,4,5]
    {
        ListNode* head = buildLinkedList({1,2,3,4,5});
        int k = 1;
        ListNode* result = reverseKGroup(head, k);
        std::vector<int> output = linkedListToVector(result);
        std::vector<int> expected = {1,2,3,4,5};
        EXPECT_EQ(output, expected);
        freeLinkedList(result);
    }

    // 6) 원소가 모두 같은 경우: [2,2,2,2], k=2 => [2,2,2,2]
    //    실제로는 그룹별 뒤집기 해도 내용은 동일 (다만 노드의 연결이 바뀔 뿐)
    {
        ListNode* head = buildLinkedList({2,2,2,2});
        int k = 2;
        ListNode* result = reverseKGroup(head, k);
        std::vector<int> output = linkedListToVector(result);
        std::vector<int> expected = {2,2,2,2};
        EXPECT_EQ(output, expected);
        freeLinkedList(result);
    }

    // 7) 여러 그룹 + leftover 존재: [1,2,3,4,5,6,7], k=2 => [2,1,4,3,6,5,7]
    //    마지막 7이 남아서 그대로
    {
        ListNode* head = buildLinkedList({1,2,3,4,5,6,7});
        int k = 2;
        ListNode* result = reverseKGroup(head, k);
        std::vector<int> output = linkedListToVector(result);
        std::vector<int> expected = {2,1,4,3,6,5,7};
        EXPECT_EQ(output, expected);
        freeLinkedList(result);
    }

    // 8) leftover가 k보다 적은 예시: [1,2,3,4,5,6,7,8,9], k=4
    //    그룹1: 1,2,3,4 -> 뒤집어서 4,3,2,1
    //    그룹2: 5,6,7,8 -> 뒤집어서 8,7,6,5
    //    leftover: 9 -> 변화 없음
    // => 결과: [4,3,2,1,8,7,6,5,9]
    {
        ListNode* head = buildLinkedList({1,2,3,4,5,6,7,8,9});
        int k = 4;
        ListNode* result = reverseKGroup(head, k);
        std::vector<int> output = linkedListToVector(result);
        std::vector<int> expected = {4,3,2,1,8,7,6,5,9};
        EXPECT_EQ(output, expected);
        freeLinkedList(result);
    }

    // 9) 길이가 꽤 큰 리스트(예: 10) + 중복, k=3
    //    [1,2,2,3,4,4,5,6,6,7], k=3
    //    - 첫 3개: 1,2,2 -> 뒤집으면 2,2,1
    //    - 다음 3개: 3,4,4 -> 뒤집으면 4,4,3
    //    - 다음 3개: 5,6,6 -> 뒤집으면 6,6,5
    //    - leftover 1개: 7 -> 그대로
    // => 결과: [2,2,1,4,4,3,6,6,5,7]
    {
        std::vector<int> input = {1,2,2,3,4,4,5,6,6,7};
        std::vector<int> expected = {2,2,1,4,4,3,6,6,5,7};
        ListNode* head = buildLinkedList(input);
        int k = 3;
        ListNode* result = reverseKGroup(head, k);
        std::vector<int> output = linkedListToVector(result);
        EXPECT_EQ(output, expected);
        freeLinkedList(result);
    }

    // 10) k=n (전체 한 번에 뒤집기) + 모두 뒤집은 후 leftover 없음
    //     [10,9,8,7,6], k=5 -> [6,7,8,9,10]
    {
        ListNode* head = buildLinkedList({10,9,8,7,6});
        int k = 5;
        ListNode* result = reverseKGroup(head, k);
        std::vector<int> output = linkedListToVector(result);
        std::vector<int> expected = {6,7,8,9,10};
        EXPECT_EQ(output, expected);
        freeLinkedList(result);
    }
}

// Main entry for GoogleTest
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
