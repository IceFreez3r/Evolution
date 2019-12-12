#include <iostream>
#include <math.h>

#include "dot.hpp"

#include "util.hpp"

using namespace std;

const unsigned int testground_size = 65535;

Dot::Dot():
  speed_(10),
  sight_(50)
{
  energy_ = 1000;
  reproduction_cooldown_ = 20;
  unsigned int x = rand() % testground_size;
  unsigned int y = rand() % testground_size;
  position_ = make_pair(x, y);
  direction_ = rand() % 360;
  food_in_sight_ = false;
  food_in_sight_pos_ = make_pair(0,0);
}

Dot::Dot(int energy, const unsigned int speed, const unsigned int sight, const pair<unsigned int, unsigned int> pos):
  speed_(speed),
  sight_(sight)
{
  energy_ = energy;
  reproduction_cooldown_ = 20;
  position_ = pos;
  direction_ = rand() % 360;
  food_in_sight_ = false;
  food_in_sight_pos_ = make_pair(0,0);
}

void Dot::tick(){
  if (food_in_sight_) {
    unsigned int dist = distance(position_,food_in_sight_pos_);
    if(dist < speed_){
      position_ = food_in_sight_pos_;
      energy_ -= pow(dist, 2);
    } else {
      position_ = move(position_, direction_/*xxx*/, speed_);
      energy_ -= pow(speed_, 2);
    }
  } else {
    direction_ += (rand() % 21) - 10;
    position_ = move(position_, direction_, speed_);
    energy_ -= pow(speed_, 2);
  }

  energy_ -= sight_;
  --reproduction_cooldown_;
}

Dot Dot::replicate(){
  unsigned int x_new = position_.first + rand() % (sight_ * 2 + 1) - sight_;
  unsigned int y_new = position_.second + rand() % (sight_ * 2 + 1) - sight_;
  Dot child(energy_/2, speed_ + rand() % 5 - 2, sight_ + rand() % 7 - 3, make_pair(x_new, y_new));
  energy_ /= 2;
  reproduction_cooldown_ = 20;
  return child;
}

void Dot::eat(int amount){
  energy_ += amount;
}
