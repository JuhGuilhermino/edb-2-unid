#include "program.h"
#include <iostream>
#include <chrono>
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

template<typename Func>
long long measure_execution_time(Func func) {
  auto start = std::chrono::high_resolution_clock::now();

  func();

  auto end = std::chrono::high_resolution_clock::now();
  return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

#pragma endregion

#pragma region CONFIG_METHODS

template <typename Tree>
void Program<Tree>::usage() const {
  std::cerr
    << "Usage: fauna [-f <file_relative_path>] [-h, --help]\n"
    << "  Opções:\n"
    << "    -f     <file_relative_path> Caminho do arquivo de registro de faun.\n"
    << "    -h, --help           Imprime esse texto de ajuda.\n"
    << "    -t, --tree <avl, br> Tipo de árvore utilizada para armazenar os dados. AVL ou Rubro-Negra.\n";

  exit(0);
}

template <typename Tree>
void Program<Tree>::read_file() {
  std::ifstream ifs(m_file_path);
  std::ostringstream oss;

  if (ifs.is_open()) {
    oss << BLUE << "\n>>> Abrindo arquivo [" << m_file_path << "]\n"
      << ">>> Processando dados...\n";

    while (ifs) {
      std::string line;
      std::getline(ifs, line);
      if (line.empty()) continue;

      auto tokens = tokenize(line, ';-h:');

      if (tokens.size() > 2) continue;

      Animal animal = parse_animal(tokens.at(0));

      if (tokens.size() > 1) {
        std::vector<std::string> historyTokens = tokenize(tokens.at(1), '#');

        auto itr = historyTokens.begin();
        while (itr != historyTokens.end()) {
          HistoryItem history = parse_history(*itr);
          animal.insert_history(std::make_shared<HistoryItem>(history));
          ++itr;
        }
      }

      auto animal_ptr = std::make_shared<Animal>(animal);
      m_animals.root = m_animals.insert(m_animals.root, animal_ptr, animal.m_id);
    }
  }
  else {
  }

  oss << ">>> Arquivo lido com sucesso. " << m_animals.size() << " registros encontrados.";
  std::cout << oss.str() << "\n\n";
}

template <typename Tree>
void Program<Tree>::initialize(int argc, char* argv[]) {
  m_state = e_state::STARTING;
  m_error_msg = "";

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

template <typename Tree>
bool Program<Tree>::has_finished() {
  return m_state == e_state::QUITTING;
}

#pragma endregion

#pragma region USER_INTERACTIONS

template <typename Tree>
Animal Program<Tree>::parse_animal(const std::string& animal_str) {
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

template <typename Tree>
HistoryItem Program<Tree>::parse_history(const std::string& history_str) {
  HistoryItem history;

  auto tokens = tokenize(history_str, ';');

  if (tokens.size() < 6) {
    throw std::invalid_argument("Invalid history string!");
  }

  history.m_temperature = std::stoi(tokens.at(1));
  history.m_height = std::stof(tokens.at(2));
  history.m_weight = std::stof(tokens.at(3));
  history.m_blood_collected = tokens.at(4) == "y" || tokens.at(4) == "Y";
  history.m_is_ok = tokens.at(5) == "y" || tokens.at(5) == "Y";

  try {
    history.m_note = tokens.at(6);
  }
  catch (const std::out_of_range& err) { /* Não foi passada Nota */ }

  auto date = tokenize(tokens.at(0), '/');
  common::Date date_obj(std::stoi(date.at(0)), std::stoi(date.at(1)), std::stoi(date.at(2)));

  history.m_date = date_obj;

  return history;
}

template <typename Tree>
void Program<Tree>::read_animal() {
  Animal animal;

  std::string line;
  std::getline(std::cin, line);

  animal = parse_animal(line);
  auto animal_ptr = std::make_shared<Animal>(animal);
  m_animals.root = m_animals.insert(m_animals.root, animal_ptr, animal.m_id);
  m_msg = "Animal adicionado!";
}

template <typename Tree>
void Program<Tree>::read_animal_history() {
  std::string line;
  std::cout << BLUE << "Digite o ID do animal:\n"
    << RESET << ">>> ";
  std::getline(std::cin, line);

  int animal_id = -1;
  try
  {
    animal_id = std::stoi(line);
  }
  catch (const std::exception& e)
  {
    m_error_msg = "Código de animal inválido!";
    return;
  }

  print_include_animal_history();

  std::getline(std::cin, line);
  HistoryItem history = parse_history(line);

  auto duration = measure_execution_time([&]() {
    m_animals.search(m_animals.root, animal_id)->value->insert_history(std::make_shared<HistoryItem>(history));
    });

  std::cout << CYAN << "\nTempo de busca: " << duration << " ms\n";
  std::cout << GREEN << "\n>>> Pressione Enter para voltar ao menu...\n";

  m_msg = "Histórico adicionado ao animal " + std::to_string(animal_id) + ".";
}

template <typename Tree>
void Program<Tree>::search_animal() {
  std::string line;
  std::getline(std::cin, line);

  int key = std::stoi(line);
  std::shared_ptr<Animal> animal = nullptr;
  auto duration = measure_execution_time([&]() {
    animal = m_animals.search(m_animals.root, key)->value;
    });

  if (animal == nullptr) {
    m_error_msg = "Animal não encontrado";
    return;
  }

  std::cout << BLUE << BOLD << "\nAnimal:\n\n" << RESET
    << "------------------------------------------------------------------------------------\n";

  animal->print();

  std::cout
    << "------------------------------------------------------------------------------------\n\n";
  animal->print_history();

  std::cout << CYAN << "Tempo de busca: " << duration << " ms\n";
  std::cout << GREEN << "\n>>> Pressione Enter para voltar ao menu...\n";
}

template <typename Tree>
void Program<Tree>::remove_animal() {
  int animal_id;
  std::string line;
  std::getline(std::cin, line);

  try
  {
    animal_id = std::stoi(line);
  }
  catch (const std::exception& e)
  {
    m_error_msg = "Código de Animal inválido!";
  }

  auto node = m_animals.deleteNode(m_animals.root, animal_id);

  if (node == nullptr) {
    m_error_msg = "Animal não encontrado";
    return;
  }

  m_animals.root = node;
  m_msg = "Animal " + std::to_string(animal_id) + " removido!";
}

template <typename Tree>
void Program<Tree>::save_file() {
  std::ofstream ofs(m_file_path);

  if (ofs.is_open()) {
    ofs << m_animals.inorder(m_animals.root, true);
    ofs.close();

    m_msg = "Arquivo salvo!";
  }
  else {
    m_error_msg = "Não foi possível abrir o caminho especificado!";
  }
}

#pragma endregion

template <typename Tree>
void Program<Tree>::process_events() {
  m_error_msg = "";
  m_msg = "";
  if (m_state == e_state::READING_MENU_OPT) {
    std::string line;
    std::getline(std::cin, line);
    m_error_msg = "";

    try
    {
      auto option_selected = std::stoi(line);
      if (option_selected > e_state::QUITTING || option_selected < 1)
        throw std::runtime_error("error");

      m_selected_option = (e_menu_option)option_selected;

      std::cout << "\n";
    }
    catch (const std::exception& e)
    {
      m_error_msg = "Opção inválida!";
      m_selected_option = e_menu_option::INVALID;
    }
  }
  else if (m_state == e_state::READING_FILE) {
    read_file();
  }
  else if (m_state == e_state::INCLUDING_ANIMAL) {
    read_animal();
  }
  else if (m_state == e_state::INCLUDING_ANIMAL_HISTORY) {
    read_animal_history();
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
  else if (m_state == e_state::READING_ANIMAL) {
    search_animal();

    std::string line;
    std::getline(std::cin, line);
  }
  else {
    std::string line;
    std::getline(std::cin, line);
  }
}

template <typename Tree>
void Program<Tree>::update() {
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
    else if (m_selected_option == e_menu_option::READ_ANIMAL) {
      m_state = e_state::READING_ANIMAL;
    }
    else if (m_selected_option == e_menu_option::INCLUDE_ANIMAL_HISTORY) {
      m_state = e_state::INCLUDING_ANIMAL_HISTORY;
    }
    else if (m_selected_option == e_menu_option::INVALID) {}
  }
  else if (
    m_state == e_state::HELPING
    || m_state == e_state::INCLUDING_ANIMAL
    || m_state == e_state::INCLUDING_ANIMAL_HISTORY
    || m_state == e_state::CONSULTING_ANIMALS
    || m_state == e_state::READING_ANIMAL
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

template <typename Tree>

void Program<Tree>::render() const {
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
  else if (m_state == e_state::INCLUDING_ANIMAL_HISTORY) {
    // print_include_animal();
  }
  else if (m_state == e_state::CONSULTING_ANIMALS) {
    print_animals();
  }
  else if (m_state == e_state::READING_ANIMAL) {
    print_reading_animal();
  }
  else if (m_state == e_state::REMOVING_ANIMAL) {
    std::cout << BLUE << "Digite o código do animal:\n"
      << RESET << ">>> ";
  }
  else if (m_state == e_state::CONSULTING_ANIMAL_HISTORY) {}
  else if (m_state == e_state::SAVING_FILE) {
    print_writing_file();
  }
  else if (m_state == e_state::READING_FILE) {

  }
}

#pragma region RENDER_METHODS

template <typename Tree>
void Program<Tree>::print_welcome() const {
  std::cout
    << BLUE << "Mensagem de boas vindas\n\n"
    << GREEN << ">>> Pressione Enter para iniciar...";
}

template <typename Tree>
void Program<Tree>::print_reading_file() const {
  std::cout << YELLOW << ">>> Lendo arquivo (" << BOLD << m_file_path << RESET << YELLOW << ")...\n\n";
}

template <typename Tree>
void Program<Tree>::print_writing_file() const {
  std::cout << YELLOW
    << "Digite o caminho do arquivo:\n"
    << ">>> ";
}

template <typename Tree>
void Program<Tree>::print_menu() const {
  std::cout << "\n\n"
    << BLUE << "----------------------- Gestão de fauna do parque ------------------------\n"
    << "| "
    << padStart(std::to_string(m_animals.size()), 4, '0')
    << " Animais cadastrados                                               |\n"
    << "| Selecione uma opção:                                                   |\n"
    << "|                                                                        |\n"
    << "| 1: Incluir animal                                                      |\n"
    << "| 2: Consultar animais                                                   |\n"
    << "| 3: Ler animal                                                          |\n"
    << "| 4: Incluir histórico de monitoramento de animal                        |\n"
    << "| 5: Remover animal                                                      |\n"
    << "| 6: Salvar arquivo                                                      |\n"
    << "| 7: Ajuda                                                               |\n"
    << "| 8: Sair                                                                |\n"
    << "--------------------------------------------------------------------------\n\n"
    << RED << "ERROR: [" << m_error_msg << "]\n" << RESET
    << YELLOW << "MSG: [" << m_msg << "]\n\n" << RESET
    << ">>> ";
}

template <typename Tree>
void Program<Tree>::print_include_animal() const {
  std::cout
    << BLUE << BOLD << "Inclusão de animal\n" << RESET
    << YELLOW << "Insira os dados no formato:\n"
    << "<ID (numérico)>;<Nome>;<Espécie>;<Gênero>;<Data de Monitoramento (dd/MM/yyyy)>;<Data de Nascimento (dd/MM/yyyy)>\n\n"
    << RESET << ">>> ";
}

template <typename Tree>
void Program<Tree>::print_include_animal_history() const {
  std::cout
    << BLUE << BOLD << "Inclusão de histórico de monitoramento de animal\n" << RESET
    << YELLOW << "Insira os dados no formato:\n"
    << "<Data de Monitoramento (dd/MM/yyyy)>;<Peso>;<Altura>;<Coletou sangue (y/n)>;<Animal OK (y/n)>;<Nota (opcional)>\n\n"
    << RESET << ">>> ";
}

template <typename Tree>
void Program<Tree>::print_animals() const {
  std::cout << BLUE << BOLD << "Animais:\n\n" << RESET;

  std::cout << BLUE << BOLD
    << "------------------------------------------------------------------------------------\n"
    << "| ID   | Nome             | Espécie          | Gênero    | Monitorado | Nascimento |\n"
    << "------------------------------------------------------------------------------------\n"
    << RESET;

  m_animals.inorder(m_animals.root);

  std::cout << BLUE << BOLD
    << "------------------------------------------------------------------------------------\n"
    << " " << m_animals.size() << " animais.\n\n";
  std::cout << GREEN << "\n>>> Pressione Enter para voltar ao menu...\n";
}

template<typename Tree>
void Program<Tree>::print_reading_animal() const {
  std::cout
    << BLUE << BOLD << "Consulta de animal. Digite o ID do animal: \n"
    << RESET << ">>> ";
}

template <typename Tree>
void Program<Tree>::print_help() const {
  std::cout
    << BLUE << "<HELP>\n"
    << GREEN << ">>> Pressione Enter para voltar ao menu...\n";
}

template <typename Tree>
void Program<Tree>::print_exit() const {
  std::cout
    << "<QUITTING>\n\n";
}

#pragma endregion