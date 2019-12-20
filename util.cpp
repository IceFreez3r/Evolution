#include <math.h>
#include <iostream>

#include "util.hpp"

using namespace std;

extern bool debug;
bool debug_util = false;

uint16_t distance(const pair<uint16_t, uint16_t> pos, const pair<uint16_t, uint16_t> pos2, const uint16_t testground_size){
  uint16_t x_diff = min(pos2.first - pos.first, testground_size - abs(pos2.first - pos.first));
  uint16_t y_diff = min(pos2.second - pos.second, testground_size - abs(pos2.second - pos.second));
  if(debug || debug_util){
    cout << "Distanz von (" << pos.first << "," << pos.second << ") und (" << pos2.first << "," << pos2.second << "): " << sqrt(pow(x_diff, 2) + pow(y_diff, 2)) << endl;
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
    cout << x1 << "   " << x2 << "   " << xf << endl;
    cout << y1 << "   " << y2 << "   " << yf << endl;
    cout << "Richtung von (" << start_pos.first << "," << start_pos.second << ") nach (" << target_pos.first << "," << target_pos.second << "): " << atan2(yf, xf) / (2 * M_PI) * 360 << endl;
  }
  return atan2(yf, xf) / (2 * M_PI) * 360;
}

pair<uint16_t, uint16_t> move(const pair<uint16_t, uint16_t> pos, uint16_t direction, const uint16_t step_size, const uint16_t testground_size){
  uint16_t x_new = (uint16_t)(cos(((float)direction)/360 * 2 * M_PI) * step_size + pos.first) % testground_size;
  uint16_t y_new = (uint16_t)(sin(((float)direction)/360 * 2 * M_PI) * step_size + pos.second) % testground_size;
  if(debug || debug_util){
    cout << "Neue Position von (" << pos.first << "," << pos.second << ") in Richtung " << direction << " und Weite " << step_size << ": (" << x_new << "," << y_new << ")\n";
  }
  return make_pair(x_new, y_new);
}

// Credits to user79758 on https://stackoverflow.com/questions/1903954/is-there-a-standard-sign-function-signum-sgn-in-c-c
float signum(float val) {
    return (0 < val) - (val < 0);
}
// template <typename T> int signum(T val) {
//     return (T(0) < val) - (val < T(0));
// }
