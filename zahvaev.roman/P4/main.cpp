#include <iostream>
#include <cstring>
#include <cctype>
#include <new>

namespace zahvaev {

  void removeLatin(char* str_2, const char* str, size_t length)
  {
    size_t length_2 = 0;
    for (size_t i = 0; i < length; ++i) {
      if (std::isalpha(static_cast< unsigned char >(str[i]))
          && ((str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= 'a' && str[i] <= 'z'))) {
        continue;
      } else {
        str_2[length_2] = str[i];
        ++length_2;
      }
    }
    str_2[length_2] = '\0';
    for (size_t i = 0; i < length_2; ++i) {
      std::cout << str_2[i];
    }
    std::cout << '\n';
  }

  void sumLatin(char* str_3, char* str, const char* str_4, size_t length)
  {
    size_t length_3 = 0;
    for (size_t i = 0; i < length; ++i) {
      if (std::isupper(static_cast< unsigned char >(str[i]))) {
        str[i] = std::tolower(static_cast< unsigned char >(str[i]));
      }
    }
    char symbol = 'a';

    for (size_t count = 0; symbol <= 'z'; ++symbol) {
      for (size_t i = 0; i < length; ++i) {
        if (symbol == str[i]) {
          ++count;
        }
      }
      for (size_t i = 0; i < std::strlen(str_4); ++i) {
        if (symbol == std::tolower(static_cast< unsigned char >(str_4[i]))) {
          ++count;
        }
      }
      if (count != 0) {
        str_3[length_3] = symbol;
        ++length_3;
      }
      count = 0;
    }
    str_3[length_3] = '\0';
    for (size_t i = 0; i < length_3; ++i) {
      std::cout << str_3[i];
    }
    std::cout << '\n';
  }
}

int main()
{
  size_t capacity = 4;
  size_t length = 0;
  char* str = nullptr;
  try {
    str = new char[capacity];
  } catch (const std::bad_alloc&) {
    std::cerr << "Out of memory\n";
    return 1;
  }
  char symbol = '\0';

  while (std::cin.get(symbol) && symbol != '\n') {
    if (length == capacity - 1) {
      size_t& capacity_ref = capacity;
      capacity_ref *= 2;
      char* new_str = nullptr;
      try {
        new_str = new char[capacity_ref];
      } catch (const std::bad_alloc&) {
        delete[] str;
        std::cerr << "Out of memory\n";
        return 1;
      }
      for (size_t i = 0; i < length; ++i) {
        new_str[i] = str[i];
      }
      delete[] str;
      str = new_str;
    }
    str[length] = symbol;
    ++length;
  }
  str[length] = '\0';
  if (length == 0) {
    delete[] str;
    std::cerr << "Empty input\n";
    return 1;
  }
  char* str_2 = nullptr;
  try {
    str_2 = new char[capacity];
  } catch (const std::bad_alloc&) {
    delete[] str;
    std::cerr << "Out of memory\n";
    return 1;
  }
  const size_t str_3_size = 27;
  char* str_3 = nullptr;
  try {
    str_3 = new char[str_3_size];
  } catch (const std::bad_alloc&) {
    delete[] str;
    delete[] str_2;
    std::cerr << "Out of memory\n";
    return 1;
  }
  const char* const str_4 = "def_ghk";
  zahvaev::removeLatin(str_2, str, length);
  zahvaev::sumLatin(str_3, str, str_4, length);
  delete[] str;
  delete[] str_2;
  delete[] str_3;
  str = nullptr;
  str_2 = nullptr;
  str_3 = nullptr;
  return 0;
}
