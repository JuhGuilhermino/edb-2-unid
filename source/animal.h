#ifndef ANIMAL_H
#define ANIMAL_H

#include <vector>
#include <string>
#include <memory>
#include "history.h"

using namespace fauna;

class Animal
{
private:
  using history_item = std::shared_ptr<HistoryItem>;
  using history = std::vector<history_item>;

  unsigned int m_id;
  std::string m_name;
  std::string m_species;
  std::string m_genre;
  fauna::Date m_monitored_at;
  fauna::Date m_born_at;

  history m_history;

public:
  void insert_history(history_item& history);
  void print_history() const;

};

#endif