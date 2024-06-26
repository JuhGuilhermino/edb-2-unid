#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <string>
#include <queue>
#include <iomanip>

namespace common {

  struct Date {
    unsigned short day;
    unsigned short month;
    unsigned short year;

    Date(unsigned short day, unsigned short month, unsigned short year) : day(day), month(month), year(year) {};
    Date() {};

    static std::string to_string(Date date) {
      auto dday = std::to_string(date.day);
      if (dday.size() == 1) dday = "0" + dday;

      auto dmonth = std::to_string(date.month);
      if (dmonth.size() == 1) dmonth = "0" + dmonth;

      auto dyear = std::to_string(date.year);

      return  dday + "/" + dmonth + "/" + dyear;
    }

    friend std::ostream& operator<<(std::ostream& os, const Date& date) {
      os << Date::to_string(date);
      return os;
    }
  };

  template<typename T>
  void inorderTraversal(T* root) {
    if (root != nullptr) {
      inorderTraversal(root->left);
      std::cout << root->data << " ";
      inorderTraversal(root->right);
    }
  }

  template<typename T>
  void printTree(T* root) {
    if (root == nullptr) {
      std::cout << "Árvore vazia!" << std::endl;
      return;
    }

    // Usando uma fila para armazenar os nós de cada nível
    std::queue<T*> q;
    q.push(root);

    while (!q.empty()) {
      int size = q.size();
      bool levelEmpty = true; // Flag para indicar se o nível está vazio

      // Iterar sobre todos os nós no nível atual
      for (int i = 0; i < size; ++i) {
        T* current = q.front();
        q.pop();

        // Imprimir o valor do nó ou um espaço em branco se o nó for nulo
        if (current != nullptr) {
          std::cout << current->data << " ";
          q.push(current->left);
          q.push(current->right);
          // Se houver pelo menos um nó não nulo neste nível, definir a flag para falso
          if (current->left != nullptr || current->right != nullptr) {
            levelEmpty = false;
          }
        }
        else {
          std::cout << "  ";
          q.push(nullptr);
          q.push(nullptr);
        }
      }

      std::cout << std::endl;

      // Se todos os nós no nível atual forem nulos, significa que a árvore terminou
      if (levelEmpty) {
        break;
      }
    }
  }

};

#endif // COMMON_H