namespace AVLTree {
  struct Node {
    int data;
    Node* left;
    Node* right;
    int height;

    Node(int val) : data(val), left(nullptr), right(nullptr), height(1) {}
  };

  int max(int a, int b) {
    return a > b ? a : b;
  }

  int getHeight(Node* node) {
    if (node == nullptr) return 0;
    return node->height;
  }

  void updateNodeHeight(Node* node) {
    if (node == nullptr) return;
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
  }

  int getBalance(Node* node) {
    if (node == nullptr) return 0;
    return getHeight(node->left) - getHeight(node->right);
  }

  Node* rotateLeft(Node* node) {
    Node* root = node->right;
    Node* temp = root->left;

    root->left = node;
    node->right = temp;

    updateNodeHeight(root);
    updateNodeHeight(node);

    return root;
  }

  Node* rotateRight(Node* node) {
    Node* root = node->left;
    Node* temp = root->right;

    root->right = node;
    node->left = temp;

    updateNodeHeight(root);
    updateNodeHeight(node);

    return root;
  }

  Node* insert(Node* root, int val) {
    if (root == nullptr) return new Node(val);

    if (val < root->data) {
      root->left = insert(root->left, val);
    } else {
      root->right = insert(root->right, val);
    }

    updateNodeHeight(root);

    int balance = getBalance(root);

    // Desbalanceamento à esquerda
    if (balance > 1 && val < root->left->data) {
      return rotateRight(root);
    }

    // Desbalanceamento à direita
    if (balance < -1 && val > root->right->data) {
      return rotateLeft(root);
    }

    // Desbalanceamento à esquerda-direita
    if (balance > 1 && val > root->left->data) {
      root->left = rotateLeft(root->left);
      return rotateRight(root);
    }

    // Desbalanceamento à direita-esquerda
    if (balance < -1 && val < root->right->data) {
      root->right = rotateRight(root->right);
      return rotateLeft(root);
    }

    return root;
  }

  Node* search(Node* root, int val) {
    if (root == nullptr || root->data == val) return root;
    if (val < root->data) return search(root->left, val);
    return search(root->right, val);
  }
};
