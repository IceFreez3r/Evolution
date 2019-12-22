#include <utility>
#include <cstdint>

std::uint16_t distance(const std::pair<std::uint16_t, std::uint16_t> pos, const std::pair<std::uint16_t, std::uint16_t> pos2);

std::uint16_t direction(std::pair<std::uint16_t, std::uint16_t> start_pos, std::pair<std::uint16_t, std::uint16_t> target_pos);

std::pair<std::uint16_t, std::uint16_t> move(const std::pair<std::uint16_t, std::uint16_t> pos, std::uint16_t direction, const std::uint16_t step_size, const std::uint16_t testground_size);
