#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <limits>
#include "dataStruct.hpp"

int main()
{
  std::vector<velizade::DataStruct> data;
  while (!std::cin.eof())
  {
    velizade::DataStruct record;
    if (std::cin >> record)
    {
      data.push_back(record);
    }
    else
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }

  std::sort(data.begin(), data.end());
  std::copy(data.begin(), data.end(), std::ostream_iterator<velizade::DataStruct>(std::cout, "\n"));

  return 0;
}
