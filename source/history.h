#ifndef HISTORY_H
#define HISTORY_H

#include <string>
#include <common.h>

using namespace fauna;

class HistoryItem {
private:

  fauna::Date m_date; //!< Data de inspeção
  float m_temperature; //!< Temperatura em ºC
  float m_height; //!< Altura em cm
  float m_weight; //!< Peso em Kg
  bool m_blood_collected; //!< Se coletou sangue
  bool m_is_ok; //!< Se tá tudo certo com o animal
  std::string m_note; //!< Se não estiver bem, o motivo

public:
  void print() const;
};

#endif