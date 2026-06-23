#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <vector>
#include <iostream>
#include "polygon.hpp"

namespace velizade
{
  void area(std::istream& in, std::ostream& out, const std::vector<Polygon>& polygons);
  void max(std::istream& in, std::ostream& out, const std::vector<Polygon>& polygons);
  void min(std::istream& in, std::ostream& out, const std::vector<Polygon>& polygons);
  void count(std::istream& in, std::ostream& out, const std::vector<Polygon>& polygons);
  void rects(std::istream& in, std::ostream& out, const std::vector<Polygon>& polygons);
  void same(std::istream& in, std::ostream& out, const std::vector<Polygon>& polygons);
}

#endif
