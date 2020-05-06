#include <string>

#include "format.h"

using std::string;

// INPUT: Long int
// OUTPUT: 2 (including zero) or more digit string
string formatted_string(long l) {
  if (l < 10) {
    return std::to_string(0) + std::to_string(l);
  } else {
    return std::to_string(l);
  }
}

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  const long hour = seconds / 3600;
  seconds = seconds % 3600;
  const long min = seconds / 60;
  seconds = seconds % 60;
  const string hour_string = formatted_string(hour);
  const string min_string = formatted_string(min);
  const string seconds_string = formatted_string(seconds);

  return hour_string + ":" + min_string + ":" + seconds_string;
}
