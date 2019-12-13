#include <iostream>
#include <algorithm>

#include "environment.hpp"

using namespace std;

Environment::Environment():
  size_(65535),
  dots_(),
  food_()
{
  srand(time(NULL));
  tick_ = 0;
  contamination(1000);
  feeding(200);
}

Environment::Environment(const uint16_t testground_size, const int dot_count, const int min_food_count, const int max_food_count):
  size_(testground_size),
  dots_(),
  food_()
{
  srand(time(NULL));
  tick_ = 0;
  contamination(dot_count);
  feeding(min_food_count, max_food_count);
}

void Environment::tick(){
  // sort(food_.begin(),food_.end());
  for (size_t i = 0; i < dots_.size(); ++i) {
    dots_[i].tick();
  }
  feeding(200);
  ++tick_;
}

void Environment::tick(const int amount){
  for (int i = 0; i < amount; ++i) {
    tick();
  }
}

void Environment::contamination(const int amount){
  for (int i = 0; i < amount; ++i) {
    dots_.push_back(Dot(size_));
  }
}

void Environment::feeding(const int min_amount, const int max_amount){
  int food_count = rand() % (max_amount - min_amount) + min_amount + 1;
  for (size_t i = 0; i < food_count; ++i) {
    uint16_t x = rand() % size_;
    uint16_t y = rand() % size_;
    // cout << x << "  " << y << '\n';
    food_.push_back(make_pair(x,y));
  }
}

void Environment::feeding(const int max_amount){
  feeding(0, max_amount);
}

void Environment::printTestground(){
  cout << "Der (skalierte) Testbereich in Tick " << tick_ << ":" << endl;
  vector<vector<int>> map_d(100, vector<int>(100));
  for (size_t i = 0; i < dots_.size(); ++i) {
    ++map_d[(dots_[i].getPos().first) * 100 / size_][(dots_[i].getPos().second) * 100 / size_];
  }
  vector<vector<int>> map_f(100, vector<int>(100));
  for (size_t i = 0; i < food_.size(); ++i) {
    ++map_f[(food_[i].first) * 100 / size_][(food_[i].second) * 100 / size_];
  }
  for (size_t i = 0; i < 100; ++i) {
    for (size_t j = 0; j < 100; ++j) {
      if (map_d[i][j] != 0) {
        if (map_f[i][j] != 0) {
          cout << "%";
        } else {
          cout << ".";
        }
      } else if (map_f[i][j] != 0) {
        cout << "x";
      } else {
        cout << " ";
      }
    }
    cout << endl;
  }
  cout << "Legende: ' ' Nichts, '.' Dot, 'x' Essen, '%' Dot und Essen\nBei mehreren Objekten auf demselben Punkt wird nur eins angezeigt\nDie Ausgabe ist skaliert auf 100x100" << endl;
}

int main(int argc, char const *argv[]) {
  Environment e(65535, 1, 0, 200);
  e.tick(10);
  e.printTestground();
  return 0;
}
