#include <vector>
#include <cstdint>

#include "dot.hpp"

class Environment {
public:
  // Standard Constructor
  Environment();
  // Custom Constructor
  Environment(const std::uint16_t testground_size, const int dot_count, const int min_food_count, const int max_food_count);
   // Triggers tick() function of every dot and runs feeding(...)
  void tick();
  // Runs tick() multiple times
  void tick(const int amount);
  // Places new dots on the Testground with standard properties
  void contamination(const int amount);
  // Places new Foodsources at random locations on the Testground
  void feeding(const int min_amount, const int max_amount);
  void feeding(const int max_amount);
  // "Tells" a Dot if it can see food
  void searchFood(Dot d);
  // Prints the Positions of dots and food scaled to a 100x100 matrix
  void printTestground();
private:
  const std::uint16_t size_; // Size of the Testground
  std::vector<std::pair<std::uint16_t, std::uint16_t>> food_; //Positions of Food
  std::vector<Dot> dots_; //Vector of all dots
  uint32_t tick_;
};
