
#include "environment.hpp"
#include <vector>
#include <iostream>

using namespace std;

extern double search_time;

int main() {
  std::vector<double> times(0);
  while (times.size() < 10) {
    Dot d(1000, 10000, 10, 20, 10, make_pair(0,0));
    Environment e(100, 10, 1, 10, d);
    for (int j = 0; j < 10; ++j) {
      e.printProperties();
      e.tick(100);
    }
    if (e.printProperties() > 0) {
      times.push_back(search_time);
    }
    search_time = 0.0;
  }
  for (size_t i = 0; i < times.size(); ++i) {
    cout << "\n" << times[i];
  }
  return 0;
}
