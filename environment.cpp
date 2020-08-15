#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <math.h> //ceil

#include "environment.hpp"

#include "util.hpp"

using namespace std;

bool debug = false;
bool debug_env = false;

Environment::Environment():
  testground_size_(65535),
  food_vec_(),
  min_food_per_tick_(0),
  max_food_per_tick_(200),
  start_dot_(Dot(testground_size_)),
  dots_vec_(),
  tick_(0),
  mutagen_vec_()
{
  srand(time(NULL));
  contamination(1000);
}

Environment::Environment(const uint16_t testground_size, const int dot_count, const int min_food_count, const int max_food_count):
  testground_size_(min(testground_size, (uint16_t)65535)),
  food_vec_(),
  start_dot_(Dot(testground_size_)),
  dots_vec_(),
  tick_(0),
  mutagen_vec_()
{
  srand(time(NULL));
  changeFoodPerTick(min_food_count, max_food_count);
  contamination(dot_count);
}

Environment::Environment(const uint16_t testground_size, const int dot_count, const int min_food_count, const int max_food_count, const Dot start_dot):
  testground_size_(min(testground_size, (uint16_t)65535)),
  food_vec_(),
  start_dot_(start_dot),
  dots_vec_(),
  tick_(0),
  mutagen_vec_()
{
  srand(time(NULL));
  if(testground_size_ != start_dot_.getTestgroundSize()){
    start_dot_.setTestgroundSize(testground_size_);
  }
  changeFoodPerTick(min_food_count, max_food_count);
  contamination(dot_count);
}

void Environment::placeMutagen(std::vector<std::pair<std::uint16_t, std::uint16_t> > positions){
  mutagen_vec_.insert(mutagen_vec_.end(), positions.begin(), positions.end());
}

void Environment::placeMutagen(uint16_t amount /* = 1 */){
  for (size_t i = 0; i < amount; ++i) {
    uint16_t x_mut = rand() % testground_size_;
    uint16_t y_mut = rand() % testground_size_;
    mutagen_vec_.push_back(make_pair(x_mut, y_mut));
  }
}

void Environment::placeMutagen(std::pair<std::uint16_t, std::uint16_t> pos){
  mutagen_vec_.push_back(pos);
}

void Environment::clearMutagen(){
  mutagen_vec_.clear();
}

void Environment::tick(const int amount /* = 1 */){
  for (int i = 0; i < amount; ++i) {
    // Generate new Food
    feeding();
    // Trigger Tick of every Dot and let Dots with enough energy replicate
    searchFood();
    std::cout << __LINE__ << '\n';
    cannibalism();
    std::cout << __LINE__ << '\n';
    for (size_t i = 0; i < dots_vec_.size(); ++i) {
      dots_vec_[i].tick();
      if(dots_vec_[i].getReproductionCooldown() <= 0 && dots_vec_[i].getEnergy() >= 5000 + 10 * dots_vec_[i].getSize()){
        double mutation_rate = 0.2; // the background_mutationrate is constant at the moment, change here if wanted
        for (size_t i = 0; i < mutagen_vec_.size(); ++i) {
          uint16_t distance_to_mutagen = distance(dots_vec_[i].getPosition(), mutagen_vec_[i], testground_size_);
          mutation_rate += 1/(distance_to_mutagen * distance_to_mutagen + 1);
        }
        dots_vec_.push_back(dots_vec_[i].replicate(mutation_rate));
      }
    }
    std::cout << __LINE__ << '\n';
    // Delete Dot with 0 energy or less
    dots_vec_.erase(remove_if(dots_vec_.begin(), dots_vec_.end(), [](Dot &d){
      return d.getEnergy() <= 0;
    }), dots_vec_.end());
    ++tick_;

    if(debug || debug_env){
      cout << "Tick: " << tick_;
      cout << "\nLebende Dots: " << dots_vec_.size();
      cout << "\nMenge Futter auf dem Testfeld: " << food_vec_.size() << "\n";
    }
  }
}

