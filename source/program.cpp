#include "program.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "common.h"
#include <sstream>
#include <memory>
#include <vector>

#define RESET   "\033[0m"
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLD    "\033[1m"       /* Bold */

#pragma region UTILS

std::vector<std::string> tokenize(const std::string& str, const char& separator) {
  std::vector<std::string> tokens;
  std::stringstream strm(str);

  std::string aux;
  while (std::getline(strm, aux, separator)) {
    tokens.push_back(aux);
  }

  return tokens;
}
std::string padStart(const std::string& input, size_t targetLength, char padChar = ' ') {
  if (input.length() >= targetLength) {
    return input;
  }
  return std::string(targetLength - input.length(), padChar) + input;
}

#pragma endregion

#pragma region CONFIG_METHODS

void Program::usage() const {
  std::cerr
    << "Usage: fauna [-f <file_relative_path>] [-h, --help]\n"
    << "  Opções:\n"
    << "    -f     <file_relative_path> Caminho do arquivo de registro de faun.\n"
    << "    -h, --help       Imprime esse texto de ajuda.\n";

  exit(0);
}

void Program::read_file() {
  std::ifstream ifs(m_file_path);
  std::ostringstream oss;

  if (ifs.is_open()) {
    oss << ">>> Abrindo arquivo [" << m_file_path << "]\n"
      << ">>> Processando dados...\n";

    while (ifs) {
      std::string line;
      std::getline(ifs, line);
      if (line.empty()) continue;

      Animal animal = parse_animal(line);
      m_animals.push_back(std::make_shared<Animal>(animal));
    }
  }
  else {
  }

  oss << ">>> Arquivo lido com sucesso. " << m_animals.size() << " registros encontrados.";
  std::cout << oss.str() << "\n\n";
}

void Program::initialize(int argc, char* argv[]) {
  m_state = e_state::STARTING;
  m_error_msg = "";
  m_file_path = ".\\data\\fauna-1.txt";
  for (int i = 1; i < argc; i++) {
    std::string current_arg{ argv[i] };

    if (current_arg[0] == '-') {
      if (current_arg == "-f") {
        std::string file_path = argv[i + 1];
        m_file_path = file_path;

      }
      else if (current_arg == "-h" || current_arg == "--help") {
        usage();
      }
    }
  }
}

bool Program::has_finished() {
  return m_state == e_state::QUITTING;
}

#pragma endregion

#pragma region USER_INTERACTIONS

Animal Program::parse_animal(const std::string& animal_str) {
  Animal animal;

  auto tokens = tokenize(animal_str, ';');

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

  return animal;
}

void Program::read_animal() {
  Animal animal;

  std::string line;
  std::getline(std::cin, line);

  animal = parse_animal(line);

  m_animals.push_back(std::make_shared<Animal>(animal));
  m_msg = "Animal adicionado!";
}

void Program::remove_animal() {
  int animal_id;
  std::string line;
  std::getline(std::cin, line);

  animal_id = std::stoi(line);

  m_msg = "Animal " + std::to_string(animal_id) + " removido!";
}

void Program::save_file() {
  std::ofstream ofs(m_file_path);

  auto itr = m_animals.begin();
  if (ofs.is_open()) {

    while (itr != m_animals.end()) {
      std::shared_ptr<Animal> animal = *itr;

      ofs << animal->write();

      ++itr;
    }

    ofs.close();

    m_msg = "Arquivo salvo!";
  }
  else {
    m_error_msg = "Não foi possível abrir o caminho especificado!";
  }
}

#pragma endregion

void Program::process_events() {
  m_error_msg = "";
  m_msg = "";
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
  else if (m_state == e_state::READING_FILE) {
    read_file();
  }
  else if (m_state == e_state::INCLUDING_ANIMAL) {
    read_animal();
  }
  else if (m_state == e_state::REMOVING_ANIMAL) {
    remove_animal();
  }
  else if (m_state == e_state::STARTING) {

  }
  else if (m_state == e_state::SAVING_FILE) {
    std::getline(std::cin, m_file_path);
    save_file();
  }
  else {
    std::string line;
    std::getline(std::cin, line);
  }
}

