#include "program.h"
#include "common.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace fauna;

void Program::usage() const {
  std::cerr
    << "Usage: fauna [-f <file_relative_path>] [-h, --help]\n"
    << "  Opções:\n"
    << "    -f     <file_relative_path> Caminho do arquivo de registro de faun.\n"
    << "    -h, --help       Imprime esse texto de ajuda.\n";
  std::cerr << std::endl;

  exit(0);
}

void Program::read_file() {
  std::ifstream ifs(m_file_path);
  std::ostringstream oss;

  if (ifs.is_open()) {
    oss << ">>> Abrindo arquivo [" << m_file_path << "]\n"
      << ">>> Processando dados...\n";
    // std::cout << Color::tcolor(oss.str(), Color::GREEN);

    while (ifs) {
      // SBoard board;
      // short board_numbers[SB_LINEAR_SIZE];

      // for (int i = 0; i < SB_LINEAR_SIZE; i++)
      //   if (!(ifs >> board_numbers[i]))
      //     break;

      // board.set_board(board_numbers);
      // if (!board.is_board_valid()) {
      //   // coms::Warning("Found a invalid board at position " + std::to_string(m_puzzles.size() + 1) + " of the list.");
      //   continue;
      // }

      // add_board(board);
    }
  }
  else {
    // coms::Error("Can't open the file, make shure the path it's correct and have content.");
  }

  // if (m_puzzles.size() < 0) {
  //   coms::Error("Not valid puzzle found!");
  // }

  // m_player_board.set_puzzle(m_puzzles[0]);

  oss.str("");
  oss << ">>> Arquivo lido com sucesso. " << 10 << " registros encontrados.";
  std::cout << oss.str() << "\n\n";
}


void Program::initialize(int argc, char* argv[]) {
  m_state = e_state::STARTING;
  m_error_msg = "";

  for (int i = 1; i < argc; i++) {
    std::string current_arg{ argv[i] };

    if (current_arg[0] == '-') {
      if (current_arg == "-f") {
        // TODO: ler arquivo
      }
      else if (current_arg == "-h" || current_arg == "--help") {
        usage();
      }
    }
  }
  std::cout << ">>> DEBUG: Inicializado";
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
  else {
    std::string line;
    std::getline(std::cin, line);
  }
}

void Program::update() {
  std::cout << ">>> UPDATE DEBUG: " << m_state << "\n";

  if (m_state == e_state::STARTING) {
    m_state = e_state::READING_MENU_OPT;
  }
  else if (m_state == e_state::READING_MENU_OPT) {
    if (m_selected_option == e_menu_option::HELP) {}
    else if (m_selected_option == e_menu_option::QUIT) {}
    else if (m_selected_option == e_menu_option::INCLUDE_ANIMAL) {}
    else if (m_selected_option == e_menu_option::CONSULT_ANIMAL_HISTORY) {}
    else if (m_selected_option == e_menu_option::SAVE_FILE) {}
    else if (m_selected_option == e_menu_option::REMOVE_ANIMAL) {}
    else if (m_selected_option == e_menu_option::READ_ANIMAL) {}
    else if (m_selected_option == e_menu_option::INVALID) {}
  }
}

void Program::render() const {
  if (m_state == e_state::STARTING) { std::cout << "Iniciou"; }
  else if (m_state == e_state::READING_FILE) { std::cout << "Lendo arquivo"; }
  else if (m_state == e_state::READING_MENU_OPT) {
    print_menu();
  }
  else if (m_state == e_state::HELPING) {}
  else if (m_state == e_state::QUITTING) {}
  else if (m_state == e_state::INCLUDING_ANIMAL) {}
  else if (m_state == e_state::READING_ANIMAL) {}
  else if (m_state == e_state::REMOVING_ANIMAL) {}
  else if (m_state == e_state::CONSULTING_ANIMAL_HISTORY) {}
  else if (m_state == e_state::SAVING_FILE) {}
}

void Program::print_menu() const {
  std::cout << "----------------------- Gestão de fauna do parque -----------------------\n"
    << "| 1: Incluir animal\n"
    << "| 2: Consultar animais\n"
    << "| 3: Ler animal\n"
    << "| 4: Remover animal\n"
    << "| 5: Salvar arquivo\n"
    << "| 6: Ajuda\n"
    << "| 7: Sair\n";
}