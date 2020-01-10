#include <math.h>
#include <iostream>

#include "util.hpp"

using namespace std;

extern bool debug;
bool debug_util = false;

uint16_t distance(const pair<uint16_t, uint16_t> pos, const pair<uint16_t, uint16_t> pos2, const uint16_t testground_size){
  int x_diff = min(pos2.first - pos.first, testground_size - abs(pos2.first - pos.first));
  int y_diff = min(pos2.second - pos.second, testground_size - abs(pos2.second - pos.second));
  if(debug || debug_util){
    cout << "X-Diff: " << x_diff << ", Y-Diff: " << y_diff;
    cout << "\nDistanz von (" << pos.first << "," << pos.second << ") und (" << pos2.first << "," << pos2.second << "): " << sqrt(pow(x_diff, 2) + pow(y_diff, 2)) << "\n";
  }
  return sqrt(pow(x_diff, 2) + pow(y_diff, 2));
}

uint16_t direction(const pair<uint16_t, uint16_t> start_pos, const pair<uint16_t, uint16_t> target_pos, const uint16_t testground_size){
  float x1 = target_pos.first - start_pos.first;
  float x2 = x1 - testground_size * signum(x1);
  float y1 = target_pos.second - start_pos.second;
  float y2 = y1 - testground_size * signum(y1);
  float xf;
  if(abs(x1) <= abs(x2)){
    xf = x1;
  }else {
    xf = x2;
  }
  float yf;
  if(abs(y1) <= abs(y2)){
    yf = y1;
  }else {
    yf = y2;
  }
  if(debug || debug_util){
    cout << "x1|x2|xfinal" << x1 << "|" << x2 << "|" << xf;
    cout << "\ny1|y2|yfinal" << y1 << "|" << y2 << "|" << yf;
    cout << "\nRichtung von (" << start_pos.first << "," << start_pos.second << ") nach (" << target_pos.first << "," << target_pos.second << "): " << atan2(yf, xf) / (2 * M_PI) * 360 << "\n";
  }
  return atan2(yf, xf) / (2 * M_PI) * 360;
}

pair<uint16_t, uint16_t> move(const pair<uint16_t, uint16_t> pos, int16_t direction, const uint16_t step_size, const uint16_t testground_size){
  uint16_t x_new = ((int)(cos(((float)direction)/360 * 2 * M_PI) * step_size) + pos.first + testground_size) % testground_size;
  uint16_t y_new = ((int)(sin(((float)direction)/360 * 2 * M_PI) * step_size) + pos.second + testground_size) % testground_size;
  if(debug || debug_util){
    cout << "Neue Position von (" << pos.first << "," << pos.second << ") in Richtung " << direction << " und Weite " << step_size << ": (" << x_new << "," << y_new << ")\n";
  }
  return make_pair(x_new, y_new);
}

// Credits to user79758 on https://stackoverflow.com/questions/1903954/is-there-a-standard-sign-function-signum-sgn-in-c-c
int signum(float val) {
    return (0 < val) - (val < 0);
}
// template <typename T> int signum(T val) {
//     return (T(0) < val) - (val < T(0));
// }

std::string niceNumberPrint(const uint16_t number, uint8_t length){
  uint8_t front_spaces = 0, back_spaces = 0;
  std::string number_string = to_string(number);
  while(front_spaces + back_spaces + number_string.length() < length){
    if(front_spaces == back_spaces){
      ++front_spaces;
    } else {
      ++back_spaces;
    }
  }
  std::string out_string = "";
  for (uint8_t i = 0; i < front_spaces; ++i) {
    out_string += " ";
  }
  out_string += number_string;
  for (uint8_t i = 0; i < back_spaces; ++i) {
    out_string += " ";
  }
  return out_string;
}
//   2    3      4       5    ...
// | 1| | 1 | |  1 |  |  1  | ...
// |11| | 11| | 11 |  |  11 | ...
//      |111| | 111|  | 111 | ...
//            |1111|  | 1111| ...
//                    |11111| ...
