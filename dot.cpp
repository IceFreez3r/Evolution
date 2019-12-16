#include <iostream>
#include <math.h>

#include "dot.hpp"

#include "util.hpp"

using namespace std;

extern bool debug; // "extern" tells the compiler that debug is already declared somewhere else (environment.cpp)
bool debug_dot = false;

Dot::Dot(const uint16_t testground_size):
  speed_(10),
  sight_(50)
{
  energy_ = 5000;
  reproduction_cooldown_ = 20;
  testground_size_ = testground_size;
  uint16_t x = rand() % testground_size_;
  uint16_t y = rand() % testground_size_;
  position_ = make_pair(x, y);
  direction_ = rand() % 360;
  food_in_sight_ = false;
  food_in_sight_pos_ = make_pair(0,0);
}

Dot::Dot(const uint16_t testground_size, const int energy, const uint16_t speed, const uint16_t sight, const pair<uint16_t, uint16_t> pos):
  speed_(speed),
  sight_(sight)
{
  energy_ = energy;
  reproduction_cooldown_ = 20;
  testground_size_ = testground_size;
  position_ = pos;
  direction_ = rand() % 360;
  food_in_sight_ = false;
  food_in_sight_pos_ = make_pair(0,0);
}

Dot::Dot(const Dot &d):
  speed_(d.speed_),
  sight_(d.sight_)
{
  energy_ = d.energy_;
  position_ = d.position_;
  reproduction_cooldown_ = d.reproduction_cooldown_;
  direction_ = d.direction_;
  testground_size_ = d.testground_size_;
  food_in_sight_ = d.food_in_sight_;
  food_in_sight_pos_ = d.food_in_sight_pos_;
}

void Dot::tick(){
  if (food_in_sight_) {
    uint16_t dist = distance(position_,food_in_sight_pos_, testground_size_);
    if(dist < speed_){
      position_ = food_in_sight_pos_;
      energy_ -= pow(dist, 2);
    } else {
      direction_ = direction(position_, food_in_sight_pos_, testground_size_);
      position_ = move(position_, direction_, speed_, testground_size_);
      energy_ -= pow(speed_, 2);
    }
  } else {
    direction_ += ((rand() % 21) - 10) % 360;
    position_ = move(position_, direction_, speed_, testground_size_);
    energy_ -= pow(speed_, 2);
  }

  energy_ -= sight_;
  --reproduction_cooldown_;
}

Dot Dot::replicate(){
  uint16_t x_new = (position_.first + rand() % (sight_ * 2 + 1) - sight_) % testground_size_;
  uint16_t y_new = (position_.second + rand() % (sight_ * 2 + 1) - sight_) % testground_size_;
  Dot child(testground_size_, energy_/2, speed_ + rand() % 5 - 2, sight_ + rand() % 7 - 3, make_pair(x_new, y_new));
  energy_ /= 2;
  reproduction_cooldown_ = 20;
  return child;
}

void Dot::newFoodSource(std::pair<std::uint16_t, std::uint16_t> food_pos){
  food_in_sight_ = true;
  food_in_sight_pos_ = food_pos;
}

void Dot::eat(int amount){
  energy_ += amount;
  food_in_sight_ = false;
}

// Get()-Functions
int Dot::getEnergy(){
  return energy_;
}

std::pair<std::uint16_t, std::uint16_t> Dot::getPosition(){
  return position_;
}

int Dot::getReproductionCooldown(){
  return reproduction_cooldown_;
}

uint16_t Dot::getSpeed(){
  return speed_;
}

uint16_t Dot::getSight(){
  return sight_;
}
