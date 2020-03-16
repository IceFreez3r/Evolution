// g++ -Wall -Werror -W -pedantic -std=c++17 main.cpp environment.cpp dot.cpp util.cpp -o a
#include "environment.hpp"
#include "util.hpp"
#include <iostream>
#include <string> // stoi
using namespace std;


// // Comparison of 2 identical simulations except for one is with 100 mutagens
// int main() {
//   Dot d(1000, 10000, 10, 20, 100, make_pair(0,0));
//   Environment e(1000, 10, 100, 100, d);
//   Environment e2(1000, 10, 100, 100, d);
//   e.printProperties();
//   e2.printProperties();
//   e.placeMutagen(100);
//   for (size_t i = 0; i < 10; ++i) {
//     e.tick(100);
//     e.printProperties();
//     e2.tick(100);
//     e2.printProperties();
//   }
//   e.printMap();
//   e2.printMap();
//   return 0;
// }

int main(int argc, char const *argv[]) {
  if (argc < 2) {
    cerr << "Bitte die Anzahl an Ticks mitangeben. \n(Es werden im Moment nur 100er Werte verwendet)\n";
    return 1;
  }
  Dot d(1000, 10000, 10, 20, 100, make_pair(0,0));
  Environment e(1000, 10, 100, 100, d);
  e.printProperties();
  for (int i = 0; i < stoi(argv[1])/100; ++i) {
    e.tick(100);
    e.printProperties();
  }
  return 0;
}
