// library_code.cc
#include "library_code.h"

#include <unordered_map>

// --------------------------------------------
//                  Problem 3
// --------------------------------------------

void deleteTree(Node *node) {
  if (node == nullptr)
    return;
  deleteTree(node->left);
  deleteTree(node->right);
  delete node;
}

/**
 * @brief Recursive helper function for reconstructBST.
 * Recursively constructs the tree using preorder and inorder traversal
 * information.
 * @param preorder Preorder traversal vector (const reference).
 * @param preorder_index Current index being processed in the preorder traversal
 * (reference). Will be modified during function call.
 * @param inorder Inorder traversal vector (const reference).
 * @param inorder_start Start index of the current subtree's inorder traversal.
 * @param inorder_end End index of the current subtree's inorder traversal.
 * @param inorder_map Hash map mapping inorder values to indices (reference).
 * @return Pointer to the root node of the constructed subtree. Returns nullptr
 * if the subtree is empty.
 */
Node *reconstructBSTHelper(const std::vector<int> &preorder,
                           int &preorder_index, const std::vector<int> &inorder,
                           int inorder_start, int inorder_end,
                           const std::unordered_map<int, int> &inorder_map) {
  if (preorder_index >= preorder.size() || inorder_start > inorder_end) {
    return nullptr;
  }

  int root_value{preorder.at(preorder_index++)};
  Node *root{new Node(root_value)};

  int inorder_index{inorder_map.at(root_value)};

  root->left =
      reconstructBSTHelper(preorder, preorder_index, inorder, inorder_start,
                           inorder_index - 1, inorder_map);
  root->right =
      reconstructBSTHelper(preorder, preorder_index, inorder, inorder_index + 1,
                           inorder_end, inorder_map);

  return root;
}

Node *reconstructBST(const std::vector<int> &preorder,
                     const std::vector<int> &inorder) {
  if (preorder.empty() || inorder.empty() ||
      preorder.size() != inorder.size()) {
    return nullptr;
  }
  std::unordered_map<int, int> inorder_map{};
  for (int i{}; i < inorder.size(); ++i) {
    inorder_map.emplace(inorder.at(i), i);
  }

  int preorder_index{0};

  return reconstructBSTHelper(preorder, preorder_index, inorder, 0,
                              inorder.size() - 1, inorder_map);
}

void getPreorder(const Node *node, std::vector<int> &result) {
  if (node == nullptr)
    return;
  result.push_back(node->value);
  getPreorder(node->left, result);
  getPreorder(node->right, result);
}

void getInorder(const Node *node, std::vector<int> &result) {
  if (node == nullptr)
    return;
  getInorder(node->left, result);
  result.push_back(node->value);
  getInorder(node->right, result);
}

// Question 3: Split BST function
// This function splits the given BST into two BSTs based on the target value.
// Nodes less than or equal to the target belong to the first tree, larger nodes
// to the second tree. The return value is a pair of root node
// pointers for the two subtrees.
std::pair<Node *, Node *> splitBST(Node *root, int target) {
  if(!root){
    // 빈 트리일 경우 nullptr 반환
    return {nullptr, nullptr};
  }

  if(root->value <= target){
    // 현재 루트와 왼쪽 서브트리가 모두 타겟보다 작을 경우 오른쪽 서브트리만 재귀 분할
    auto subTree = splitBST(root->right, target);
    // 루트의 우측에 서브 트리의 좌측 부분을 연결
    root->right = subTree.first;
    return {root, subTree.second};
  } else {
    // 현재 루트와 오른쪽 서브트리가 타겟보다 클 경우 왼쪽 서브트리만 재귀 분할
    auto subTree = splitBST(root->left, target);
    // 루트의 좌측에 서브 트리의 우측 부분을 연결
    root->left = subTree.second;
    return {subTree.first, root};
  }
}

SplitResult solveSplitBST(int target, const std::vector<int> &preorder,
                          const std::vector<int> &inorder) {
  Node *root{reconstructBST(preorder, inorder)};
  auto [left, right]{splitBST(root, target)};

  SplitResult result;
  getPreorder(left, result.preorder1);
  getInorder(left, result.inorder1);
  getPreorder(right, result.preorder2);
  getInorder(right, result.inorder2);

  deleteTree(left);
  deleteTree(right);

  return result;
}

// --------------------------------------------
//                  Problem 4
// --------------------------------------------

std::vector<int> getCDLLSuccessorTraversal(const Node *head) {
  std::vector<int> result;

  if (head == nullptr)
    return result;

  const Node *current{head};
  do {
    result.push_back(current->value);
    current = current->right;
  } while (current != head);

  return result;
}

std::vector<int> getCDLLPredecessorTraversal(const Node *head) {
  std::vector<int> result;

  if (head == nullptr)
    return result;

  const Node *current{head};
  do {
    result.push_back(current->value);
    current = current->left;
  } while (current != head);

  return result;
}

