#include "animal.h"

void Animal::print() const {
  std::cout
    << "| " << m_id
    << " | " << m_name
    << " | " << m_species
    << " | " << m_genre
    << " | " << m_monitored_at
    << " | " << m_born_at
    << " |\n";
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