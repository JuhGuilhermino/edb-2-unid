#include <iostream>
#include <algorithm> // Para std::max
#include "../animal.h"
namespace avl {

  template <typename T>
  class Node {
  public:
    int key;
    T value;
    Node<T>* left;
    Node<T>* right;
    int height;

    Node(int k, T val) : key(k), value(val), left(nullptr), right(nullptr), height(1) {}
  };

  template <typename T>
  class AVLTree {
    unsigned int m_size;

  public:
    Node<T>* root;

    AVLTree() : root(nullptr), m_size(0) {}

    int size() const { return m_size; }

    int height(Node<T>* N) {
      if (N == nullptr)
        return 0;
      return N->height;
    }

    int getBalanceFactor(Node<T>* N) {
      if (N == nullptr)
        return 0;
      return height(N->left) - height(N->right);
    }

    Node<T>* rightRotate(Node<T>* y) {
      Node<T>* x = y->left;
      Node<T>* T2 = x->right;

      x->right = y;
      y->left = T2;

      y->height = std::max(height(y->left), height(y->right)) + 1;
      x->height = std::max(height(x->left), height(x->right)) + 1;

      return x;
    }

    Node<T>* leftRotate(Node<T>* x) {
      Node<T>* y = x->right;
      Node<T>* T2 = y->left;

      y->left = x;
      x->right = T2;

      x->height = std::max(height(x->left), height(x->right)) + 1;
      y->height = std::max(height(y->left), height(y->right)) + 1;

      return y;
    }

    Node<T>* insert(Node<T>* node, T value, int key) {
      if (node == nullptr) {
        ++m_size;
        return(new Node<T>(key, value));
      }

      if (key < node->key)
        node->left = insert(node->left, value, key);
      else if (key > node->key)
        node->right = insert(node->right, value, key);
      else // Chaves duplicadas não são permitidas
        return node;

      // Atualiza altura deste nó ancestral
      node->height = 1 + std::max(height(node->left), height(node->right));

      // Verifica se este nó se tornou desbalanceado
      int balance = getBalanceFactor(node);

      // Se o nó se tornou desbalanceado, então há 4 casos

      // Left Left Case
      if (balance > 1 && key < node->left->key)
        return rightRotate(node);

      // Right Right Case
      if (balance < -1 && key > node->right->key)
        return leftRotate(node);

      // Left Right Case
      if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
      }

      // Right Left Case
      if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
      }

      return node;
    }

    Node<T>* search(Node<T>* root, int key) {
      if (root->key == key) return root;

      else if (key < root->key)
        return search(root->left, key);
      else
        return search(root->right, key);
    }

    void inorder(Node<T>* root) const {
      if (root != nullptr) {
        inorder(root->left);

        root->value->print();

        inorder(root->right);
      }
    }
  };

  template class AVLTree<std::shared_ptr<Animal>>;
};