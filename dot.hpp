#include <cstdint>

class Dot {
public:
  Dot() = default;
  // Initial constructor
  Dot(const uint16_t testground_size);
  // Replication constructor
  Dot(const uint16_t testground_size, const int energy, const uint16_t speed, const uint16_t sight, const uint16_t size, const std::pair<uint16_t, uint16_t> pos);
  // Copy constructor
  Dot(const Dot &d, bool exact_copy = true);
  // ~Dot(); // Neccessary?
  // If the dot targets a food source, it moves toward the food
  // Otherwise the dot changes its direction a bit and moves in that direction
  // The distance is dependent of its speed
  // The dot looses energy equal to the square of its speed + its sight
  void tick();
  // the dot can replicate, if it has enough energy and its reproduction cooldown is 0
  // this creates another dot in sight of this dot
  // the properties of the new dot are the same as the ones of the original dot with some mutations
  // maximum change is dependent on the old value multiplied with the mutation_rate
  Dot replicate(const double mutation_rate);

  // Gets triggered by environment if a food might be in sight
  // updates food_in_sight_, food_in_sight_idx_, food_in_sight_pos_, food_in_sight_dist_
  void newFoodSource(std::pair<uint16_t, uint16_t> food_pos);

  //Gets triggered by environment if a bigger enemy is in sight
  void newHazardSource(std::pair<uint16_t, uint16_t> hazard_pos);

  // Gets triggered by the environment if the dot reaches a food source
  void eat(int amount);

  // Get- and Set-Functions
  int getEnergy() const;
  std::pair<std::uint16_t, std::uint16_t> getPosition() const;
  int getReproductionCooldown() const;
  uint16_t getSpeed() const;
  uint16_t getSight() const;
  uint16_t getSize() const;
  uint16_t getTestgroundSize() const;
  void setTestgroundSize(uint16_t testground_size);
  bool getFoodInSight() const;
  void setFoodInSight(bool food_in_sight);
  std::size_t getFoodInSightIdx() const;

private:
  // Constant properties
  uint16_t speed_; // distance the dot can travel in one tick
  uint16_t sight_; // distance in which the dot can see food and other dots
  uint16_t size_; // size of dots

  // Dynamic properties
  int energy_; // energy the dot, the dot dies when it reaches 0
  int reproduction_cooldown_; // time until the dot can replicate again
  uint16_t testground_size_;
  bool food_in_sight_; // True, if a food source is in sight

  std::size_t food_in_sight_idx_; // index of the food source in sight
  std::pair<std::uint16_t, std::uint16_t> food_in_sight_pos_; // position of the food source in sight
  uint16_t food_in_sight_dist_;
  bool hazard_in_sight_; // True, if a food source is in sight
  std::pair<uint16_t, uint16_t> hazard_in_sight_pos_; // position of the hazard source in sight

  std::pair<uint16_t, uint16_t> position_; // position on the Testground
  int16_t direction_; // direction of movement, (0 right, 90 up, 180 left, -90 down)
};
