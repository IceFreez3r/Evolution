// g++ -Wall -Werror -W -pedantic main.cpp environment.cpp dot.cpp util.cpp -o a
#include "environment.hpp"
#include "util.hpp"
#include <iostream>
using namespace std;

int main() {
  Dot d(1000, 10000, 10, 20, 100, make_pair(0,0));
  Environment e(1000, 1000, 1, 100, d);
  e.tick(1000);
  e.printMap();
  return 0;
}
