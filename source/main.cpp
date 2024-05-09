// #include "lib/avl-tree.h"
#include "program.h"
#include "common.h"
#include <memory>
#include <string>

std::string get_tree(int argc, char* argv[]) {
  for (int i = 1; i < argc; i++) {
    std::string current_arg{ argv[i] };

    if (current_arg[0] == '-') {
      if (current_arg == "-t") {
        std::string tree = argv[i + 1];

        if (tree == "avl") return "avl";
        else if (tree == "br") return "br";
        else return "avl";
      }
    }
  }

  return "avl";
}

template<typename TreeType>
void run_program(int argc, char* argv[]) {
  Program<TreeType> program;
  program.initialize(argc, argv);

  while (!program.has_finished()) {
    program.process_events();
    program.update();
    program.render();
  }
}

int main(int argc, char* argv[])
{
  std::string tree_type = get_tree(argc, argv);

  if (tree_type == "avl") {
    std::cout << ">>> Arvore AVL selecionada.\n";
    run_program<avl::AVLTree<std::shared_ptr<Animal>>>(argc, argv);
  }
  else if (tree_type == "br") {
    std::cout << ">>> Arvore Rubro-Negra selecionada.\n";
    run_program<rb::RedBlackTree<std::shared_ptr<Animal>>>(argc, argv); // TODO: implementar arvore RN e trocar o template
  }

  return 0;
}