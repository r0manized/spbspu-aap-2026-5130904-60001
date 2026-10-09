#include <iostream>
int main()
{
  long long a = 0, b = 0, c = 0;
  long long max_value = 0;
  int count1 = 0, count2 = 0, value = 0;
  while (true) {
    std::cin >> c;
    if (std::cin.fail()) {
      std::cerr << "Input error\n";
      return 1;
    }
    if (c == 0) {
      break;
    }
    value++;
    if (value == 1 || c > max_value) {
      max_value = c;
      count2 = 0;
    } else if (c == max_value) {
      count2 = 0;
    } else {
      count2++;
    }
    if (value == 1) {
      a = c;
      continue;
    }
    if (value == 2) {
      b = c;
      continue;
    }
    if (a > 0 && b > 0 && c > 0 && a * a + b * b == c * c) {
      count1++;
    }
    a = b;
    b = c;
  }
  if (value < 3) {
    std::cerr << "Sequence is too short\n";
    return 2;
  }
  std::cout << count1 << "\n";
  std::cout << count2 << "\n";
  return 0;
}
