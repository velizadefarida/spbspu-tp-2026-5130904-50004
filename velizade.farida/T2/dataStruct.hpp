#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP

#include <iostream>
#include <string>
#include "utils.hpp"

namespace velizade
{
  struct DataStruct
  {
    DecLiteral key1;
    HexLiteral key2;
    std::string key3;
  };

  std::istream& operator>>(std::istream& in, DataStruct& ds);
  std::ostream& operator<<(std::ostream& out, const DataStruct& ds);
  bool operator<(const DataStruct& lhs, const DataStruct& rhs);
}

#endif
