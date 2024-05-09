#include <iostream>
#include <memory>
#include <sstream>
#include "../animal.h"

namespace rb {

  enum Color { RED, BLACK };

  template<typename T>
  class Node {
  public:
    int key;
    T value;
    Node<T>* left;
    Node<T>* right;
    Node<T>* parent;
    Color color;

    Node(int k, T val) : key(k), value(val), left(nullptr), right(nullptr), parent(nullptr), color(RED) {}
  };

  template<typename T>
  class RedBlackTree {
    unsigned int m_size;

  public:
    Node<T>* root;
    Node<T>* nil;

    RedBlackTree() : root(nullptr), nil(new Node<T>(-1, nullptr)), m_size(0) {
      nil->color = BLACK;
      root = nil;
    }

    ~RedBlackTree() {
      destroyTree(root);
      delete nil;
    }

    void destroyTree(Node<T>* root) {
      if (root != nil) {
        destroyTree(root->left);
        destroyTree(root->right);
        delete root;
      }
    }

    int size() const { return m_size; }

    void leftRotate(Node<T>* x) {
      Node<T>* y = x->right;
      x->right = y->left;

      if (y->left != nil)
        y->left->parent = x;

      y->parent = x->parent;

      if (x->parent == nil)
        root = y;
      else if (x == x->parent->left)
        x->parent->left = y;
      else
        x->parent->right = y;

      y->left = x;
      x->parent = y;
    }

    void rightRotate(Node<T>* y) {
      Node<T>* x = y->left;
      y->left = x->right;

      if (x->right != nil)
        x->right->parent = y;

      x->parent = y->parent;

      if (y->parent == nil)
        root = x;
      else if (y == y->parent->left)
        y->parent->left = x;
      else
        y->parent->right = x;

      x->right = y;
      y->parent = x;
    }

    void fixInsert(Node<T>* z) {
      while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
          Node<T>* y = z->parent->parent->right;

          if (y->color == RED) {
            z->parent->color = BLACK;
            y->color = BLACK;
            z->parent->parent->color = RED;
            z = z->parent->parent;
          }
          else {
            if (z == z->parent->right) {
              z = z->parent;
              leftRotate(z);
            }

            z->parent->color = BLACK;
            z->parent->parent->color = RED;
            rightRotate(z->parent->parent);
          }
        }
        else {
          Node<T>* y = z->parent->parent->left;

          if (y->color == RED) {
            z->parent->color = BLACK;
            y->color = BLACK;
            z->parent->parent->color = RED;
            z = z->parent->parent;
          }
          else {
            if (z == z->parent->left) {
              z = z->parent;
              rightRotate(z);
            }

            z->parent->color = BLACK;
            z->parent->parent->color = RED;
            leftRotate(z->parent->parent);
          }
        }
      }
      root->color = BLACK;
    }

    Node<T>* insert(Node<T>*& root, T value, int key) {
      Node<T>* z = new Node<T>(key, value);
      Node<T>* y = nil;
      Node<T>* x = root;

      while (x != nil) {
        y = x;

        if (z->key < x->key)
          x = x->left;
        else
          x = x->right;
      }

      z->parent = y;

      if (y == nil)
        root = z;
      else if (z->key < y->key)
        y->left = z;
      else
        y->right = z;

      z->left = nil;
      z->right = nil;
      z->color = RED;
      fixInsert(z);
      ++m_size;

      return root;
    }

    void rbTransplant(Node<T>* u, Node<T>* v) {
      if (u->parent == nil)
        root = v;
      else if (u == u->parent->left)
        u->parent->left = v;
      else
        u->parent->right = v;

      v->parent = u->parent;
    }

    Node<T>* rbMinimum(Node<T>* x) {
      while (x->left != nil)
        x = x->left;
      return x;
    }

    void fixDelete(Node<T>* x) {
      while (x != root && x->color == BLACK) {
        if (x == x->parent->left) {
          Node<T>* w = x->parent->right;

          if (w->color == RED) {
            w->color = BLACK;
            x->parent->color = RED;
            leftRotate(x->parent);
            w = x->parent->right;
          }

          if (w->left->color == BLACK && w->right->color == BLACK) {
            w->color = RED;
            x = x->parent;
          }
          else {
            if (w->right->color == BLACK) {
              w->left->color = BLACK;
              w->color = RED;
              rightRotate(w);
              w = x->parent->right;
            }

            w->color = x->parent->color;
            x->parent->color = BLACK;
            w->right->color = BLACK;
            leftRotate(x->parent);
            x = root;
          }
        }
        else {
          Node<T>* w = x->parent->left;

          if (w->color == RED) {
            w->color = BLACK;
            x->parent->color = RED;
            rightRotate(x->parent);
            w = x->parent->left;
          }

          if (w->right->color == BLACK && w->left->color == BLACK) {
            w->color = RED;
            x = x->parent;
          }
          else {
            if (w->left->color == BLACK) {
              w->right->color = BLACK;
              w->color = RED;
              leftRotate(w);
              w = x->parent->left;
            }

            w->color = x->parent->color;
            x->parent->color = BLACK;
            w->left->color = BLACK;
            rightRotate(x->parent);
            x = root;
          }
        }
      }

      x->color = BLACK;
    }

    void rbDelete(Node<T>* z) {
      Node<T>* y = z;
      Node<T>* x;
      Color originalColor = y->color;

      if (z->left == nil) {
        x = z->right;
        rbTransplant(z, z->right);
      }
      else if (z->right == nil) {
        x = z->left;
        rbTransplant(z, z->left);
      }
      else {
        y = rbMinimum(z->right);
        originalColor = y->color;
        x = y->right;

        if (y->parent == z)
          x->parent = y;
        else {
          rbTransplant(y, y->right);
          y->right = z->right;
          y->right->parent = y;
        }

        rbTransplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
      }

      if (originalColor == BLACK)
        fixDelete(x);

      delete z;
      --m_size;
    }

    Node<T>* deleteNode(Node<T>* root, int key) {
      Node<T>* z = search(root, key);
      if (z != nil)
        rbDelete(z);

      return root;
    }

    Node<T>* search(Node<T>* root, int key) {
      if (root == nil || root->key == key)
        return root;
      else if (key < root->key)
        return search(root->left, key);
      else
        return search(root->right, key);
    }

    void inorder(Node<T>* root) const {
      if (root != nil) {
        inorder(root->left);
        root->value->print();
        inorder(root->right);
      }
    }

    std::string inorder(Node<T>* root, bool save) const {
      std::ostringstream oss;

      if (root != nil) {
        oss << inorder(root->left, true);
        oss << root->value->write();
        oss << inorder(root->right, true);
      }

      return oss.str();
    }
  };

  template class RedBlackTree<std::shared_ptr<Animal>>;
}
