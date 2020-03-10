// g++ -Wall -Werror -W -pedantic -std=c++17 main.cpp environment.cpp dot.cpp util.cpp -o a
#include "environment.hpp"
#include "util.hpp"
#include <iostream>
using namespace std;

int main() {
  Dot d(1000, 10000, 10, 20, 100, make_pair(0,0));
  Environment e(1000, 10, 100, 100, d);
  Environment e2(1000, 10, 100, 100, d);
  e.printProperties();
  e2.printProperties();
  e.placeMutagen(100);
  for (size_t i = 0; i < 10; ++i) {
    e.tick(100);
    e.printProperties();
    e2.tick(100);
    e2.printProperties();
  }
  e.printMap();
  e2.printMap();
  return 0;
}
