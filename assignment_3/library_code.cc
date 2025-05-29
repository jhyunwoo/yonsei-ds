// library_code.cc
#include "library_code.h"

#include <unordered_map>

// --------------------------------------------
//                  Problem 3
// --------------------------------------------
// 주어진 preorder와 inorder 순회를 바탕으로 BST를 재구성하고,
// target 값을 기준으로 두 개의 BST로 분할(split)하는 기능을 구현합니다.

// Node*로 표현된 트리를 후위 순회(post-order) 방식으로 삭제하여 메모리를 해제합니다.
void deleteTree(Node *node) {
  if (node == nullptr)
    return;                    // 기저: 노드가 없으면 종료
  deleteTree(node->left);     // 왼쪽 서브트리 삭제
  deleteTree(node->right);    // 오른쪽 서브트리 삭제
  delete node;                // 현재 노드 삭제
}

// preorder 인덱스와 inorder 범위를 사용해 해당 서브트리를 재귀적으로 구성합니다.
Node *reconstructBSTHelper(
    const std::vector<int> &preorder,
    int &preorder_index,               // preorder 순회의 현재 위치
    const std::vector<int> &inorder,
    int inorder_start,                 // inorder 배열에서 서브트리 시작 인덱스
    int inorder_end,                   // inorder 배열에서 서브트리 종료 인덱스
    const std::unordered_map<int,int> &inorder_map) {
  // 1) 종료 조건: preorder 인덱스가 범위를 벗어나거나
  //    inorder 범위가 유효하지 않으면 nullptr 반환
  if (preorder_index >= (int)preorder.size() || inorder_start > inorder_end)
    return nullptr;

  // 2) preorder에서 루트 값을 꺼내 새 노드 생성
  int root_value = preorder[preorder_index++];
  Node *root = new Node(root_value);

  // 3) inorder_map으로 루트의 inorder 위치를 찾아 왼쪽/오른쪽 크기 판단
  int inorder_index = inorder_map.at(root_value);

  // 4) 왼쪽 서브트리: inorder_start부터 inorder_index-1까지
  root->left = reconstructBSTHelper(
      preorder, preorder_index,
      inorder, inorder_start, inorder_index - 1,
      inorder_map);
  // 5) 오른쪽 서브트리: inorder_index+1부터 inorder_end까지
  root->right = reconstructBSTHelper(
      preorder, preorder_index,
      inorder, inorder_index + 1, inorder_end,
      inorder_map);

  // 6) 구성된 서브트리의 루트 반환
  return root;
}

// 외부에서 호출하는 BST 재구성 함수
Node *reconstructBST(
    const std::vector<int> &preorder,
    const std::vector<int> &inorder) {
  // 유효성 검사: 크기 불일치 시 nullptr 반환
  if (preorder.empty() || inorder.empty() || preorder.size() != inorder.size())
    return nullptr;

  // inorder 값과 인덱스를 저장한 해시맵 구성 (O(1) 조회용)
  std::unordered_map<int,int> inorder_map;
  inorder_map.reserve(inorder.size());
  for (int i = 0; i < (int)inorder.size(); ++i) {
    inorder_map[inorder[i]] = i;
  }

  // preorder 인덱스를 0으로 초기화하고 전체 범위로 헬퍼 호출
  int preorder_index = 0;
  return reconstructBSTHelper(
      preorder, preorder_index,
      inorder,
      0, inorder.size() - 1,
      inorder_map);
}

// BST를 전위순회하여 result 벡터에 저장합니다.
void getPreorder(const Node *node, std::vector<int> &result) {
  if (!node) return;
  result.push_back(node->value);       // 루트 방문
  getPreorder(node->left, result);     // 왼쪽 서브트리 방문
  getPreorder(node->right, result);    // 오른쪽 서브트리 방문
}

// BST를 중위순회하여 result 벡터에 저장합니다.
void getInorder(const Node *node, std::vector<int> &result) {
  if (!node) return;
  getInorder(node->left, result);      // 왼쪽 서브트리 방문
  result.push_back(node->value);       // 루트 방문
  getInorder(node->right, result);     // 오른쪽 서브트리 방문
}

