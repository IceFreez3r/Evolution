// g++ -Wall -Werror -W -pedantic -std=c++17 main.cpp environment.cpp dot.cpp util.cpp -o a
#include "environment.hpp"
#include "util.hpp"
#include <iostream>
#include <string> // stoi
#include <fstream> //file

using namespace std;


// // Comparison of 2 identical simulations except for one is with 100 mutagens
// int main() {
//   Dot d(1000, 10000, 10, 20, 100, make_pair(0,0));
//   Environment e(1000, 10, 100, 100, d);
//   Environment e2(1000, 10, 100, 100, d);
//   // e.printProperties();
//   // e2.printProperties();
//   e.placeMutagen(100);
//   for (size_t i = 0; i < 10; ++i) {
//     e.tick(100);
//     e2.tick(100);
//     // e.printProperties();
//     // e2.printProperties();
//   }
//   e.printMap();
//   e2.printMap();
//   return 0;
// }

// int main(int argc, char const *argv[]) {
//   uint16_t ticks_at_once = 100;
//   if (argc < 2) {
//     cerr << "Bitte die Anzahl an Ticks mitangeben. \n(Es wird im Moment auf den nächsten " << ticks_at_once << "er Wert abgerundet)\n";
//     return 1;
//   }
//   Dot d(1000, 10000, 10, 20, 100, make_pair(0,0));
//   //Dot(const uint16_t testground_size, const int energy, const uint16_t speed, const uint16_t sight, const uint16_t size, const std::pair<uint16_t, uint16_t> pos);
//   Environment e(1000, 10, 100, 100, d);
//   //Environment(const std::uint16_t testground_size, const int dot_count, const int min_food_count, const int max_food_count, const Dot start_dot);
//   e.printProperties();
//   for (int i = 0; i < stoi(argv[1])/ticks_at_once; ++i) {
//     e.tick(ticks_at_once);
//     e.printProperties();
//     // e.printMap();
//   }
//   return 0;
// }
int main(){
  std::ofstream file;
  file.open("evo_test.txt");
  file << "Mal was hineingeschrieben!" << "\n";
  file << "Noch etwas hinzugefügt";
  file << " Ende;" << "Tschau";
  file.close();
}
