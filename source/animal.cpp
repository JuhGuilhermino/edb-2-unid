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
    << m_born_at << "\n";

  return oss.str();
}