/**
 * @brief Recursive helper function for convertBSTToCDLL.
 * Constructs the CDLL by traversing the BST in inorder and modifying the
 * left/right pointers of the nodes.
 * @param root Pointer to the current node being processed.
 * @param head Pointer to the head node of the CDLL (reference). Will be
 * updated to point to the smallest node.
 * @param prev Pointer to the previously visited node during inorder traversal
 * (reference). Used for linking nodes.
 */
void convertBSTToCDLLHelper(Node *root, Node *&head, Node *&prev) {
  // 루트가 없을 경우 종료
  if(!root){
    return;
  }
  // 왼쪽 서브트리를 순회
  convertBSTToCDLLHelper(root->left, head, prev);

  // 현재 노드에 prev가 없으면 head로 설정
  if (!prev) {
    head = root; // The first node becomes the head of the CDLL
  } else {
    // 있으면 prev와 root를 연결
    prev->right = root;
    root->left = prev;
  }
  // prev를 갱신
  prev = root;

  // 오른쪽 서비트리를 순회
  convertBSTToCDLLHelper(root->right, head, prev);
}

// Question 4: Convert BST to CDLL
// This function converts the given BST into a circular doubly linked list and
// returns the head node.
Node *convertBSTToCDLL(Node *root) {
  // 빈 트리일 경우 nullptr 반환
  if (!root) return nullptr;

  Node *head = nullptr; // CDLL의 head를 저장하는 변수
  Node *prev = nullptr; // 직전 순회 노드를 저장하는 변수

  convertBSTToCDLLHelper(root, head, prev);

  // 마지막 노드와 head를 연결하여 원형으로 완성
  head->left = prev;
  prev->right = head;
  return head;
}

CDLLResult solveBSTToCDLL(const std::vector<int> &preorder,
                          const std::vector<int> &inorder) {
  Node *root{reconstructBST(preorder, inorder)};
  Node *head{convertBSTToCDLL(root)};

  CDLLResult result;
  result.successor_traversal = getCDLLSuccessorTraversal(head);
  result.predecessor_traversal = getCDLLPredecessorTraversal(head);

  if (head != nullptr) {
    Node *current{head->right};
    while (current != head) {
      Node *next{current->right};
      delete current;
      current = next;
    }
    delete head;
  }

  return result;
}

// --------------------------------------------
//                  Problem 5
// --------------------------------------------

int height(const Node *node) {
  if (node == nullptr)
    return 0;
  return node->height;
}

// 노드의 자식 높이를 확인해 높이를 갱신하는 함수
void updateHeight(Node *node) {
  // 노드가 nullptr인 경우 종료
  if (!node) return;
  // 자식 노드의 높이를 가져와서 최대값을 구하고 1을 더해 현재 노드의 높이 갱신
  int hl = height(node->left);
  int hr = height(node->right);
  node->height = std::max(hl, hr) + 1;
}

// 노드의 balance factor를 계산하는 함수
int getBalance(const Node *node) {
  // 노드가 nullptr인 경우 0 반환
  if (!node) return 0;
  // 왼쪽 서브트리 높이에서 오른쪽 서브트리 높이를 빼서 균형 인수 반환
  return height(node->left) - height(node->right);
}

// 우측 회전을 수행하는 함수
Node *rotateRight(Node *y) {
  Node *x = y->left;
  Node *T2 = (x ? x->right : nullptr);

  // 회전 수행
  x->right = y;
  y->left = T2;

  // 높이 갱신
  updateHeight(y);
  updateHeight(x);
  return x;
}

// 좌측 회전을 수행하는 함수
Node *rotateLeft(Node *x) {
  Node *y = x->right;
  Node *T2 = (y ? y->left : nullptr);

  // 회전 수행
  y->left = x;
  x->right = T2;

  // 높이 갱신
  updateHeight(x);
  updateHeight(y);
  return y;
}

// AVL 트리에 키를 삽입하는 함수
Node *insertAVL(Node *node, int key) {
  // 노드가 nullptr인 경우 새 노드 생성
  if (!node) {
    return new Node(key);
  }
  // 키가 현재 노드의 값보다 작으면 왼쪽 서브트리에 삽입, 크면 오른쪽 서브트리에 삽입
  if (key < node->value) {
    node->left  = insertAVL(node->left, key);
  } else if (key > node->value) {
    node->right = insertAVL(node->right, key);
  } else {
    return node;
  }

  // 삽입 후 높이 갱신
  updateHeight(node);

  // 균형 인수 계산
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

AVLResult solveInsertAVL(const std::vector<int> &keys) {
  Node *root{nullptr};
  for (int key : keys) {
    root = insertAVL(root, key);
  }

  AVLResult result;
  getPreorder(root, result.preorder);
  getInorder(root, result.inorder);

  deleteTree(root);

  return result;
}