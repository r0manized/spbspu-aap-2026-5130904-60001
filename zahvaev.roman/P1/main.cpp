#include <cstddef>
#include <iostream>
#include <limits>

int main()
{
  constexpr long long max_limit = std::numeric_limits< long long >::max();

  long long prev_prev = 0;
  long long prev = 0;
  long long current = 0;
  long long max_value = std::numeric_limits< long long >::min();

  std::size_t triples_count = 0;
  std::size_t elements_after_max = 0;
  std::size_t numbers_count = 0;

  while (std::cin >> current && current != 0) {
    ++numbers_count;

    if (current >= max_value) {
      max_value = current;
      elements_after_max = 0;
    } else {
      ++elements_after_max;
    }

    if (prev_prev > 0 && prev > 0 && prev_prev < current && prev < current && current <= max_limit / current) {
      if (current * current - prev_prev * prev_prev == prev * prev) {
        ++triples_count;
      }
    }

    prev_prev = prev;
    prev = current;
  }

  if (std::cin.fail() && !std::cin.eof()) {
    std::cerr << "Input error\n";
    return 1;
  }

  if (numbers_count < 3) {
    std::cerr << "Sequence is too short\n";
    return 2;
  }

  std::cout << triples_count << "\n";
  std::cout << elements_after_max << "\n";

  return 0;
}