// --- splitBST 구현 시작 ---
/**
 * 주어진 BST를 target 기준으로 분할합니다.
 * @param root: 분할할 원본 BST의 루트
 * @param target: 분할 기준 값
 * @return pair.first = 값 ≤ target인 트리의 루트
 *         pair.second = 값 > target인 트리의 루트
 */
std::pair<Node*,Node*> splitBST(Node *root, int target) {
  if (!root) {
    // 빈 트리인 경우 두 트리 모두 nullptr
    return {nullptr, nullptr};
  }

  if (root->value <= target) {
    // 현재 루트와 왼쪽 서브트리는 모두 ≤ target 이므로,
    // 오른쪽 서브트리만 재귀 분할
    auto sub = splitBST(root->right, target);
    // root->right에 ≤target 부분을 연결
    root->right = sub.first;
    // 결과: root가 포함된 ≤target 트리와 sub.second (>target 트리)
    return {root, sub.second};
  } else {
    // 현재 루트와 오른쪽 서브트리는 > target 이므로,
    // 왼쪽 서브트리만 재귀 분할
    auto sub = splitBST(root->left, target);
    // root->left에 >target 부분을 연결
    root->left = sub.second;
    // 결과: sub.first (≤target 트리)와 root가 포함된 >target 트리
    return {sub.first, root};
  }
}
// --- splitBST 구현 끝 ---

// solveSplitBST: Problem 3 진입점
SplitResult solveSplitBST(int target,
                          const std::vector<int> &preorder,
                          const std::vector<int> &inorder) {
  // 1) BST 재구성
  Node *root = reconstructBST(preorder, inorder);
  // 2) splitBST로 분할
  auto [left, right] = splitBST(root, target);

  // 3) 분할된 두 트리의 순회 결과 수집
  SplitResult result;
  getPreorder(left,  result.preorder1);
  getInorder(left,   result.inorder1);
  getPreorder(right, result.preorder2);
  getInorder(right,  result.inorder2);

  // 4) 메모리 해제
  deleteTree(left);
  deleteTree(right);
  return result;
}

// --------------------------------------------
//                  Problem 4
// --------------------------------------------
// BST를 순회하며 노드들을 원형 이중 연결 리스트(CDLL)로 변환합니다.

// CDLL의 successor(오른쪽 링크) 방향 순회 결과 반환
std::vector<int> getCDLLSuccessorTraversal(const Node *head) {
  std::vector<int> result;
  if (!head) return result;           // 빈 리스트인 경우 빈 벡터 반환
  const Node *cur = head;
  do {
    result.push_back(cur->value);
    cur = cur->right;
  } while (cur != head);
  return result;
}

// CDLL의 predecessor(왼쪽 링크) 방향 순회 결과 반환
std::vector<int> getCDLLPredecessorTraversal(const Node *head) {
  std::vector<int> result;
  if (!head) return result;
  const Node *cur = head;
  do {
    result.push_back(cur->value);
    cur = cur->left;
  } while (cur != head);
  return result;
}

// 중위순회 도중에 노드를 CDLL 형태로 연결하는 헬퍼 함수
void convertBSTToCDLLHelper(Node *node, Node *&head, Node *&prev) {
  if (!node) return;
  // 1) 왼쪽 서브트리 순회
  convertBSTToCDLLHelper(node->left, head, prev);

  // 2) 현재 노드 처리: prev가 없으면 head로 설정,
  //    있으면 prev<->node 연결
  if (!prev) {
    head = node;      // 가장 작은 노드를 head로 지정
  } else {
    prev->right = node;
    node->left  = prev;
  }
  prev = node;        // prev 갱신

  // 3) 오른쪽 서브트리 순회
  convertBSTToCDLLHelper(node->right, head, prev);
}

// convertBSTToCDLL: Problem 4 진입점
Node *convertBSTToCDLL(Node *root) {
  if (!root) return nullptr;
  Node *head = nullptr;  // CDLL의 head (최솟값)
  Node *prev = nullptr;  // 직전 순회 노드 저장
  convertBSTToCDLLHelper(root, head, prev);
  // 마지막 노드(prev)와 head를 연결하여 원형으로 완성
  head->left   = prev;
  prev->right  = head;
  return head;
}

