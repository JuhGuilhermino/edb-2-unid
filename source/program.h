#ifndef PROGRAM_H
#define PROGRAM_H

#include <vector>
#include <memory>
#include "animal.h"

class Program
{
private:
  using animal = std::shared_ptr<Animal>;
  using fauna = std::vector<animal>;

  enum e_state {
    STARTING,
    READING_FILE,
    READING_MENU_OPT,

    INCLUDING_ANIMAL,
    REMOVING_ANIMAL,
    READING_ANIMAL,
    CONSULTING_ANIMAL_HISTORY,
    SAVING_FILE,
    HELPING,
    QUITTING,
  };

  enum e_menu_option {
    INCLUDE_ANIMAL = 1,
    REMOVE_ANIMAL,
    READ_ANIMAL,
    CONSULT_ANIMAL_HISTORY,
    SAVE_FILE,
    HELP,
    QUIT,
    INVALID,
  };

  e_state m_state;
  std::string m_file_path;
  e_menu_option m_selected_option;
  std::string m_error_msg;

  //!< Methods
  void print_menu() const;
  void print_help();
  void print_exit();

  void print_animals();
  void print_animal_history();

  void read_file();
public:
  void initialize(int argc, char* argv[]);
  bool has_finished();

  void process_events();
  void update();
  void render() const;
  void usage() const;
};

#endif