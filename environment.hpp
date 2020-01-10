#include <vector>
#include <cstdint>

#include "dot.hpp"

class Environment {
public:
  // Standard Constructor
  Environment();
  // Custom Constructor
  Environment(const std::uint16_t testground_size, const int dot_count, const int min_food_count, const int max_food_count);
  Environment(const std::uint16_t testground_size, const int dot_count, const int min_food_count, const int max_food_count, const Dot start_dot);
   // Triggers tick() function of every dot and runs feeding(...), if amount is given the function is repeated multiple times
  void tick(const int amount = 1);
  // Places new dots on the Testground with standard properties
  void contamination(const int amount);
  // Places new Foodsources at random locations on the Testground
  void feeding(const int min_amount, const int max_amount); // (?) können wir zusammenschrumpfen, dann brauchen wir nicht 2x den gleichen code.
  void feeding(const int food_count);
  // "Tells" a Dot if it can see food
  void searchFood();
  // Prints the Positions of dots and food scaled to a 100x100 matrix
  void printMap();
  void printTestground();
  void printProperties();
private:
  const std::uint16_t testground_size_; // Size of the Testground
  std::vector<std::pair<std::uint16_t, std::uint16_t>> food_; //Positions of Food
  const int min_food_per_tick_;
  const int max_food_per_tick_;
  Dot start_dot_;
  std::vector<Dot> dots_; //Vector of all dots
  uint32_t tick_;
};
