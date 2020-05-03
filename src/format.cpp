#include <string>

#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  long hour = seconds / 3600;
  seconds = seconds % 3600;
  long min = seconds / 60;
  seconds = seconds % 60;
  return std::to_string(hour) + ":" + std::to_string(min) + ":" +
         std::to_string(seconds);
}