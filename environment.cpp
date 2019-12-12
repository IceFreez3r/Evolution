#include <iostream>
#include <algorithm>

#include "environment.hpp"

using namespace std;

Environment::Environment():
  size_(65535)
{
 srand(time(NULL));
 food_ = vector<pair<unsigned int, unsigned int>>(0);
 dots_ = vector<Dot>(0);
}

void Environment::tick(){
  int food_count = rand() % 200 + 1;
  for (size_t i = 0; i < food_count; ++i) {
    unsigned int x = rand() % size_;
    unsigned int y = rand() % size_;
    // cout << x << "  " << y << '\n';
    food_.push_back(make_pair(x,y));
  }
  // sort(food_.begin(),food_.end());
  for (size_t i = 0; i < dots_.size(); ++i) {
    dots_[i].tick();
  }
}

void Environment::printFood(){
  vector<vector<int>> map(100, vector<int>(100));
  for (size_t i = 0; i < food_.size(); ++i) {
    ++map[(food_[i].first) * 100 / size_][(food_[i].second) * 100 / size_];
  }
  for (size_t i = 0; i < 100; ++i) {
    for (size_t j = 0; j < 100; ++j) {
      if (map[i][j] == 0) {
        std::cout << " ";
      } else {
        cout << "x";
        // cout << map[i][j];
      }
    }
    cout << endl;
  }
}

int main(int argc, char const *argv[]) {
  Environment e;
  for (int i = 0; i < 100; ++i) {
    e.tick();
  }
  e.printFood();
  return 0;
}
