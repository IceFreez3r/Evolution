#include <iostream>

#include "util.cpp"

using namespace std;

Dot::Dot(const unsigned int testground_size):(
    speed_(10);
    sight_(50);
  ){
    energy_ = 1000;
    reproduction_cooldown_ = 10;
    unsigned int x = rand() % testground_size;
    unsigned int y = rand() % testground_size;
    position_ = make_pair(x, y);
    direction_ = rand() % 360;
    food_in_sight_ = false;
    food_in_sight_pos_ = make_pair(0,0);
}

Dot::Dot(const unsigned int testground_size, const int energy, const int speed, const int sight, const unsigned int x_pos, const unsigned int y_pos):(
    speed_(speed);
    sight_(sight);
  ){
    energy_ = energy;
    reproduction_cooldown_ = 10;
    position_ = make_pair(x_pos, y_pos);
    direction_ = rand() % 360;
    food_in_sight_ = false;
    food_in_sight_pos_ = make_pair(0,0);
}

//Evtl Pointer auf neuen Dot, falls replikation, sonst auf sich selbst?
void Dot::tick(){
  if (food_in_sight_) {
    unsigned int dist = distance(position_,food_in_sight_pos_) < speed_
    if(dist < speed){
      position_ = food_in_sight_pos_;
      energy_ -= pow(dist, 2);
    } else {
      position_ = move(position_, direction_/*xxx*/, speed);
      energy_ -= pow(speed, 2);
    }
  } else {
    direction_ += (rand() % 21) - 10;
    position_ = move(position_, direction_, speed);
    energy_ -= pow(speed, 2);
  }

  energy_ -= sight_;

//Funktioniert so nicht: child stirbt bei nächster "}"
  --reproduction_cooldown_;
  if(reproduction_cooldown_ <= 0 && energy_ > 5000/*xxx*/){
    Dot child(65535, energy_/2, speed_ + rand() % 5 - 2, sight_ + rand() % 7 - 3, position_.first + rand() % (sight_ * 2 + 1) - sight_, position_.second + rand() % (sight_ * 2 + 1) - sight_)
    energy_ /= 2;
    reproduction_cooldown_ = 20;
  }
}
