#include <iostream>
#include <algorithm> 
#include <sstream> 
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


    ~AVLTree() {
      destroyTree(root);
    }

    void destroyTree(Node<T>* root) {
      if (root != nullptr) {
        destroyTree(root->left);
        destroyTree(root->right);
        delete root;
      }
    }

    int size() const { return m_size; }

    int height(Node<T>* N) {
      if (N == nullptr)
        return 0;
      return N->height;
    }

    int getBalance(Node<T>* N) {
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
      int balance = getBalance(node);

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
      if (root == nullptr || root->key == key) return root;

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

    std::string inorder(Node<T>* root, bool save) const {
      std::ostringstream oss;

      if (root != nullptr) {
        oss << inorder(root->left, true);
        oss << root->value->write();
        oss << inorder(root->right, true);
      }

      return oss.str();
    }

    Node<T>* minValueNode(Node<T>* node) {
      Node<T>* current = node;
      while (current->left != nullptr)
        current = current->left;
      return current;
    }

    Node<T>* deleteNode(Node<T>* root, int key) {
      if (root == nullptr)
        return root;

      if (key < root->key)
        root->left = deleteNode(root->left, key);
      else if (key > root->key)
        root->right = deleteNode(root->right, key);
      else {
        if (root->left == nullptr || root->right == nullptr) {
          Node<T>* temp = root->left ? root->left : root->right;
          if (temp == nullptr) {
            temp = root;
            root = nullptr;
          }
          else {
            *root = *temp;
            root->key = temp->key;
          }
          delete temp;
        }
        else {
          Node<T>* temp = minValueNode(root->right);
          root->key = temp->key;
          root->value = temp->value;
          --m_size;
          root->right = deleteNode(root->right, temp->key);
        }
      }

      if (root == nullptr)
        return root;

      root->height = 1 + std::max(height(root->left), height(root->right));

      int balance = getBalance(root);

      if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);
      if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
      }
      if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);
      if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
      }

      return root;
    }
  };

  template class AVLTree<std::shared_ptr<Animal>>;
};