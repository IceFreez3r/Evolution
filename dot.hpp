#include <cstdint>

class Dot {
public:
  // Initial constructor
  Dot(const std::uint16_t testground_size);
  // Replication constructor
  Dot(const std::uint16_t testground_size, const int energy, const std::uint16_t speed, const std::uint16_t sight, const std::pair<std::uint16_t, std::uint16_t> pos);

  // If the dot targets a food source, it moves toward the food
  // Otherwise the dot changes its direction a bit and moves in that direction
  // The distance is dependent of its speed
  // The dot looses energy equal to the square of its speed + its sight
  void tick();
  // the dot can replicate, if it has enough energy and its reproduction cooldown is 0
  // this creates another dot in sight of this dot
  // the properties of the new dot are the same as the ones of the original dot with some mutations
  Dot replicate();

  // Gets triggered by the environment if the dot reached a food source
  void eat(int amount);

  std::pair<std::uint16_t, std::uint16_t> getPos();
private:
  // constant properties
  const std::uint16_t speed_; // distance the dot can travel in one tick
  const std::uint16_t sight_; // distance in which the dot can see food and other dots
  // dynamic properties
  int energy_; // energy the dot, the dot dies when it reaches 0
  int reproduction_cooldown_; // time until the dot can replicate again

  std::pair<std::uint16_t, std::uint16_t> position_; // position on the Testground
  std::uint16_t direction_; // direction of movement, (0 right, 90 up, 180 left, 270 down)
  std::uint16_t testground_size_;
  bool food_in_sight_; // True, if a food source in in sight
  std::pair<std::uint16_t, std::uint16_t> food_in_sight_pos_; // position of the food source in sight
};
