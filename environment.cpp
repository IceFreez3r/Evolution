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
  food_(),
  min_food_per_tick_(0),
  max_food_per_tick_(200),
  dots_()
{
  srand(time(NULL));
  tick_ = 0;
  start_dot_ = Dot(testground_size_);
  contamination(1000);
}

Environment::Environment(const uint16_t testground_size, const int dot_count, const int min_food_count, const int max_food_count):
  testground_size_(min(testground_size, (uint16_t)65535)),
  food_(),
  min_food_per_tick_(min_food_count),
  max_food_per_tick_(max_food_count),
  dots_()
{
  srand(time(NULL));
  tick_ = 0;
  start_dot_ = Dot(testground_size_);
  contamination(dot_count);
}

Environment::Environment(const uint16_t testground_size, const int dot_count, const int min_food_count, const int max_food_count, const Dot start_dot):
  testground_size_(min(testground_size, (uint16_t)65535)),
  food_(),
  min_food_per_tick_(min_food_count),
  max_food_per_tick_(max_food_count),
  dots_()
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
    searchFood();
    for (size_t i = 0; i < dots_.size(); ++i) {
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
      cout << "Tick: " << tick_;
      cout << "\nLebende Dots: " << dots_.size();
      cout << "\nMenge Futter auf dem Testfeld: " << food_.size() << "\n";
    }
  }
}

// Places new Dots at random locations on the map.
void Environment::contamination(const int amount){
  for (int i = 0; i < amount; ++i) {
    dots_.push_back(Dot(start_dot_, false)); // "false" prevents copying of postion and direction
  }
}

void Environment::feeding(const int min_amount, const int max_amount /* = 0 */){
  int food_count;
  if (min_amount >= max_amount) {
    food_count = min_amount;
  } else {
    food_count = rand() % (max_amount - min_amount) + min_amount;
  }
  for (int i = 0; i < food_count; ++i) {
    uint16_t x = rand() % testground_size_;
    uint16_t y = rand() % testground_size_;
    food_.push_back(make_pair(x,y));
  }
}

void Environment::searchFood(){
  sort(food_.begin(), food_.end());
  if (debug || debug_env) {
    for (size_t i = 0; i < food_.size(); ++i) {
      cout << "(" << food_[i].first << "," << food_[i].first << ")\n";
    }
  }
  for (size_t i = 0; i < dots_.size(); ++i) {
    // Calculate Min. und Max. X-values for each Dot
    uint16_t min_x = (dots_[i].getPosition().first - dots_[i].getSight() + testground_size_) % testground_size_;
    uint16_t max_x = (dots_[i].getPosition().first + dots_[i].getSight() + testground_size_) % testground_size_;

    // Find interval for x values of food_
    auto interval_start = find_if(food_.begin(), food_.end(), [&min_x](const pair<uint16_t, uint16_t>& food) {
      return food.first >= min_x;
    });
    auto interval_end = find_if(food_.begin(), food_.end(), [&max_x](const pair<uint16_t, uint16_t>& food) {
      return food.first > max_x;
    });
    uint16_t min_distance;
    do {
      // Calculate exact distance for food in interval
      auto min_it = interval_end;
      min_distance = ~0;
      if(interval_end <= interval_start){
        // Dots close to the edge can see food sources on the other side of
        // the map. This creates some kinda weird interval:
        // |-.--->         <--|, so we need 2 for-loops
        for (auto j = food_.begin(); j < interval_end; ++j) {
          uint16_t dist = distance(dots_[i].getPosition(), food_[j - food_.begin()], testground_size_);
          if(dist < min_distance){
            min_it = j;
            min_distance = dist;
          }
        }
        for (auto j = interval_start; j < food_.end(); ++j) {
          uint16_t dist = distance(dots_[i].getPosition(), food_[j - food_.begin()], testground_size_);
          if(dist < min_distance){
            min_it = j;
            min_distance = dist;
          }
        }
      } else {
        // normal case: |  <---.--->       |
        for (auto j = interval_start; j < interval_end; ++j) {
          uint16_t dist = distance(dots_[i].getPosition(), food_[j - food_.begin()], testground_size_);
          if(dist < min_distance){
            min_it = j;
            min_distance = dist;
          }
        }
      }
      // Tell the Dot about the Food
      if(min_distance == 0){
        dots_[i].eat(1000);
        food_.erase(min_it);
        --interval_end;
      } else if(min_distance < dots_[i].getSight()){
        dots_[i].newFoodSource(food_[min_it - food_.begin()]);
      }
    } while(min_distance == 0);
  }
}

