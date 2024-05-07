#ifndef ANIMAL_H
#define ANIMAL_H

#include <vector>
#include <string>
#include <memory>
#include "history.h"
#include "common.h"

class Animal
{
private:
  using history_item = std::shared_ptr<HistoryItem>;
  using history = std::vector<history_item>;

  history m_history;

public:
  Animal() {

  };

  unsigned int m_id;
  std::string m_name;
  std::string m_species;
  std::string m_genre;
  common::Date m_monitored_at;
  common::Date m_born_at;

  void insert_history(history_item& history);
  void print_history() const;
  void print() const;
  std::string write() const;

};

#endif