void Environment::contamination(const int amount){
  for (int i = 0; i < amount; ++i) {
    dots_vec_.push_back(Dot(start_dot_, false)); // "false" prevents copying of postion and direction
  }
}

void Environment::changeFoodPerTick(const int min_amount, const int max_amount){
	if(min_amount > max_amount){
		min_food_per_tick_ = min_amount;
		max_food_per_tick_ = min_amount;
	} else {
		min_food_per_tick_ = min_amount;
		max_food_per_tick_ = max_amount;
	}
}

void Environment::feeding(){
  int food_count;
  if (max_food_per_tick_ != min_food_per_tick_){
    food_count = rand() % (max_food_per_tick_ - min_food_per_tick_) + min_food_per_tick_;
  } else {
    food_count = min_food_per_tick_;
  }
  for (int i = 0; i < food_count; ++i) {
    uint16_t x = rand() % testground_size_;
    uint16_t y = rand() % testground_size_;
    food_vec_.push_back(make_pair(x,y));
  }
}

void Environment::searchFood(){
  // Remove food exactly below Dots first
  std::vector<bool> remove_food(food_vec_.size(), false);
  for (size_t i = 0; i < dots_vec_.size(); ++i) {
    if(dots_vec_[i].getFoodInSight()){
      if(!(remove_food[dots_vec_[i].getFoodInSightIdx()]) && dots_vec_[i].getPosition() == food_vec_[dots_vec_[i].getFoodInSightIdx()]){
        dots_vec_[i].eat(1000);
        remove_food[dots_vec_[i].getFoodInSightIdx()] = true;
      }
    }
  }

  // Remove eaten food
  // food_vec_.erase(remove_if(food_vec_.begin(), food_vec_.end(), [& remove_food](std::pair<uint16_t, uint16_t> &food){
  //   return remove_food[];
  // }), food_vec_.end());
  for(size_t i = remove_food.size(); i != 0; --i){
    if(remove_food[i - 1]){
      food_vec_.erase(food_vec_.begin() + i - 1);
    }
  }
  for(size_t i = 0; i < dots_vec_.size(); ++i){
    dots_vec_[i].setFoodInSight(false);
  }
  // Calculate distance for the rest-food
  // Based on this answer: https://stackoverflow.com/a/59432406/12540220
  uint16_t grid_size = 50;
  uint16_t grid_length = testground_size_ / grid_size;
  // Only save indices of Dots in dots_vec_
  std::vector<std::vector<std::vector<size_t> > > grid(grid_size, std::vector<std::vector<size_t> > (grid_size));
  for(size_t i = 0; i < dots_vec_.size(); ++i){
    const std::pair<uint16_t, uint16_t>& dot_pos = dots_vec_[i].getPosition();
    const std::pair<uint16_t, uint16_t> grid_pos(dot_pos.first / grid_length, dot_pos.second / grid_length);
    uint16_t neighborhood = ceil(dots_vec_[i].getSight()/grid_length);
    for(int16_t x = grid_pos.first - neighborhood; x <= grid_pos.first + neighborhood; ++x){
      for(int16_t y = grid_pos.second - neighborhood; y <= grid_pos.second + neighborhood; ++y){
        grid[((x + grid_size) % grid_size)][((y + grid_size) % grid_size)].push_back(i);
      }
    }
  }
  for(size_t i = 0; i < food_vec_.size(); ++i){
    std::pair<uint16_t, uint16_t> grid_pos(food_vec_[i].first / grid_length, food_vec_[i].second / grid_length);
    for(size_t j = 0; j < grid[grid_pos.first][grid_pos.second].size(); ++j){
      Dot& dot = dots_vec_[grid[grid_pos.first][grid_pos.second][j]];
      uint16_t dist = distance(food_vec_[i], dot.getPosition(), testground_size_);
      if(dist < dot.getSight()){
        dot.newFoodSource(food_vec_[i], i, dist);
      }
    }
  }
}

