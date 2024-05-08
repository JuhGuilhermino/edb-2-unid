#ifndef PROGRAM_H
#define PROGRAM_H

#include <vector>
#include <memory>
#include "animal.h"
#include "lib/avl-tree.h"

template<typename Tree>
class Program
{
private:
  // using animal = std::shared_ptr<Animal>;
  // using fauna = Tree<animal>;

  enum e_state {
    STARTING,
    READING_FILE,
    READING_MENU_OPT,
    WELCOMING,

    INCLUDING_ANIMAL,
    REMOVING_ANIMAL,
    READING_ANIMAL,
    CONSULTING_ANIMALS,
    CONSULTING_ANIMAL_HISTORY,
    SAVING_FILE,
    HELPING,
    QUITTING,
  };

  enum e_menu_option {
    INCLUDE_ANIMAL = 1,
    CONSULT_ANIMALS,
    READ_ANIMAL,
    REMOVE_ANIMAL,
    SAVE_FILE,
    HELP,
    QUIT,
    INVALID,
  };

  e_state m_state;
  std::string m_file_path;
  e_menu_option m_selected_option;
  std::string m_error_msg;
  std::string m_msg;
  Tree m_animals;

  //!< Methods
  void print_welcome() const;
  void print_menu() const;
  void print_help() const;
  void print_exit() const;
  void print_reading_animal() const;
  void print_reading_file() const;
  void print_writing_file() const;
  void print_include_animal() const;

  void print_animals() const;
  void print_animal_history() const;

  void read_file();
  void save_file();
  void read_animal();
  void search_animal();
  void remove_animal();

  Animal parse_animal(const std::string& animals_str);

public:
  void initialize(int argc, char* argv[]);
  bool has_finished();

  void process_events();
  void update();
  void render() const;
  void usage() const;
};

template class Program<avl::AVLTree<std::shared_ptr<Animal>>>;

#endif