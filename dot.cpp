#include <iostream>
#include <math.h>

#include "dot.hpp"

#include "util.hpp"

using namespace std;

extern bool debug; // "extern" tells the compiler that debug is already declared somewhere else (environment.cpp)
bool debug_dot = false;

Dot::Dot(const uint16_t testground_size):
//start dots
  speed_(10),
  sight_(50),
  size_(10),
  energy_(7500),
  reproduction_cooldown_(20),
  testground_size_(testground_size),
  food_in_sight_(false),
  prey_in_sight_(false),
  hazard_in_sight_(false)
{
  uint16_t x = rand() % testground_size_;
  uint16_t y = rand() % testground_size_;
  position_ = make_pair(x, y);
  direction_ = rand() % 360;
  food_in_sight_pos_ = make_pair(0,0);
  prey_in_sight_pos_ = make_pair(0,0);
  hazard_in_sight_pos_ = make_pair(0,0);
}

Dot::Dot(const uint16_t testground_size, const int energy, const uint16_t speed, const uint16_t sight, const uint16_t size, const pair<uint16_t, uint16_t> pos):
// start dots or replication
  speed_(speed),
  sight_(sight),
  size_(size),
  energy_(energy),
  reproduction_cooldown_(20),
  testground_size_(testground_size),
  food_in_sight_(false),
  prey_in_sight_(false),
  hazard_in_sight_(false),
  position_(pos)
{
  direction_ = rand() % 360;
  food_in_sight_pos_ = make_pair(0,0);
  prey_in_sight_pos_ = make_pair(0,0);
  hazard_in_sight_pos_ = make_pair(0,0);
}

Dot::Dot(const Dot &d, bool exact_copy /* = true*/):
  speed_(d.speed_),
  sight_(d.sight_),
  size_(d.size_),
  energy_(d.energy_),
  reproduction_cooldown_(d.reproduction_cooldown_),
  testground_size_(d.testground_size_),
  food_in_sight_(d.food_in_sight_),
  food_in_sight_idx_(d.food_in_sight_idx_),
  food_in_sight_pos_(d.food_in_sight_pos_),
  prey_in_sight_(d.prey_in_sight_),
  prey_in_sight_idx_(d.prey_in_sight_idx_),
  prey_in_sight_pos_(d.prey_in_sight_pos_),
  hazard_in_sight_(d.hazard_in_sight_),
  hazard_in_sight_pos_(d.hazard_in_sight_pos_)
{
	if(exact_copy){
  	position_ = d.position_;
  	direction_ = d.direction_;
	} else {
		uint16_t x = rand() % testground_size_;
		uint16_t y = rand() % testground_size_;
		position_ = make_pair(x, y);
		direction_ = rand() % 360;
	}
}

void Dot::tick(){
  // First priority is to flee from hazards
  if (hazard_in_sight_){
    if (distance(position_, hazard_in_sight_pos_, testground_size_) > sight_ * 2){
      hazard_in_sight_ = false;
    } else {
      direction_ = (direction(position_, hazard_in_sight_pos_, testground_size_) + 180) % 360;
      position_ = move(position_, direction_, speed_, testground_size_);
      energy_ -= pow(speed_, 2);
    }
  }
  // This can't be just an else-block since hazard_in_sight_ may change in the first block to false
  if(!hazard_in_sight_){
    // Second priority is to hunt preys
    if(prey_in_sight_){
      if(prey_in_sight_dist_ < speed_){
        position_ = prey_in_sight_pos_;
        energy_ -= prey_in_sight_dist_ * speed_;
        // TODO: Trigger deletion of Prey
      } else {
        direction_ = direction(position_, prey_in_sight_pos_, testground_size_);
        position_ = move(position_, direction_, speed_, testground_size_);
        energy_ -= pow(speed_, 2);
      }
    }
    // Third priority is to move to food
    if (food_in_sight_){
      if(food_in_sight_dist_ < speed_){
        position_ = food_in_sight_pos_;
        energy_ -= food_in_sight_dist_ * speed_;
      } else {
        direction_ = direction(position_, food_in_sight_pos_, testground_size_);
        position_ = move(position_, direction_, speed_, testground_size_);
        energy_ -= pow(speed_, 2);
      }
    // Last option is to just randomly walk around
    } else{
      direction_ += ((rand() % 21) - 10) % 360;
      position_ = move(position_, direction_, speed_, testground_size_);
      energy_ -= pow(speed_, 2);
    }
  }
  // Substract basic consumption
  energy_ -= sight_;
  energy_ -= size_;
  // Reduce reproduction cooldown
  --reproduction_cooldown_;
}