void Environment::cannibalism(){
  std::cout << __LINE__ << '\n';
  // Remove food exactly below Dots first
  std::vector<bool> remove_dot(dots_vec_.size(), false);
  for (size_t i = 0; i < dots_vec_.size(); ++i) {
    if(dots_vec_[i].getPreyInSight()){
      if(!(remove_dot[dots_vec_[i].getPreyInSightIdx()]) && dots_vec_[i].getPosition() == dots_vec_[dots_vec_[i].getPreyInSightIdx()].getPosition()){
        dots_vec_[i].eat(500 + 0.3 * dots_vec_[dots_vec_[i].getPreyInSightIdx()].getEnergy());
        remove_dot[dots_vec_[i].getPreyInSightIdx()] = true;
      }
    }
  }
  std::cout << __LINE__ << '\n';
  // Remove eaten dots
  // dots_vec_.erase(remove_if(dots_vec_.begin(), dots_vec_.end(), [& remove_dot](std::pair<uint16_t, uint16_t> &dot){
  //   return remove_dot[];
  // }), dots_vec_.end());
  for(size_t i = remove_dot.size(); i != 0; --i){
    if(remove_dot[i - 1]){
      dots_vec_.erase(dots_vec_.begin() + i - 1);
    }
  }
  for(size_t i = 0; i < dots_vec_.size(); ++i){
    dots_vec_[i].setPreyInSight(false);
  }
  std::cout << __LINE__ << '\n';
  // Calculate distance for the rest-food
  // Based on this answer: https://stackoverflow.com/a/59432406/12540220
  uint16_t grid_size = 50;
  uint16_t grid_length = testground_size_ / grid_size;
  // Only save indices of Dots in dots_vec_
  std::vector<std::vector<std::vector<size_t> > > grid(grid_size, std::vector<std::vector<size_t> > (grid_size));
  for(size_t i = 0; i < dots_vec_.size(); ++i){
    const std::pair<uint16_t, uint16_t>& dot_pos = dots_vec_[i].getPosition();
    const std::pair<uint16_t, uint16_t> grid_pos(dot_pos.first / grid_length, dot_pos.second / grid_length);
    uint16_t neighborhood = ceil(dots_vec_[i].getSight()/grid_length);
    for(int16_t x = grid_pos.first - neighborhood; x <= grid_pos.first + neighborhood; ++x){
      for(int16_t y = grid_pos.second - neighborhood; y <= grid_pos.second + neighborhood; ++y){
        grid[((x + grid_size) % grid_size)][((y + grid_size) % grid_size)].push_back(i);
      }
    }
  }
  std::cout << __LINE__ << '\n';
  for(size_t i = 0; i < dots_vec_.size(); ++i){
    const std::pair<uint16_t, uint16_t>& dot_pos = dots_vec_[i].getPosition();
    std::pair<uint16_t, uint16_t> grid_pos(dot_pos.first / grid_length, dot_pos.second / grid_length);
    for(size_t j = 0; j < grid[grid_pos.first][grid_pos.second].size(); ++j){
      Dot& dot = dots_vec_[grid[grid_pos.first][grid_pos.second][j]];
      uint16_t dist = distance(dot_pos, dot.getPosition(), testground_size_);
      if(dist < dot.getSight()){
        if(dots_vec_[i].getSize() > dot.getSize() * 1.1){
          dots_vec_[i].newPrey(dot.getPosition(), i, dist);
        } else if (dots_vec_[i].getSize() * 1.1 < dot.getSize()){
          dots_vec_[i].newHazardSource(dot.getPosition(), dist);
        }
      }
    }
  }
  std::cout << __LINE__ << '\n';
}