void Environment::printMap(){
  // Scale the the Environment down to 100x100 if testground_size_ is bigger than 100
  uint16_t scale = min((uint16_t)100, testground_size_);
  // Save scaled positions of Dots and foodsources
  vector<vector<int>> map_d(scale, vector<int>(scale));
  for (size_t i = 0; i < dots_.size(); ++i) {
    ++map_d[(dots_[i].getPosition().first) * scale / testground_size_][(dots_[i].getPosition().second) * scale / testground_size_];
  }
  vector<vector<int>> map_f(scale, vector<int>(scale));
  for (size_t i = 0; i < food_.size(); ++i) {
    ++map_f[(food_[i].first) * scale / testground_size_][(food_[i].second) * scale / testground_size_];
  }
  // Output
  cout << "\n--- Aktuelle Karte der Testumgebung ---\n";
  for (uint16_t i = 0; i < scale; ++i) {
    for (uint16_t j = 0; j < scale; ++j) {
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
    cout << "\n";
  }
  cout << "Legende: ' ' Nichts, '.' Dot, 'x' Essen, '%' Dot und Essen\nBei mehreren Objekten auf demselben Punkt wird nur eins angezeigt\nDie Ausgabe ist skaliert auf " << scale << "x" << scale << "\n";
}

void Environment::printTestground(){
  cout << "\n--- Eigenschaften der Testumgebung ---\nTestgroundsize: " << testground_size_ << "\nMin-|Maxfutter pro Tick: " << min_food_per_tick_ << "|"<< max_food_per_tick_ << "\nTick: " << tick_ << "\n#Dots: " << dots_.size() << "\n#Futter: " << food_.size() << "\n";
}

void Environment::printProperties(){
  if(dots_.size() != 0){
    std::vector<uint16_t> speed_count_vec;
    uint16_t min_speed = ~0;
    uint16_t max_speed = 0;

    std::vector<uint16_t> sight_count_vec;
    uint16_t min_sight = ~0;
    uint16_t max_sight = 0;

    int min_energy = ~(1 << 31);
    int max_energy = 0;
    int64_t sum_energy = 0;

    for (size_t i = 0; i < dots_.size(); ++i) {
      // Get() properties once and save them
      uint16_t dot_speed = dots_[i].getSpeed();
      uint16_t dot_sight = dots_[i].getSight();
      int dot_energy = dots_[i].getEnergy();

      // Count occurences of specific values
      if(dot_speed > speed_count_vec.size()){
        speed_count_vec.resize(dot_speed);
      }
      ++speed_count_vec[dot_speed - 1];
      if(dot_sight > sight_count_vec.size()){
        sight_count_vec.resize(dot_sight);
      }
      ++sight_count_vec[dot_sight - 1];

      // Compare to current minimum
      if(dot_speed < min_speed){
        min_speed = dot_speed;
      }
      if(dot_sight < min_sight){
        min_sight = dot_sight;
      }
      if(dot_energy < min_energy){
        min_energy = dot_energy;
      }

      // Compare to current maximum
      if(dot_speed > max_speed){
        max_speed = dot_speed;
      }
      if(dot_sight > max_sight){
        max_sight = dot_sight;
      }
      if(dot_energy > max_energy){
        max_energy = dot_energy;
      }

      // Add to sum, needed for average
      sum_energy += dot_energy;
    }
    uint32_t sum_sight = 0;
    for (size_t i = 1; i <= sight_count_vec.size(); ++i) {
      sum_sight += sight_count_vec[i-1]*(i);
    }
    uint32_t sum_speed = 0;
    for (size_t i = 1; i <= speed_count_vec.size(); ++i) {
      sum_speed += speed_count_vec[i-1]*(i);
    }
    cout << "\n--- Werte von " << dots_.size() << " Dots in Tick " << tick_ << " ---\n";
    cout << "        | MIN | MAX | AVG |\n";
    cout << "SIGHT:  |" << niceNumberPrint(min_sight, 5) << "|" << niceNumberPrint(max_sight, 5) << "|" << niceNumberPrint((float)sum_sight/dots_.size(), 5) << "|\n";
    cout << "SPEED:  |" << niceNumberPrint(min_speed, 5) << "|" << niceNumberPrint(max_speed, 5) << "|" << niceNumberPrint((float)sum_speed/dots_.size(), 5) << "|\n";
    cout << "ENERGY: |" << niceNumberPrint(min_energy, 5) << "|" << niceNumberPrint(max_energy, 5) << "|" << niceNumberPrint((float)sum_energy/dots_.size(), 5) << "|\n";

    // Output of count-vectors
    cout << "Genaue Sightwerte:";
    std::string line1 = "\nWert:   |";
    std::string line2 = "\nAnzahl: |";
    for (size_t i = min_sight; i <= sight_count_vec.size(); ++i) {
      if (sight_count_vec[i-1] != 0){
        line1 += niceNumberPrint(i, 3);
        line2 += niceNumberPrint(sight_count_vec[i-1], 3);
      }
      line1 += "|";
      line2 += "|";
    }
    cout << line1 << line2;
    cout << "\nGenaue Speedwerte:";
    line1 = "\nWert:   |";
    line2 = "\nAnzahl: |";
    for (size_t i = min_speed; i <= speed_count_vec.size(); ++i) {
      if (speed_count_vec[i-1] != 0){
        line1 += niceNumberPrint(i, 3);
        line2 += niceNumberPrint(speed_count_vec[i-1], 3);
      }
      line1 += "|";
      line2 += "|";
    }
    cout << line1 << line2;

    cout << "\nIm aktuellen Tick sind " << food_.size() << " Futterstuecke auf dem Feld\n";
    if(debug || debug_env){
      int f = food_.size();
      int dots = dots_.size();
      int sum = 2 * f * log(f) + 2 * dots * (log(f) + log(f / 2) + pow(f * (sum_sight / dots) / testground_size_,2) + 2 * f * pow((sum_sight / dots) / testground_size_, 2));
      int sum2 = f * log(f) + dots * (log(f) + log(f / 2) + 2 * f * (sum_sight / dots) / testground_size_);
      cout << "Laufzeitanalyse Futtersuche (geschätzte Werte):\nNaiver Ansatz: " << f * dots << "\nSchnitt aus Intervallen aus sortierten Listen: " << sum << "\nIntervall einer sortierten Liste:  " << sum2 << "\n";
    }
  } else {
    cout << "Im Tick " << tick_ << " sind keine Dots mehr am Leben.\n";
  }
}