Dot Dot::replicate(const double mutation_rate){
  uint16_t speed_change = max(1.0, (double)speed_ * mutation_rate);
  uint16_t sight_change = max(1.0, (double)sight_ * mutation_rate);
  uint16_t size_change = max(1.0, (double)size_ * mutation_rate);
  uint16_t speed_new = max(speed_ + rand() % (speed_change * 2 + 1) - speed_change, 1);
  uint16_t sight_new = max(sight_ + rand() % (sight_change * 2 + 1) - sight_change, 1);
  uint16_t size_new = max(size_ + rand() % (size_change * 2 + 1) - size_change, 1);
  uint16_t x_new = (position_.first + rand() % (sight_ * 2 + 1) - sight_) % testground_size_;
  uint16_t y_new = (position_.second + rand() % (sight_ * 2 + 1) - sight_) % testground_size_;
  Dot child(testground_size_, energy_ / 2, speed_new, sight_new, size_new, make_pair(x_new, y_new));
  energy_ /= 2;
  reproduction_cooldown_ = 20;
  return child;
}

void Dot::newFoodSource(pair<uint16_t, uint16_t> food_pos, size_t idx, uint16_t dist){
  if(!food_in_sight_ || dist < food_in_sight_dist_){
    food_in_sight_ = true;
    food_in_sight_pos_ = food_pos;
    food_in_sight_idx_ = idx;
    food_in_sight_dist_ = dist;
  }
}

void Dot::newPrey(pair<uint16_t, uint16_t> prey_pos, size_t idx, uint16_t dist){
  if(!prey_in_sight_ || dist < prey_in_sight_dist_){
    prey_in_sight_ = true;
    prey_in_sight_pos_ = prey_pos;
    prey_in_sight_idx_ = idx;
    prey_in_sight_dist_ = dist;
  }
}

void Dot::newHazardSource(pair<uint16_t, uint16_t> hazard_pos, uint16_t dist){
  hazard_in_sight_ = true;
  hazard_in_sight_pos_ = hazard_pos;
  hazard_in_sight_dist_ = dist;
}

void Dot::eat(int amount){
  energy_ += amount;
}

// Get()- and Set()-Functions
int Dot::getEnergy() const{
  return energy_;
}

pair<uint16_t, uint16_t> Dot::getPosition() const{
  return position_;
}

int Dot::getReproductionCooldown() const{
  return reproduction_cooldown_;
}

uint16_t Dot::getSpeed() const{
  return speed_;
}

uint16_t Dot::getSight() const{
  return sight_;
}

uint16_t Dot::getSize() const{
  return size_;
}

uint16_t Dot::getTestgroundSize() const{
  return testground_size_;
}

void Dot::setTestgroundSize(uint16_t testground_size){
  testground_size_ = testground_size;
}

bool Dot::getFoodInSight() const{
  return food_in_sight_;
}

void Dot::setFoodInSight(bool food_in_sight){
  food_in_sight_ = food_in_sight;
}

std::size_t Dot::getFoodInSightIdx() const{
  return food_in_sight_idx_;
}

bool Dot::getPreyInSight() const{
  return prey_in_sight_;
}

void Dot::setPreyInSight(bool prey_in_sight){
  prey_in_sight_ = prey_in_sight;
}

std::size_t Dot::getPreyInSightIdx() const{
  return prey_in_sight_idx_;
}

void Dot::setPreyInSightIdx(size_t idx){
  prey_in_sight_idx_ = idx;
}
