class Dot {
public:
  // Initial constructor
  Dot(const unsigned int testground_size);
  // Replication constructor
  Dot(const unsigned int testground_size, const int energy, const unsigned int speed, const unsigned int sight, const unsigned int x_pos, const unsigned int y_pos);
  // If the dot targets a food source, it moves toward the food
  // Otherwise the dot changes its direction a bit and moves in that direction
  // The distance is dependent of its speed
  // The dot looses energy equal to the square of its speed + its sight
  void tick();
  // the dot can replicate, if it has enough energy and its reproduction cooldown is 0
  // this creates another dot in sight of this dot
  // the properties of the new dot are the same as the ones of the original dot with some mutations
  void replicate();

  void eat(int amount);

private:
  // constant properties
  const unsigned int speed_; // distance the dot can travel in one tick
  const unsigned int sight_; // distance in which the dot can see food and other dots
  // dynamic properties
  int energy_; // energy the dot, the dot dies when it reaches 0
  int reproduction_cooldown_; // time until the dot can replicate again
  pair<unsigned int, unsigned int> position_; // position on the Testground
  unsigned int direction_; // direction of movement, (0 top, 90 right, 180 bottom, 270 left)
  bool food_in_sight_; // True, if a food source in in sight
  pair<unsigned int, unsigned int> food_in_sight_pos_; // position of the food source in sight
};
