#include <vector>
class Environment {
public:
  Environment();
  void tick(); // Generates new Food and triggers tick() function of every dot.
  void printFood(); // Prints the Positions of the food as a dotplot
private:
  const unsigned int size_; // Size of the Testground
  std::vector<std::pair<unsigned int, unsigned int>> food_; //Positions of Food
  std::vector<dot> dots_; //Vector of all dots
};
