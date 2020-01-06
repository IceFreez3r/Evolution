#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "environment.hpp"

#include "util.hpp"

using namespace std;

bool debug = false;
bool debug_env = false;

Environment::Environment():
  testground_size_(65535),
  dots_(),
  food_(),
  min_food_per_tick_(0),
  max_food_per_tick_(200)
{
  srand(time(NULL));
  tick_ = 0;
  start_dot_ = Dot(testground_size_);
  contamination(1000);
}

Environment::Environment(const uint16_t testground_size, const int dot_count, const int min_food_count, const int max_food_count):
  testground_size_(min(testground_size, (uint16_t)65535)),
  dots_(),
  food_(),
  min_food_per_tick_(min_food_count),
  max_food_per_tick_(max_food_count)
{
  srand(time(NULL));
  tick_ = 0;
  start_dot_ = Dot(testground_size_);
  contamination(dot_count);
}

Environment::Environment(const uint16_t testground_size, const int dot_count, const int min_food_count, const int max_food_count, const Dot start_dot):
  testground_size_(min(testground_size, (uint16_t)65535)),
  dots_(),
  food_(),
  min_food_per_tick_(min_food_count),
  max_food_per_tick_(max_food_count)
{
  srand(time(NULL));
  tick_ = 0;
  start_dot_ = start_dot;
  if(testground_size_ != start_dot_.getTestgroundSize()){
    start_dot_.setTestgroundSize(testground_size_);
  }
  contamination(dot_count);
}

void Environment::tick(const int amount /* = 1 */){
  for (int i = 0; i < amount; ++i) {
    // Generate new Food
    feeding(min_food_per_tick_, max_food_per_tick_);
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
}

void Environment::contamination(const int amount){
  for (int i = 0; i < amount; ++i) {
    dots_.push_back(Dot(start_dot_));
  }
}

void Environment::feeding(const int min_amount, const int max_amount){
  if (min_amount >= max_amount) {
    feeding(min_amount);
    return;
  }
  int food_count = rand() % (max_amount - min_amount) + min_amount;
  for (size_t i = 0; i < food_count; ++i) {
    uint16_t x = rand() % testground_size_;
    uint16_t y = rand() % testground_size_;
    food_.push_back(make_pair(x,y));
  }
}

void Environment::feeding(const int food_count){
  for (size_t i = 0; i < food_count; ++i) {
    uint16_t x = rand() % testground_size_;
    uint16_t y = rand() % testground_size_;
    food_.push_back(make_pair(x,y));
  }
}

void Environment::searchFood(Dot &d){
  sort(food_.begin(), food_.end());
  // Needs Optimization badly
  int min_index = -1;
  uint16_t min_distance = ~0;
  for (size_t i = 0; i < food_.size(); ++i) {
    uint16_t dist = distance(d.getPosition(), food_[i], testground_size_);
    if(dist < min_distance){
      min_index = i;
      min_distance = dist;
    }
  }
  if(min_index != -1 && min_distance == 0){
    d.eat(1000);
    food_.erase(food_.begin() + min_index);
    searchFood(d);
  } else if(min_distance < d.getSight()){
    d.newFoodSource(food_[min_index]);
  }
}

void Environment::printMap(){
  uint16_t scale = min((uint16_t)100, testground_size_);
  cout << "Der (skalierte) Testbereich in Tick " << tick_ << ":" << endl;
  vector<vector<int>> map_d(scale, vector<int>(scale));
  for (size_t i = 0; i < dots_.size(); ++i) {
    ++map_d[(dots_[i].getPosition().first) * scale / testground_size_][(dots_[i].getPosition().second) * scale / testground_size_];
  }
  vector<vector<int>> map_f(scale, vector<int>(scale));
  for (size_t i = 0; i < food_.size(); ++i) {
    ++map_f[(food_[i].first) * scale / testground_size_][(food_[i].second) * scale / testground_size_];
  }
  for (size_t i = 0; i < scale; ++i) {
    for (size_t j = 0; j < scale; ++j) {
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

void Environment::printTestground(){
  cout << "Testgroundsize: " << testground_size_ << "\nMin-|Maxfutter pro Tick: " << min_food_per_tick_ << "|"<< max_food_per_tick_ << "\nTick: " << tick_ << "\n#Dots: " << dots_.size() << "\n#Futter: " << food_.size();
}

void Environment::printProperties(){
  if(dots_.size() != 0){
    uint16_t min_speed = ~0;
    uint16_t max_speed = 0;
    uint32_t sum_speed = 0;

    uint16_t min_sight = ~0;
    uint16_t max_sight = 0;
    uint32_t sum_sight = 0;

    int min_energy = ~(1 << 31);
    int max_energy = 0;
    int64_t sum_energy = 0;

    for (size_t i = 0; i < dots_.size(); ++i) {
      uint16_t dot_speed = dots_[i].getSpeed();
      uint16_t dot_sight = dots_[i].getSight();
      int dot_energy = dots_[i].getEnergy();
      if(dot_speed < min_speed){
        min_speed = dot_speed;
      }
      if(dot_sight < min_sight){
        min_sight = dot_sight;
      }
      if(dot_energy < min_energy){
        min_energy = dot_energy;
      }
      if(dot_speed > max_speed){
        max_speed = dot_speed;
      }
      if(dot_sight > max_sight){
        max_sight = dot_sight;
      }
      if(dot_energy > max_energy){
        max_energy = dot_energy;
      }
      sum_sight += dot_sight;
      sum_speed += dot_speed;
      sum_energy += dot_energy;
    }
    cout << "Min-/Max-/Avgwerte von " << dots_.size() << " Dots in Tick " << tick_ << ": " << endl;
    cout << "SIGHT: " << min_sight << "/" << max_sight << "/" <<(float)sum_sight/dots_.size() << endl;
    cout << "SPEED: " << min_speed << "/" << max_speed << "/" <<(float)sum_speed/dots_.size() << endl;
    cout << "ENERGY: " << min_energy << "/" << max_energy << "/" <<(float)sum_energy/dots_.size() << endl;
    cout << "Im aktuellen Tick sind " << food_.size() << " Futterstuecke auf dem Feld" << endl;
    if(debug || debug_env){
      int f = food_.size();
      int dots = dots_.size();
      int sum = 2 * f * log(f) + 2 * dots * (log(f) + log(f / 2) + pow(f * (sum_sight / dots) / testground_size_,2) + 2 * f * pow((sum_sight / dots) / testground_size_, 2));
      int sum2 = f * log(f) + dots * (log(f) + log(f / 2) + 2 * f * (sum_sight / dots) / testground_size_);
      cout << "Laufzeitanalyse Futtersuche (geschätzte Werte):\nNaiver Ansatz: " << f * dots << "\nSchnitt aus Intervallen aus sortierten Listen: " << sum << "\nIntervall einer sortierten Liste:  " << sum2 << endl;
    }
  } else {
    cout << "Im Tick " << tick_ << " sind keine Dots mehr am Leben." << endl;
  }
}