CDLLResult solveBSTToCDLL(
    const std::vector<int> &preorder,
    const std::vector<int> &inorder) {
  // 1) BST 재구성
  Node *root = reconstructBST(preorder, inorder);
  // 2) CDLL로 변환
  Node *head = convertBSTToCDLL(root);

  // 3) 순회 결과 수집
  CDLLResult result;
  result.successor_traversal   = getCDLLSuccessorTraversal(head);
  result.predecessor_traversal = getCDLLPredecessorTraversal(head);

  // 4) CDLL 메모리 해제
  if (head) {
    Node *cur = head->right;
    while (cur != head) {
      Node *nxt = cur->right;
      delete cur;
      cur = nxt;
    }
    delete head;
  }
  return result;
}

// --------------------------------------------
//                  Problem 5
// --------------------------------------------
// AVL 트리 삽입을 위해 높이, 균형 인수, 회전 함수 등을 정의합니다.

// 노드의 height 값을 반환(노드가 nullptr일 경우 0 반환)
int height(const Node *node) {
  return node ? node->height : 0;
}

// node의 자식 높이를 확인해 node->height 갱신
void updateHeight(Node *node) {
  if (!node) return;
  int hl = height(node->left);
  int hr = height(node->right);
  node->height = std::max(hl, hr) + 1;
}

// node의 balance factor 계산: left 높이 - right 높이
int getBalance(const Node *node) {
  if (!node) return 0;
  return height(node->left) - height(node->right);
}

// 우측 회전 수행: LL 케이스 처리
Node *rotateRight(Node *y) {
  Node *x  = y->left;     // y의 왼쪽 자식
  Node *T2 = (x ? x->right : nullptr); // 회전 후 y가 가질 서브트리

  // 회전
  x->right = y;
  y->left  = T2;
  // 높이 갱신 순서: 아래 노드부터
  updateHeight(y);
  updateHeight(x);
  return x;  // 새로운 서브트리 루트
}

// 좌측 회전 수행: RR 케이스 처리
Node *rotateLeft(Node *x) {
  Node *y  = x->right;    // x의 오른쪽 자식
  Node *T2 = (y ? y->left : nullptr);

  // 회전
  y->left  = x;
  x->right = T2;
  // 높이 갱신
  updateHeight(x);
  updateHeight(y);
  return y;
}

// AVL 트리에 키를 삽입하고 필요 시 회전으로 균형 맞춤
Node *insertAVL(Node *node, int key) {
  // 1) BST 삽입
  if (!node) {
    return new Node(key);  // 새 노드 생성 및 height=1
  }
  if (key < node->value) {
    node->left  = insertAVL(node->left, key);
  } else if (key > node->value) {
    node->right = insertAVL(node->right, key);
  } else {
    return node;           // 중복 키 삽입 없음
  }

  // 2) 재귀에서 올라오며 높이 갱신
  updateHeight(node);

  // 3) 균형 인수 확인
  int bf = getBalance(node);
  // LL
  if (bf > 1 && key < node->left->value)
    return rotateRight(node);
  // RR
  if (bf < -1 && key > node->right->value)
    return rotateLeft(node);
  // LR
  if (bf > 1 && key > node->left->value) {
    node->left = rotateLeft(node->left);
    return rotateRight(node);
  }
  // RL
  if (bf < -1 && key < node->right->value) {
    node->right = rotateRight(node->right);
    return rotateLeft(node);
  }

  return node;
}

// solveInsertAVL: Problem 5 진입점
AVLResult solveInsertAVL(const std::vector<int> &keys) {
  Node *root = nullptr;
  // 순서대로 키 삽입
  for (int k : keys) {
    root = insertAVL(root, k);
  }

  // 최종 트리의 전위 및 중위 순회 결과 수집
  AVLResult result;
  getPreorder(root, result.preorder);
  getInorder(root,  result.inorder);
  // 메모리 해제
  deleteTree(root);
  return result;
}