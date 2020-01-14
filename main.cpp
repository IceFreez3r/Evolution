
#include "environment.hpp"
#include "util.hpp"
#include <iostream>
using namespace std;

int main(int argc, char const *argv[]) {
  Dot d(1000, 10000, 10, 20, 100, make_pair(0,0));
  Environment e(100, 10, 1, 10, d);
  for (int i = 0; i < 10; ++i) {
    e.tick(10);
    e.printProperties();
    // e.printTestground();
  }
  return 0;
}
