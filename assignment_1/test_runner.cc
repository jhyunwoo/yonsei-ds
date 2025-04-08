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
}

// Main entry for GoogleTest
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
