#ifndef COMMON_H
#define COMMON_H

#include <iostream>

namespace fauna {

  struct Date {
    unsigned short day;
    unsigned short month;
    unsigned short year;

    friend std::ostream& operator<<(std::ostream& os, const Date& date) {
      os << date.day << "/" << date.month << "/" << date.year;
      return os;
    }
  };
};

#endif // COMMON_H