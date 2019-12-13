#include <math.h>
#include <iostream>

#include "util.hpp"

using namespace std;

bool debug = false;

uint16_t distance(const pair<uint16_t, uint16_t> pos, const pair<uint16_t, uint16_t> pos2){
  if(debug){
    cout << "Distanz von (" << pos.first << "," << pos.second << ") und (" << pos2.first << "," << pos2.second << "): " << sqrt(pow(pos.first - pos2.first, 2) + pow(pos.second - pos2.second, 2)) << endl;
  }
  return sqrt(pow(pos.first - pos2.first, 2) + pow(pos.second - pos2.second, 2));
}

uint16_t direction(pair<uint16_t, uint16_t> start_pos, pair<uint16_t, uint16_t> target_pos){
  if(debug){
    cout << "Richtung von (" << start_pos.first << "," << start_pos.second << ") nach (" << target_pos.first << "," << target_pos.second << "): " << atan2((float)(target_pos.second - start_pos.second), (float)(target_pos.first - start_pos.first)) / (2 * M_PI) * 360 << endl;
  }
  return atan2((float)(target_pos.second - start_pos.second), (float)(target_pos.first - start_pos.first)) / (2 * M_PI) * 360;
}

pair<uint16_t, uint16_t> move(const pair<uint16_t, uint16_t> pos, uint16_t direction, const uint16_t step_size, const uint16_t testground_size){
  uint16_t x_new = (uint16_t)(cos(((float)direction)/360 * 2 * M_PI) * step_size + pos.first) % testground_size;
  uint16_t y_new = (uint16_t)(sin(((float)direction)/360 * 2 * M_PI) * step_size + pos.second) % testground_size;
  if(debug){
    cout << "Neue Position von (" << pos.first << "," << pos.second << ") in Richtung " << direction << " und Weite " << step_size << ": (" << x_new << "," << y_new << ")\n";
  }
  return make_pair(x_new, y_new);
}