//   // sort the Dot-vector controlled by the position of the Dots
//   sort(dots_vec_.begin(), dots_vec_.end(), [](const Dot& a, const Dot& b){
//     return a.getPosition() < b.getPosition();
//   });
//   // save the position of the Dots alone
//   vector<pair<uint16_t, uint16_t> > dot_positions(dots_vec_.size());
//   for (size_t i = 0; i < dots_vec_.size(); ++i) {
//     dot_positions[i] = dots_vec_[i].getPosition();
//   }
//   for (size_t i = 0; i < dot_positions.size(); ++i) {
//     // Calculate Min. und Max. X-values for each Dot
//     uint16_t min_x = (dot_positions[i].first - dots_vec_[i].getSight() + testground_size_) % testground_size_;
//     uint16_t max_x = (dot_positions[i].first + dots_vec_[i].getSight() + testground_size_) % testground_size_;
//
//     // Find interval for x values of food_vec_
//     auto interval_start = find_if(dot_positions.begin(), dot_positions.end(), [& min_x](const pair<uint16_t, uint16_t>& dot) {
//       return dot.first >= min_x;
//     });
//     auto interval_end = find_if(dot_positions.begin(), dot_positions.end(), [& max_x](const pair<uint16_t, uint16_t>& dot) {
//       return dot.first > max_x;
//     });
//     uint16_t min_distance;
//     do {
//       // Calculate exact distance for food in interval
//       auto min_it = interval_end;
//       min_distance = ~0;
//       if(interval_end <= interval_start){
//         // Dots close to the edge can see food sources on the other side of
//         // the map. This creates some kinda weird interval:
//         // |-.--->         <--|, so we need 2 for-loops
//         for (auto j = dot_positions.begin(); j < interval_end; ++j) {
//           uint16_t dist = distance(dot_positions[i], *j, testground_size_);
//           if(dist < min_distance){
//             min_it = j;
//             min_distance = dist;
//           }
//         }
//         for (auto j = interval_start; j < dot_positions.end(); ++j) {
//           uint16_t dist = distance(dot_positions[i], *j, testground_size_);
//           if(dist < min_distance){
//             min_it = j;
//             min_distance = dist;
//           }
//         }
//       } else {
//         // normal case: |  <---.--->       |
//         for (auto j = interval_start; j < interval_end; ++j) {
//           uint16_t dist = distance(dot_positions[i], *j, testground_size_);
//           if(dist < min_distance){
//             min_it = j;
//             min_distance = dist;
//           }
//         }
//       }
//       // Tell the Dot about the other Dot
//       if(min_distance == 0){
//         // Let the bigger Dot eat the other Dot. A size difference of at least 10 percent is needed.
//         // The energy gained is a constant value plus a procentual part of the eaten Dots energy.
//         if(dots_vec_[i].getSize() > dots_vec_[min_it - dot_positions.begin()].getSize() * 1.1){
//           dots_vec_[i].eat(500 + 0.3 * dots_vec_[min_it - dot_positions.begin()].getEnergy());
//           dots_vec_.erase((min_it - dot_positions.begin()) + dots_vec_.begin());
//           dot_positions.erase(min_it);
//           --interval_end;
//         } else if(dots_vec_[i].getSize() * 1.1 < dots_vec_[min_it - dot_positions.begin()].getSize()) {
//           dots_vec_[min_it - dot_positions.begin()].eat(500 + 0.3 * dots_vec_[i].getEnergy());
//           dots_vec_.erase(i + dots_vec_.begin());
//           dot_positions.erase(i + dot_positions.begin());
//           break;
//         } else {
//           break;
//         }
//       } else if(min_distance < dots_vec_[i].getSight()){
//         // Fight or Flight? Bigger Dots chase, smaller Dots flee
//         if(dots_vec_[i].getSize() > dots_vec_[min_it - dot_positions.begin()].getSize() * 1.1){
//           dots_vec_[i].newFoodSource(*min_it);
//         } else{
//           dots_vec_[i].newHazardSource(*min_it);
//         }
//       }
//     } while(min_distance == 0);
//   }
// }

