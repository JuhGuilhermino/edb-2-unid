#include "animal.h"
#include <iomanip>

void Animal::print() const {
  auto pad_end = [&](const std::string& input, size_t targetLength, char padChar = ' ') {
    if (input.length() >= targetLength) {
      return input;
    }
    return input + std::string(targetLength - input.length(), padChar);
    };
  auto pad_start = [&](const std::string& input, size_t targetLength, char padChar = ' ') {
    if (input.length() >= targetLength) {
      return input;
    }
    return std::string(targetLength - input.length(), padChar) + input;
    };

  std::cout << "| "
    << pad_start(std::to_string(m_id), 4, '0') << " | "
    << std::setw(16) << std::left << m_name << " | "
    << std::setw(16) << std::left << m_species << " | "
    << std::setw(9) << std::left << m_genre << " | "
    << std::setw(10) << std::left << common::Date::to_string(m_monitored_at) << " | "
    << std::setw(10) << std::left << common::Date::to_string(m_born_at) << " |\n";
}

std::string Animal::write() const {
  std::ostringstream oss;

  oss
    << std::to_string(m_id) << ";"
    << m_name << ";"
    << m_species << ";"
    << m_genre << ";"
    << m_monitored_at << ";"
    << m_born_at;

  if (m_history.size() > 0) {
    oss << ";-h:";

    auto itr = m_history.begin();
    while (itr != m_history.end()) {
      history_item history = *itr;
      oss << history->write();
      ++itr;

      if (itr != m_history.end()) oss << "#";
    }
  }

  oss << "\n";

  return oss.str();
}

#define RESET   "\033[0m"
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLD    "\033[1m"       /* Bold */

void Animal::insert_history(history_item history) {
  history->m_id = m_history.size() + 1;
  m_history.push_back(history);
}

void Animal::print_history() const {
  std::cout << BLUE << BOLD << "Histórico de monitoramento:\n\n" << RESET;

  std::cout << BLUE << BOLD
    << "-----------------------------------------------------------------------------------\n"
    << "| ID   | Data      | Temperatura | Altura (cm) | Peso (Kg) | Sangue coletado | OK |\n"
    << "-----------------------------------------------------------------------------------\n"
    << RESET;

  auto itr = m_history.begin();

  while (itr != m_history.end()) {
    history_item history = *itr;

    history->print();

    ++itr;
  }

  std::cout << BLUE << BOLD
    << "-----------------------------------------------------------------------------------\n"
    << " " << m_history.size() << " Registros.\n\n";
}