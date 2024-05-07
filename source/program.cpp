#include "program.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "common.h"
#include <sstream>
#include <memory>
#include <vector>


#pragma region CONFIG_METHODS

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

#pragma endregion

#pragma region USER_INTERACTIONS

std::vector<std::string> tokenize(const std::string& str, const char& separator) {
  std::vector<std::string> tokens;
  std::stringstream strm(str);

  std::string aux;
  while (std::getline(strm, aux, separator)) {
    tokens.push_back(aux);
  }

  return tokens;
}

void Program::read_animal() {
  Animal animal;

  std::string line;
  std::getline(std::cin, line);

  auto tokens = tokenize(line, ';');

  animal.m_id = std::stoi(tokens.at(0));
  animal.m_name = tokens.at(1);
  animal.m_species = tokens.at(2);
  animal.m_genre = tokens.at(3);

  auto monitoring = tokenize(tokens.at(4), '/');
  auto birth = tokenize(tokens.at(5), '/');

  common::Date monitoring_at(std::stoi(monitoring.at(0)), std::stoi(monitoring.at(1)), std::stoi(monitoring.at(2)));
  common::Date birth_date(std::stoi(birth.at(0)), std::stoi(birth.at(1)), std::stoi(birth.at(2)));
  animal.m_monitored_at = monitoring_at;
  animal.m_born_at = birth_date;

  m_animals.push_back(std::make_shared<Animal>(animal));
}

#pragma endregion

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

      std::cout << "\n";
    }
    catch (const std::exception& e)
    {
      m_error_msg = "Invalid option.";
      m_selected_option = e_menu_option::INVALID;
    }
  }
  else if (m_state == e_state::INCLUDING_ANIMAL) {
    read_animal();
  }
  else {
    std::string line;
    std::getline(std::cin, line);
  }
}

void Program::update() {
  if (m_state == e_state::READING_MENU_OPT) {
    if (m_selected_option == e_menu_option::HELP)
      m_state = e_state::HELPING;
    else if (m_selected_option == e_menu_option::QUIT)
      m_state = e_state::QUITTING;
    else if (m_selected_option == e_menu_option::INCLUDE_ANIMAL) {
      m_state = e_state::INCLUDING_ANIMAL;
    }
    else if (m_selected_option == e_menu_option::CONSULT_ANIMALS) {
      m_state = e_state::CONSULTING_ANIMALS;
    }
    else if (m_selected_option == e_menu_option::SAVE_FILE) {}
    else if (m_selected_option == e_menu_option::REMOVE_ANIMAL) {}
    else if (m_selected_option == e_menu_option::READ_ANIMAL) {}
    else if (m_selected_option == e_menu_option::INVALID) {}
  }
  else if (
    m_state == e_state::HELPING
    || m_state == e_state::STARTING
    || m_state == e_state::INCLUDING_ANIMAL
    || m_state == e_state::CONSULTING_ANIMALS
    || m_state == e_state::CONSULTING_ANIMAL_HISTORY
    ) {
    m_state = e_state::READING_MENU_OPT;
  }
  else if (m_state == e_state::SAVING_FILE) {

  }
  else if (m_state == e_state::REMOVING_ANIMAL) {

  }
  else if (m_state == e_state::READING_ANIMAL) {

  }
  else if (m_state == e_state::QUITTING) {
    // DO NOTHING
  }
}

void Program::render() const {
  if (m_state == e_state::STARTING) { std::cout << "Iniciou"; }
  else if (m_state == e_state::READING_FILE) { std::cout << "Lendo arquivo"; }
  else if (m_state == e_state::READING_MENU_OPT) {
    print_menu();
  }
  else if (m_state == e_state::HELPING) {
    print_help();
  }
  else if (m_state == e_state::QUITTING) {
    print_exit();
  }
  else if (m_state == e_state::INCLUDING_ANIMAL) {
    print_include_animal();
  }
  else if (m_state == e_state::CONSULTING_ANIMALS) {
    print_animals();
  }
  else if (m_state == e_state::READING_ANIMAL) {}
  else if (m_state == e_state::REMOVING_ANIMAL) {}
  else if (m_state == e_state::CONSULTING_ANIMAL_HISTORY) {}
  else if (m_state == e_state::SAVING_FILE) {}
}

#pragma region RENDER_METHODS

void Program::print_menu() const {
  std::cout << "----------------------- Gestão de fauna do parque -----------------------\n"
    << "| Selecione uma opção:\n"
    << "| 1: Incluir animal\n"
    << "| 2: Consultar animais\n"
    << "| 3: Ler animal\n"
    << "| 4: Remover animal\n"
    << "| 5: Salvar arquivo\n"
    << "| 6: Ajuda\n"
    << "| 7: Sair\n\n"
    << ">>> ";
}

void Program::print_include_animal() const {
  std::cout
    << "Inclusão de animal: \n"
    << ">>> Insira os dados no formato:\n"
    << "<ID (numérico)>;<Nome>;<Espécie>;<Gênero>;<Data de Monitoramento (dd/MM/yyyy)>;<Data de Nascimento (dd/MM/yyyy)>\n";
}

void Program::print_animals() const {
  auto itr = m_animals.begin();
  std::cout << "Animais: \n";

  while (itr != m_animals.end()) {
    std::shared_ptr<Animal> animal = *itr;

    animal->print();

    ++itr;
  }

  std::cout << "\n>>> Pressione Enter para continuar...\n";
}

void Program::print_help() const {
  std::cout
    << "<HELP>\n"
    << ">>> Pressione Enter para continuar\n";
}

void Program::print_exit() const {
  std::cout
    << "<QUITTING>\n";
}

#pragma endregion