#include <vector>
#include <cstdint>

#include "dot.hpp"

class Environment {
public:
  // Standard Constructor
  // generates default environment
  Environment();
  // Custom Constructor
  // generates environment with given values, with generic start Dots
  Environment(const std::uint16_t testground_size, const int dot_count, const int min_food_count, const int max_food_count);

  // generates environment with given values and start Dots
  Environment(const std::uint16_t testground_size, const int dot_count, const int min_food_count, const int max_food_count, const Dot start_dot);
  // creates an new mutagen or changes the position of the old one
  void placeMutagen(std::vector<std::pair<std::uint16_t, std::uint16_t> > positions);
  void placeMutagen(uint16_t amount = 1);
  void placeMutagen(std::pair<std::uint16_t, std::uint16_t> pos);
  void clearMutagen();
  // Triggers tick() function of every Dot and runs feeding(...), if amount is given the function is repeated multiple times
  void tick(const int amount = 1);
  // Places new Dots on the Testground with standard properties
  void contamination(const int amount);
  // Changes the amount of food generated in each tick
  void changeFoodPerTick(const int min_amount, const int max_amount);
  // Places new Foodsources at random locations on the Testground
  // If only min_amount is given or min_amount is greater than max_amount,
  // exactly min_amount Foodsources are generated
  // If min and max_amount are given, the function generates a random number
  // between them and places that many Foodsources
  void feeding();
  // Prints the Positions of Dots and food scaled to a 100x100 matrix
  void printMap();
  void printTestground();
  void printProperties();
private:
  // "Tells" a Dot if it can see food
  void searchFood();
  const std::uint16_t testground_size_; // Size of the Testground
  std::vector<std::pair<std::uint16_t, std::uint16_t> > food_vec_; //Positions of Food
  int min_food_per_tick_;
  int max_food_per_tick_;
  Dot start_dot_;
  std::vector<Dot> dots_vec_; //Vector of all Dots
  uint32_t tick_;
  std::vector<std::pair<std::uint16_t, std::uint16_t> >  mutagen_vec_; //Positions of Mutagen
};
