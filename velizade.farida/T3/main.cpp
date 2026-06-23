#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <functional>
#include <limits>
#include "polygon.hpp"
#include "commands.hpp"

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " <file>\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file)
  {
    std::cerr << "Cannot open file\n";
    return 1;
  }

  std::vector<velizade::Polygon> polygons;
  velizade::readData(file, polygons);

  std::map<std::string, std::function<void()>> commands;
  commands["AREA"] = std::bind(velizade::area, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["MAX"]   = std::bind(velizade::max,   std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["MIN"]   = std::bind(velizade::min,   std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["COUNT"] = std::bind(velizade::count, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["RECTS"] = std::bind(velizade::rects, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["SAME"]  = std::bind(velizade::same,  std::ref(std::cin), std::ref(std::cout), std::cref(polygons));

  std::string command;
  while (std::cin >> command)
  {
    try
    {
      commands.at(command)();
    }
    catch (...)
    {
      if (std::cin.fail())
      {
        std::cin.clear();
      }
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  return 0;
}
