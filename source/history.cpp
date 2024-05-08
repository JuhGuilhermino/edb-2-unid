#include "history.h"
#include <iostream>
#include <iomanip>
#include <sstream>

void HistoryItem::print() const {
  auto collect = (m_blood_collected ? "Coletado" : "-");
  auto ok = (m_is_ok ? "OK" : "X");

  auto pad_start = [&](const std::string& input, size_t targetLength, char padChar = ' ') {
    if (input.length() >= targetLength) {
      return input;
    }
    return std::string(targetLength - input.length(), padChar) + input;
    };

  std::cout << "| "
    << pad_start(std::to_string(m_id), 4, '0') << " | "
    << m_date << " | "
    << std::setw(11) << std::left << m_temperature << " | "
    << std::setw(11) << std::left << m_weight << " | "
    << std::setw(9) << std::left << m_height << " | "
    << std::setw(15) << std::left << collect << " | "
    << ok << " |\n"
    << std::setw(81) << std::left << "| Nota: " << " |\n"
    << std::setw(81) << std::left << "| " << m_note << " |\n";
}

std::string HistoryItem::write() const {
  std::ostringstream oss;
  oss
    << m_date << ";"
    << std::to_string(m_temperature) << ";"
    << std::to_string(m_height) << ";"
    << std::to_string(m_weight) << ";"
    << (m_blood_collected ? 'y' : 'n') << ";"
    << (m_is_ok ? 'y' : 'n') << ";"
    << m_note;

  return oss.str();
}