void Environment::printMap(){
  // Scale the the Environment down to 100x100 if testground_size_ is bigger than 100
  uint16_t scale = min((uint16_t)100, testground_size_);
  // Save scaled positions of Dots and foodsources
  vector<vector<int> > map_dot_vec(scale, vector<int>(scale));
  for (size_t i = 0; i < dots_vec_.size(); ++i) {
    ++map_dot_vec[(dots_vec_[i].getPosition().first) * scale / testground_size_][(dots_vec_[i].getPosition().second) * scale / testground_size_];
  }
  vector<vector<int> > map_food_vec(scale, vector<int>(scale));
  for (size_t i = 0; i < food_vec_.size(); ++i) {
    ++map_food_vec[(food_vec_[i].first) * scale / testground_size_][(food_vec_[i].second) * scale / testground_size_];
  }
  vector<vector<int> > map_mutagen_vec(scale, vector<int>(scale));
  for (size_t i = 0; i < mutagen_vec_.size(); ++i) {
    ++map_mutagen_vec[(mutagen_vec_[i].first) * scale / testground_size_][(mutagen_vec_[i].second) * scale / testground_size_];
  }
  // Output
  cout << "\n--- Aktuelle Karte der Testumgebung ---\n";
  for (uint16_t j = 0; j < scale; ++j) {
    for (uint16_t i = 0; i < scale; ++i) {
      if (map_mutagen_vec[i][j] != 0) {
        cout << "!";
      } else if (map_dot_vec[i][j] != 0) {
        if (map_food_vec[i][j] != 0) {
          cout << "%";
        } else {
          cout << ".";
        }
      } else if (map_food_vec[i][j] != 0) {
        cout << "x";
      } else {
        cout << " ";
      }
    }
    cout << "|\n";
  }
  cout << "Legende: ' ' Nichts, '!' Mutagen, '.' Dot, 'x' Essen, '%' Dot und Essen\nBei mehreren Objekten auf demselben Punkt wird nur eins angezeigt\nDie Ausgabe ist skaliert auf " << scale << "x" << scale << "\n";
}

void Environment::printTestground(){
  cout << "\n--- Eigenschaften der Testumgebung ---\nTestgroundsize: " << testground_size_ << "\nMin-|Maxfutter pro Tick: " << min_food_per_tick_ << "|"<< max_food_per_tick_ << "\nTick: " << tick_ << "\n#Dots: " << dots_vec_.size() << "\n#Futter: " << food_vec_.size() << "\n";
}

