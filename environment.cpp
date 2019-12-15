#include <iostream>
#include <algorithm>

#include "environment.hpp"

#include "util.hpp"

using namespace std;

bool debug = false;
bool debug_env = true;

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
  size_(min(testground_size, (uint16_t)65535)),
  dots_(),
  food_()
{
  srand(time(NULL));
  tick_ = 0;
  contamination(dot_count);
  feeding(min_food_count, max_food_count);
}

void Environment::tick(){
  // Generate new Food
  feeding(200);
  // Trigger Tick of every Dot and let Dots with enough energy replicate
  for (size_t i = 0; i < dots_.size(); ++i) {
    searchFood(dots_[i]);
    dots_[i].tick();
    if(dots_[i].getReproductionCooldown() <= 0 && dots_[i].getEnergy() >= 5000){
      dots_.push_back(dots_[i].replicate());
    }
  }
  // Delete Dot with 0 energy or less
  dots_.erase(remove_if(dots_.begin(), dots_.end(), [](Dot &d){
    return d.getEnergy() <= 0;
  }), dots_.end());
  ++tick_;

  if(debug || debug_env){
    cout << "Tick: " << tick_ << endl;
    cout << "Lebende Dots: " << dots_.size() << endl;
    cout << "Menge Futter auf dem Testfeld: " << food_.size() << endl;
  }
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
    food_.push_back(make_pair(x,y));
  }
}

void Environment::feeding(const int max_amount){
  feeding(0, max_amount);
}

void Environment::searchFood(Dot &d){
  // Needs Optimization badly
  int min_index = -1;
  uint16_t min_distance = -1;
  for (size_t i = 0; i < food_.size(); ++i) {
    if(distance(d.getPosition(), food_[i]) < min_distance){
      min_index = i;
      min_distance = distance(d.getPosition(), food_[i]);
    }
  }
  if(min_index != -1 && min_distance == 0){
    d.eat(1000);
    food_.erase(food_.begin() + min_index);
    searchFood(d);
  } else {
    d.newFoodSource(food_[min_index]);
  }
}

void Environment::printTestground(){
  cout << "Der (skalierte) Testbereich in Tick " << tick_ << ":" << endl;
  vector<vector<int>> map_d(100, vector<int>(100));
  for (size_t i = 0; i < dots_.size(); ++i) {
    ++map_d[(dots_[i].getPosition().first) * 100 / size_][(dots_[i].getPosition().second) * 100 / size_];
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
  Environment e(1000, 1, 0, 200);
  e.tick(10000);
  e.printTestground();
  return 0;
}
