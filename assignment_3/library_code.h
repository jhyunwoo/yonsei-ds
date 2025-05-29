// library_code.h
#ifndef LIBRARY_CODE_H
#define LIBRARY_CODE_H

#include <vector>

struct Node {
  int value{};
  Node *left{};
  Node *right{};
  int height{};

  explicit Node(int x) : value(x), left(nullptr), right(nullptr), height(1) {}
};

/**
 * @brief Reconstructs a Binary Search Tree (BST) from the given preorder and
 * inorder traversal results.
 * @param preorder Vector containing the preorder traversal result of the tree.
 * @param inorder Vector containing the inorder traversal result of the tree.
 * @return Pointer to the root node of the reconstructed BST. Returns nullptr if
 * the input is invalid.
 */
Node *reconstructBST(const std::vector<int> &preorder,
                     const std::vector<int> &inorder);

/**
 * @brief Converts the given Binary Search Tree (BST) into a Circular Doubly
 * Linked List (CDLL). The conversion is done in-place, modifying the left/right
 * pointers of the nodes.
 * @param root Pointer to the root node of the BST to be converted.
 * @return Pointer to the head (node with the smallest value) of the converted
 * CDLL. Returns nullptr if the input tree is empty.
 */
Node *convertBSTToCDLL(Node *root);

/**
 * @brief Stores the preorder traversal result of the tree starting from the
 * given node into a vector.
 * @param node Pointer to the node to start the traversal (const).
 * @param result Reference to the vector where the traversal result will be
 * stored.
 */
void getPreorder(const Node *node, std::vector<int> &result);
/**
 * @brief Stores the inorder traversal result of the tree starting from the
 * given node into a vector.
 * @param node Pointer to the node to start the traversal (const).
 * @param result Reference to the vector where the traversal result will be
 * stored.
 */
void getInorder(const Node *node, std::vector<int> &result);
/**
 * @brief Returns the traversal result in the successor (right pointer)
 * direction starting from the head of the given Circular Doubly Linked List
 * (CDLL).
 * @param head Pointer to the head node of the CDLL (const).
 * @return Vector containing the successor traversal result. Returns an empty
 * vector if the list is empty.
 */
std::vector<int> getCDLLSuccessorTraversal(const Node *head);
/**
 * @brief Returns the traversal result in the predecessor (left pointer)
 * direction starting from the head of the given Circular Doubly Linked List
 * (CDLL).
 * @param head Pointer to the head node of the CDLL (const).
 * @return Vector containing the predecessor traversal result. Returns an empty
 * vector if the list is empty.
 */
std::vector<int> getCDLLPredecessorTraversal(const Node *head);

/**
 * @brief Recursively deletes all nodes of the tree rooted at the given node to
 * free memory.
 * @param node Pointer to the root node of the tree to be deleted.
 */
void deleteTree(Node *node);

// Question 3: Structure to hold Split BST results
struct SplitResult {
  std::vector<int> preorder1, inorder1;
  std::vector<int> preorder2, inorder2;
};
// Question 4: Structure to hold Convert BST to CDLL results
struct CDLLResult {
  std::vector<int> successor_traversal;
  std::vector<int> predecessor_traversal;
};
// Question 5: Structure to hold AVL Tree traversals after insertions
struct AVLResult {
  std::vector<int> preorder;
  std::vector<int> inorder;
};

/**
 * @brief Splits the given BST into two BSTs based on the target value and
 * returns the traversal results of each tree.
 * @param target The value to split the tree by.
 * @param preorder Preorder traversal result of the original BST.
 * @param inorder Inorder traversal result of the original BST.
 * @return SplitResult structure containing the preorder/inorder traversal
 * results of the two split trees.
 */
SplitResult solveSplitBST(int target, const std::vector<int> &preorder,
                          const std::vector<int> &inorder);
/**
 * @brief Converts the given BST to a CDLL and returns the traversal results.
 * @param preorder Preorder traversal result of the original BST.
 * @param inorder Inorder traversal result of the original BST.
 * @return CDLLResult structure containing the CDLL traversal results and the
 * head pointer.
 */
CDLLResult solveBSTToCDLL(const std::vector<int> &preorder,
                          const std::vector<int> &inorder);

/**
 * @brief Gets the height of a node (returns 0 for nullptr).
 * @param node Pointer to the node.
 * @return Height of the node.
 */
int height(const Node *node);

/**
 * @brief Updates the height of a node based on its children's heights.
 * @param node Pointer to the node.
 */
void updateHeight(Node *node);

/**
 * @brief Gets the balance factor of a node.
 * @param node Pointer to the node.
 * @return Balance factor (height(left) - height(right)). Returns 0 for
 * nullptr.
 */
int getBalance(const Node *node);

/**
 * @brief Performs a right rotation on the subtree rooted with y.
 * @param y The root of the subtree to rotate.
 * @return The new root of the subtree after rotation.
 */
Node *rotateRight(Node *y);

/**
 * @brief Performs a left rotation on the subtree rooted with x.
 * @param x The root of the subtree to rotate.
 * @return The new root of the subtree after rotation.
 */
Node *rotateLeft(Node *x);

/**
 * @brief Inserts a key into the AVL tree rooted with node.
 * @param node The root of the AVL subtree.
 * @param key The key to insert.
 * @return The new root of the subtree after insertion and balancing.
 */
Node *insertAVL(Node *node, int key);

/**
 * @brief Builds an AVL tree by inserting keys one by one and returns its
 * traversals.
 * @param keys A vector of keys to insert into an initially empty AVL tree.
 * @return AVLResult structure containing the preorder and inorder traversals of
 * the final tree.
 */
AVLResult solveInsertAVL(const std::vector<int> &keys);

#endif // LIBRARY_CODE_H