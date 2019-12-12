#include <math.h>

#include "util.hpp"

using namespace std;

uint16_t distance(const pair<uint16_t, uint16_t> pos, const pair<uint16_t, uint16_t> pos2){
  return sqrt(pow(pos.first - pos2.first, 2) + pow(pos.second - pos2.second, 2));
}

uint16_t direction(const pair<uint16_t, uint16_t> start_pos, pair<uint16_t, uint16_t> target_pos){
  return atan2((float)(target_pos.first - start_pos.first) / (float)(target_pos.second - start_pos.second)) / M_PI * 360;
}

pair<uint16_t, uint16_t> move(const pair<uint16_t, uint16_t> pos, const uint16_t direction, const uint16_t step_size){
  uint16_t x_new = cos(((float)direction)/360 * M_PI) * step_size + pos.first;
  uint16_t y_new = sin(((float)direction)/360 * M_PI) * step_size + pos.second;
  return make_pair(x_new, y_new);
}
