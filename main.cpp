#include "environment.hpp"

using namespace std;

int main(int argc, char const *argv[]) {
  Dot d(1000, 10000, 20, 10, 100, make_pair(0,0));
  Environment e(100, 1, 1, 10, d);
  for (int i = 0; i < 10; ++i) {
    e.tick(10);
    e.printProperties();
    // e.printTestground();

  }
  return 0;
}
