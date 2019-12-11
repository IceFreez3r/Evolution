#include <math.h>
#include <utility>

using namespace std;

unsigned int distance(const pair<unsigned int, unsigned int> pos, const pair<unsigned int, unsigned int> pos2){
  return sqrt(pow(pos.first - pos2.first, 2) + pow(pos.second - pos2.second, 2));
}

pair<unsigned int, unsigned int> move(const pair<unsigned int, unsigned int> pos, const unsigned int direction, const unsigned int step_size){
  unsigned int x_new = cos(((float)direction)/360) * step_size + pos.first;
  unsigned int y_new = sin(((float)direction)/360) * step_size + pos.second;
  return make_pair(x_new, y_new);
}
