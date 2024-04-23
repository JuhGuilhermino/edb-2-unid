#include <program.h>
#include <common.h>
#include <iostream>
#include <string>

using namespace fauna;

void Program::initialize(int argc, char* argv[]) {
  m_state = e_state::STARTING;
  m_error_msg = "";

  // TODO: Checar parâmetros e ler arquivos
}

bool Program::has_finished() {
  return m_state == e_state::QUITTING;
}

void Program::process_events() {
  if (m_state == e_state::READING_MENU_OPT) {
    std::string line;
    std::getline(std::cin, line);
    m_error_msg = "";

    try
    {
      auto option_selected = std::stoi(line);
      if (option_selected > 7 || option_selected < 1)
        throw std::runtime_error("error");

      m_selected_option = (e_menu_option)option_selected;
    }
    catch (const std::exception& e)
    {
      m_error_msg = "Invalid option.";
      m_selected_option = e_menu_option::INVALID;
    }
  }
}

void Program::update() {
  if (m_selected_option == e_menu_option::HELP) {}
  else if (m_selected_option == e_menu_option::QUIT) {}
  else if (m_selected_option == e_menu_option::INCLUDE_ANIMAL) {}
  else if (m_selected_option == e_menu_option::CONSULT_ANIMAL_HISTORY) {}
  else if (m_selected_option == e_menu_option::SAVE_FILE) {}
  else if (m_selected_option == e_menu_option::REMOVE_ANIMAL) {}
  else if (m_selected_option == e_menu_option::READ_ANIMAL) {}
  else if (m_selected_option == e_menu_option::INVALID) {}
}

void Program::render() const {
  if (m_state == e_state::STARTING) {}
  else if (m_state == e_state::READING_FILE) {}
  else if (m_state == e_state::READING_MENU_OPT) {}
  else if (m_state == e_state::HELPING) {}
  else if (m_state == e_state::QUITTING) {}
  else if (m_state == e_state::INCLUDING_ANIMAL) {}
  else if (m_state == e_state::READING_ANIMAL) {}
  else if (m_state == e_state::REMOVING_ANIMAL) {}
  else if (m_state == e_state::CONSULTING_ANIMAL_HISTORY) {}
  else if (m_state == e_state::SAVING_FILE) {}
}

