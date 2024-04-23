#include "history.h"
#include <iostream>
#include <sstream>

void HistoryItem::print() const {
  auto collect = (m_blood_collected ? "Coletado" : "-");
  auto ok = (m_is_ok ? "OK" : "X");
  std::cout
    << "-------------------------------------------------------"
    << "| " << m_date << " | " << m_weight << " | " << m_height << " | " << m_temperature << " | " << collect << " | " << ok << " |\n"
    << "| Nota:                                               |\n"
    << "| " << m_note << " |"
    << "-------------------------------------------------------";
}