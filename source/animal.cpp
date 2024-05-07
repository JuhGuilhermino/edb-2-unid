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