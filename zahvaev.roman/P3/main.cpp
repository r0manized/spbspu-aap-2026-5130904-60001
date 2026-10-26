#include <algorithm>
#include <climits>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <fstream>
#include <iostream>
#include <new>
#include <stdexcept>

namespace zahvaev
{
  const std::size_t maxFixedSize = 10000;
  const std::size_t maxSize = static_cast< std::size_t >(-1);
  const int argsCount = 4;
  const int modeArg = 1;
  const int inputArg = 2;
  const int outputArg = 3;
  const int paramsError = 1;
  const int internalError = 2;

  static void *allocateZeroed(std::size_t count, std::size_t size)
  {
    void *const memory = std::calloc(count, size);
    if (memory == nullptr && count > 0)
    {
      throw std::bad_alloc();
    }
    return memory;
  }

  static void freeMatrix(int **matrix, std::size_t rows)
  {
    for (std::size_t i = 0; i < rows; ++i)
    {
      std::free(matrix[i]);
    }
    std::free(matrix);
  }

  static int **changeMatrix(const int *const *matrix, std::size_t rows, std::size_t cols)
  {
    int **const result = static_cast< int ** >(allocateZeroed(rows, sizeof(int *)));
    try
    {
      for (std::size_t i = 0; i < rows; ++i)
      {
        result[i] = static_cast< int * >(allocateZeroed(cols, sizeof(int)));
      }
      for (std::size_t i = 0; i < rows; ++i)
      {
        for (std::size_t j = 0; j < cols; ++j)
        {
          const std::size_t layer = std::min({i, j, rows - 1 - i, cols - 1 - j});
          const long long element = matrix[i][j];
          const long long offset = static_cast< long long >(layer) + 1;
          const long long value = element + offset;
          if (value > INT_MAX)
          {
            throw std::overflow_error("integer overflow");
          }
          result[i][j] = static_cast< int >(value);
        }
      }
    }
    catch (...)
    {
      freeMatrix(result, rows);
      throw;
    }
    return result;
  }

  static long long calcMaxDiagonalSum(const int *const *matrix, std::size_t rows, std::size_t cols)
  {
    if (rows == 0 || cols == 0)
    {
      return 0;
    }
    const std::size_t secondary = cols - 1;
    long long max_sum = 0;
    bool is_found = false;
    for (std::size_t diagonal = 0; diagonal < rows + cols - 1; ++diagonal)
    {
      if (diagonal == secondary)
      {
        continue;
      }
      long long sum = 0;
      for (std::size_t i = 0; i < rows; ++i)
      {
        for (std::size_t j = 0; j < cols; ++j)
        {
          if (i + j == diagonal)
          {
            sum += matrix[i][j];
          }
        }
      }
      if (!is_found || sum > max_sum)
      {
        max_sum = sum;
        is_found = true;
      }
    }
    return max_sum;
  }
}

int main(int argc, char *argv[])
{
  if (argc != zahvaev::argsCount)
  {
    std::cerr << "Error: wrong number of arguments\n";
    return zahvaev::paramsError;
  }
  const bool isFixed = (std::strcmp(argv[zahvaev::modeArg], "1") == 0);
  const bool isDynamic = (std::strcmp(argv[zahvaev::modeArg], "2") == 0);
  if (!isFixed && !isDynamic)
  {
    std::cerr << "Error: first parameter must be 1 or 2\n";
    return zahvaev::paramsError;
  }

  std::ifstream in(argv[zahvaev::inputArg]);
  std::ofstream out(argv[zahvaev::outputArg]);
  if (!in || !out)
  {
    std::cerr << "Error: cannot open file\n";
    return zahvaev::internalError;
  }

  long long raw_rows = 0;
  long long raw_cols = 0;
  if (!(in >> raw_rows >> raw_cols) || raw_rows < 0 || raw_cols < 0)
  {
    std::cerr << "Error: invalid matrix size\n";
    return zahvaev::internalError;
  }
  const std::size_t rows = static_cast< std::size_t >(raw_rows);
  const std::size_t cols = static_cast< std::size_t >(raw_cols);
  if (cols != 0 && rows > zahvaev::maxSize / cols)
  {
    std::cerr << "Error: matrix is too large\n";
    return zahvaev::internalError;
  }
  const std::size_t count = rows * cols;
  if (isFixed && (count > zahvaev::maxFixedSize || rows > zahvaev::maxFixedSize))
  {
    std::cerr << "Error: matrix does not fit into fixed array\n";
    return zahvaev::internalError;
  }

  int fixed_data[zahvaev::maxFixedSize] = {};
  int *fixed_rows[zahvaev::maxFixedSize] = {};
  int *dynamic_data = nullptr;
  int **dynamic_rows = nullptr;
  int *data = fixed_data;
  int **row_pointers = fixed_rows;
  int code = 0;

  try
  {
    if (isDynamic)
    {
      dynamic_data = static_cast< int * >(zahvaev::allocateZeroed(count, sizeof(int)));
      dynamic_rows = static_cast< int ** >(zahvaev::allocateZeroed(rows, sizeof(int *)));
      data = dynamic_data;
      row_pointers = dynamic_rows;
    }
    for (std::size_t i = 0; i < count; ++i)
    {
      if (!(in >> data[i]))
      {
        throw std::runtime_error("invalid matrix content");
      }
    }
    in >> std::ws;
    if (!in.eof())
    {
      throw std::runtime_error("invalid matrix content");
    }
    for (std::size_t i = 0; i < rows; ++i)
    {
      row_pointers[i] = data + i * cols;
    }

    int **const result = zahvaev::changeMatrix(row_pointers, rows, cols);
    out << rows << ' ' << cols;
    for (std::size_t i = 0; i < rows; ++i)
    {
      for (std::size_t j = 0; j < cols; ++j)
      {
        out << ' ' << result[i][j];
      }
    }
    out << '\n' << zahvaev::calcMaxDiagonalSum(row_pointers, rows, cols) << '\n';
    zahvaev::freeMatrix(result, rows);
    if (!out)
    {
      throw std::runtime_error("cannot write output file");
    }
  }
  catch (const std::exception &error)
  {
    std::cerr << "Error: " << error.what() << '\n';
    code = zahvaev::internalError;
  }

  std::free(dynamic_data);
  std::free(dynamic_rows);
  return code;
}