void Environment::printProperties(){
  if(dots_vec_.size() != 0){
    std::vector<uint16_t> speed_count_vec;
    uint16_t min_speed = ~0;
    uint16_t max_speed = 0;

    std::vector<uint16_t> sight_count_vec;
    uint16_t min_sight = ~0;
    uint16_t max_sight = 0;

    std::vector<uint16_t> size_count_vec;
    uint16_t min_size = ~0;
    uint16_t max_size = 0;

    int min_energy = ~(1 << 31);
    int max_energy = 0;
    int64_t sum_energy = 0;

    for (size_t i = 0; i < dots_vec_.size(); ++i) {
      // Get() properties once and save them
      uint16_t dot_speed = dots_vec_[i].getSpeed();
      uint16_t dot_sight = dots_vec_[i].getSight();
      uint16_t dot_size = dots_vec_[i].getSize();
      int dot_energy = dots_vec_[i].getEnergy();

      // Count occurences of specific values
      if(dot_speed > speed_count_vec.size()){
        speed_count_vec.resize(dot_speed);
      }
      ++speed_count_vec[dot_speed - 1];

      if(dot_sight > sight_count_vec.size()){
        sight_count_vec.resize(dot_sight);
      }
      ++sight_count_vec[dot_sight - 1];

      if(dot_size > size_count_vec.size()){
        size_count_vec.resize(dot_size);
      }
      ++size_count_vec[dot_size - 1];

      // Compare to current minimum
      if(dot_speed < min_speed){
        min_speed = dot_speed;
      }

      if(dot_sight < min_sight){
        min_sight = dot_sight;
      }

      if(dot_size < min_size){
        min_size = dot_size;
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

      if(dot_size > max_size){
        max_size = dot_size;
      }

      if(dot_energy > max_energy){
        max_energy = dot_energy;
      }

      // Add to sum, needed for average
      sum_energy += dot_energy;
    }
    uint32_t sum_speed = 0;
    for (size_t i = 1; i <= speed_count_vec.size(); ++i) {
      sum_speed += speed_count_vec[i-1]*(i);
    }

    uint32_t sum_sight = 0;
    for (size_t i = 1; i <= sight_count_vec.size(); ++i) {
      sum_sight += sight_count_vec[i-1]*(i);
    }

    uint32_t sum_size = 0;
    for (size_t i = 1; i <= size_count_vec.size(); ++i) {
      sum_size += size_count_vec[i-1]*(i);
    }

    cout << "\n--- Werte von " << dots_vec_.size() << " Dots in Tick " << tick_ << " ---\n";
    cout << "        | MIN | MAX | AVG |\n";
    cout << "SPEED:  |" << niceNumberPrint(min_speed, 5) << "|" << niceNumberPrint(max_speed, 5) << "|" << niceNumberPrint((float)sum_speed / dots_vec_.size(), 5) << "|\n";
    cout << "SIGHT:  |" << niceNumberPrint(min_sight, 5) << "|" << niceNumberPrint(max_sight, 5) << "|" << niceNumberPrint((float)sum_sight / dots_vec_.size(), 5) << "|\n";
    cout << "SIZE:   |" << niceNumberPrint(min_size, 5) << "|" << niceNumberPrint(max_size, 5) << "|" << niceNumberPrint((float)sum_size / dots_vec_.size(), 5) << "|\n";
    cout << "ENERGY: |" << niceNumberPrint(min_energy, 5) << "|" << niceNumberPrint(max_energy, 5) << "|" << niceNumberPrint((float)sum_energy / dots_vec_.size(), 5) << "|\n";

    // Output of count-vectors
    cout << "\nGenaue Speedwerte:";
    std::string line1 = "\nWert:   |";
    std::string line2 = "\nAnzahl: |";
    for (size_t i = min_speed; i <= speed_count_vec.size(); ++i) {
      if (speed_count_vec[i-1] != 0){
        line1 += niceNumberPrint(i, 3);
        line2 += niceNumberPrint(speed_count_vec[i-1], 3);
      }
      line1 += "|";
      line2 += "|";
    }
    cout << line1 << line2;
    cout << "\nGenaue Sightwerte:";
    line1 = "\nWert:   |";
    line2 = "\nAnzahl: |";
    for (size_t i = min_sight; i <= sight_count_vec.size(); ++i) {
      if (sight_count_vec[i-1] != 0){
        line1 += niceNumberPrint(i, 3);
        line2 += niceNumberPrint(sight_count_vec[i-1], 3);
      }
      line1 += "|";
      line2 += "|";
    }
    cout << line1 << line2;
    cout << "\nGenaue Sizewerte:";
    line1 = "\nWert:   |";
    line2 = "\nAnzahl: |";
    for (size_t i = min_size; i <= size_count_vec.size(); ++i) {
      if (size_count_vec[i-1] != 0){
        line1 += niceNumberPrint(i, 3);
        line2 += niceNumberPrint(size_count_vec[i-1], 3);
      }
      line1 += "|";
      line2 += "|";
    }
    cout << line1 << line2;
    cout << "\nIm aktuellen Tick sind " << food_vec_.size() << " Futterstuecke auf dem Feld\n";
    if(debug || debug_env){
      int f = food_vec_.size();
      int dots = dots_vec_.size();
      int sum = 2 * f * log(f) + 2 * dots * (log(f) + log(f / 2) + pow(f * (sum_sight / dots) / testground_size_,2) + 2 * f * pow((sum_sight / dots) / testground_size_, 2));
      int sum2 = f * log(f) + dots * (log(f) + log(f / 2) + 2 * f * (sum_sight / dots) / testground_size_);
      cout << "Laufzeitanalyse Futtersuche (geschätzte Werte):\nSchnitt aus Intervallen aus sortierten Listen: " << sum << "\nIntervall einer sortierten Liste:  " << sum2 << "\n";
    }
  } else {
    cout << "Im Tick " << tick_ << " sind keine Dots mehr am Leben.\n";
  }
}
