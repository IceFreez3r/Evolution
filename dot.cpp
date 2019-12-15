#include <iostream>
#include <math.h>

#include "dot.hpp"

#include "util.hpp"

using namespace std;

Dot::Dot(const uint16_t testground_size):
  speed_(10),
  sight_(50)
{
  energy = 1000;
  reproduction_cooldown = 20;
  testground_size_ = testground_size;
  uint16_t x = rand() % testground_size_;
  uint16_t y = rand() % testground_size_;
  position = make_pair(x, y);
  direction_ = rand() % 360;
  food_in_sight_ = false;
  food_in_sight_pos_ = make_pair(0,0);
}

Dot::Dot(const uint16_t testground_size, const int energy, const uint16_t speed, const uint16_t sight, const pair<uint16_t, uint16_t> pos):
  speed_(speed),
  sight_(sight)
{
  this->energy = energy;
  reproduction_cooldown = 20;
  testground_size_ = testground_size;
  position = pos;
  direction_ = rand() % 360;
  food_in_sight_ = false;
  food_in_sight_pos_ = make_pair(0,0);
}

Dot::operator=(const Dot &d):
  speed_(d.speed_),
  sight_(d.sight_)
{
  energy = d.energy;
  position = d.position;
  reproduction_cooldown = d.reproduction_cooldown;
  direction_ = d.direction_;
  testground_size_ = d.testground_size_;
  food_in_sight_ = d.food_in_sight_;
  food_in_sight_pos_ = d.food_in_sight_pos_;
}

void Dot::tick(){
  if (food_in_sight_) {
    uint16_t dist = distance(position,food_in_sight_pos_);
    if(dist < speed_){
      position = food_in_sight_pos_;
      energy -= pow(dist, 2);
    } else {
      position = move(position, direction_, speed_, testground_size_);
      energy -= pow(speed_, 2);
    }
  } else {
    direction_ += ((rand() % 21) - 10) % 360;
    position = move(position, direction_, speed_, testground_size_);
    energy -= pow(speed_, 2);
  }

  energy -= sight_;
  --reproduction_cooldown;
}

Dot Dot::replicate(){
  uint16_t x_new = (position.first + rand() % (sight_ * 2 + 1) - sight_) % testground_size_;
  uint16_t y_new = (position.second + rand() % (sight_ * 2 + 1) - sight_) % testground_size_;
  Dot child(testground_size_, energy/2, speed_ + rand() % 5 - 2, sight_ + rand() % 7 - 3, make_pair(x_new, y_new));
  energy /= 2;
  reproduction_cooldown = 20;
  return child;
}

void Dot::eat(int amount){
  energy += amount;
}
