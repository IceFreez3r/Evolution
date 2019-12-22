#include <utility>
#include <cstdint>

/* All functions take into account that the shortest way may go over the side of the Testground:
______________
|            |
|->x      .--|
|            |
|            |
|            |
______________
*/

// Calculates the distance of two coordinates
uint16_t distance(const std::pair<uint16_t, uint16_t> pos, const std::pair<uint16_t, uint16_t> pos2, const uint16_t testground_size);

// Calculates the direction for the shortest way from one coordinate to another
uint16_t direction(const std::pair<uint16_t, uint16_t> start_pos, const std::pair<uint16_t, uint16_t> target_pos, const uint16_t testground_size);

// Calculates a new coordinates, given a startposition, direction and stepsize
std::pair<uint16_t, uint16_t> move(const std::pair<uint16_t, uint16_t> pos, int16_t direction, const uint16_t step_size, const uint16_t testground_size);

// Returns -1 for x < 0, 0 for x = 0, 1 for x > 0
int signum(float val);
// template <typename T> int signum(T val);
