namespace RedBlackTree {
  enum e_color { RED, BLACK };

  struct Node {
    int data;
    Node* parent;
    Node* left;
    Node* right;
    e_color color;

    // Construtor
    Node(int val) : data(val), parent(nullptr), left(nullptr), right(nullptr), color(RED) {}
  };

  void rotateLeft(Node*& root, Node* node) {
    Node* y = node->right;
    node->right = y->left;
    if (y->left != nullptr) {
      y->left->parent = node;
    }
    y->parent = node->parent;
    if (node->parent == nullptr) {
      root = y;
    } else if (node == node->parent->left) {
      node->parent->left = y;
    } else {
      node->parent->right = y;
    }
    y->left = node;
    node->parent = y;
  }

  void rotateRight(Node*& root, Node* node) {
    Node* x = node->left;
    node->left = x->right;
    if (x->right != nullptr) {
      x->right->parent = node;
    }
    x->parent = node->parent;
    if (node->parent == nullptr) {
      root = x;
    } else if (node == node->parent->left) {
      node->parent->left = x;
    } else {
      node->parent->right = x;
    }
    x->right = node;
    node->parent = x;
  }

  void fixInsertion(Node*& root, Node* node) {
    while (node != root && node->parent->color == RED) {
      if (node->parent == node->parent->parent->left) {
        Node* y = node->parent->parent->right;
        if (y != nullptr && y->color == RED) {
          node->parent->color = BLACK;
          y->color = BLACK;
          node->parent->parent->color = RED;
          node = node->parent->parent;
        } else {
          if (node == node->parent->right) {
            node = node->parent;
            rotateLeft(root, node);
          }
          node->parent->color = BLACK;
          node->parent->parent->color = RED;
          rotateRight(root, node->parent->parent);
        }
      } else {
        Node* y = node->parent->parent->left;
        if (y != nullptr && y->color == RED) {
          node->parent->color = BLACK;
          y->color = BLACK;
          node->parent->parent->color = RED;
          node = node->parent->parent;
        } else {
          if (node == node->parent->left) {
            node = node->parent;
            rotateRight(root, node);
          }
          node->parent->color = BLACK;
          node->parent->parent->color = RED;
          rotateLeft(root, node->parent->parent);
        }
      }
    }
    root->color = BLACK;
  }

  void insert(Node*& root, int val) {
    Node* z = new Node(val);
    Node* y = nullptr;
    Node* x = root;
    while (x != nullptr) {
      y = x;
      if (z->data < x->data) {
        x = x->left;
      } else {
        x = x->right;
      }
    }
    z->parent = y;
    if (y == nullptr) {
      root = z;
    } else if (z->data < y->data) {
      y->left = z;
    } else {
      y->right = z;
    }
    fixInsertion(root, z);
  }

};