void Program::update() {
  // std::cout << ">>> Update: " << m_state << "\n";

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
    else if (m_selected_option == e_menu_option::SAVE_FILE) {
      m_state = e_state::SAVING_FILE;
    }
    else if (m_selected_option == e_menu_option::REMOVE_ANIMAL) {
      m_state = e_state::REMOVING_ANIMAL;
    }
    else if (m_selected_option == e_menu_option::READ_ANIMAL) {}
    else if (m_selected_option == e_menu_option::INVALID) {}
  }
  else if (
    m_state == e_state::HELPING
    || m_state == e_state::INCLUDING_ANIMAL
    || m_state == e_state::CONSULTING_ANIMALS
    || m_state == e_state::CONSULTING_ANIMAL_HISTORY
    || m_state == e_state::REMOVING_ANIMAL
    ) {
    m_state = e_state::READING_MENU_OPT;
  }
  else if (m_state == e_state::WELCOMING) {
    m_state = e_state::READING_MENU_OPT;
  }
  else if (m_state == e_state::STARTING) {
    if (!m_file_path.empty())
      m_state = e_state::READING_FILE;
    else
      m_state = e_state::WELCOMING;
  }
  else if (m_state == e_state::READING_FILE) {
    m_state = e_state::WELCOMING;
  }
  else if (m_state == e_state::SAVING_FILE) {
    m_state = e_state::READING_MENU_OPT;
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
  if (m_state == e_state::WELCOMING) {
    print_welcome();
  }
  else if (m_state == e_state::READING_FILE) {}
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
  else if (m_state == e_state::REMOVING_ANIMAL) {
    std::cout << "Digite o código do animal:\n"
      << ">>> ";
  }
  else if (m_state == e_state::CONSULTING_ANIMAL_HISTORY) {}
  else if (m_state == e_state::SAVING_FILE) {
    print_writing_file();
  }
  else if (m_state == e_state::READING_FILE) {

  }
}

#pragma region RENDER_METHODS

void Program::print_welcome() const {
  std::cout
    << BLUE << "Mensagem de boas vindas\n\n"
    << GREEN << ">>> Pressione Enter para iniciar...";
}

void Program::print_reading_file() const {
  std::cout << YELLOW << ">>> Lendo arquivo (" << BOLD << m_file_path << RESET << YELLOW << ")...\n\n";
}

void Program::print_writing_file() const {
  std::cout << YELLOW
    << "Digite o caminho do arquivo:\n"
    << ">>> ";
}

void Program::print_menu() const {
  std::cout << "\n\n"
    << BLUE << "----------------------- Gestão de fauna do parque ------------------------\n"
    << "| "
    << padStart(std::to_string(1), 4, '0')
    << " Animais cadastrados                                               |\n"
    << "| Selecione uma opção:                                                   |\n"
    << "|                                                                        |\n"
    << "| 1: Incluir animal                                                      |\n"
    << "| 2: Consultar animais                                                   |\n"
    << "| 3: Ler animal                                                          |\n"
    << "| 4: Remover animal                                                      |\n"
    << "| 5: Salvar arquivo                                                      |\n"
    << "| 6: Ajuda                                                               |\n"
    << "| 7: Sair                                                                |\n"
    << "--------------------------------------------------------------------------\n\n"
    << RED << "ERROR: [" << m_error_msg << "]\n" << RESET
    << YELLOW << "MSG: [" << m_msg << "]\n\n" << RESET
    << ">>> ";
}

void Program::print_include_animal() const {
  std::cout
    << BLUE << BOLD << "Inclusão de animal\n" << RESET
    << YELLOW << "Insira os dados no formato:\n"
    << "<ID (numérico)>;<Nome>;<Espécie>;<Gênero>;<Data de Monitoramento (dd/MM/yyyy)>;<Data de Nascimento (dd/MM/yyyy)>\n\n"
    << RESET << ">>> ";
}

void Program::print_animals() const {
  auto itr = m_animals.begin();
  std::cout << BLUE << BOLD << "Animais:\n\n" << RESET;

  while (itr != m_animals.end()) {
    std::shared_ptr<Animal> animal = *itr;

    animal->print();

    ++itr;
  }

  std::cout << GREEN << "\n>>> Pressione Enter para voltar ao menu...\n";
}

void Program::print_help() const {
  std::cout
    << BLUE << "<HELP>\n"
    << GREEN << ">>> Pressione Enter para voltar ao menu...\n";
}

void Program::print_exit() const {
  std::cout
    << "<QUITTING>\n\n";
}

#pragma endregion