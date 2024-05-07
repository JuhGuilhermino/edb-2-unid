#include <iostream>


namespace BinaryTree {
  struct Node {
    int data;
    Node* left;
    Node* right;

    // Construtor
    Node(int val) : data(val), left(nullptr), right(nullptr) {}
  };

  Node* insert(Node* root, int val) {
    if (root == nullptr)
      return new Node(val);
    else {
      if (val < root->data) {
        root->left = insert(root->left, val);
      } else {
        root->right = insert(root->right, val);
      }

      return root;
    }
  }

  Node* search(Node* root, int val) {
    if (root == nullptr || root->data == val) return root;
    else if (val < root->data) return search(root->left, val);
    else return search(root->right, val);
  }

  void inorderTraversal(Node* root) {
    if (root != nullptr) {
      inorderTraversal(root->left);
      std::cout << root->data << " ";
      inorderTraversal(root->right);
    }
  }
};
