#ifndef UTILS_HPP
#define UTILS_HPP

#include <ios>
#include <iostream>

namespace velizade
{
  class StreamGuard
  {
  public:
    explicit StreamGuard(std::ios& stream);
    ~StreamGuard();

  private:
    std::ios& stream_;
    std::streamsize precision_;
    std::streamsize width_;
    std::ios::fmtflags flags_;
    char fill_;
  };

  struct DecLiteral
  {
    unsigned long long value;
  };

  std::istream& operator>>(std::istream& in, DecLiteral& num);
  std::ostream& operator<<(std::ostream& out, const DecLiteral& num);
  bool operator<(const DecLiteral& a, const DecLiteral& b);
  bool operator==(const DecLiteral& a, const DecLiteral& b);

  struct HexLiteral
  {
    unsigned long long value;
  };

  std::istream& operator>>(std::istream& in, HexLiteral& num);
  std::ostream& operator<<(std::ostream& out, const HexLiteral& num);
  bool operator<(const HexLiteral& a, const HexLiteral& b);
  bool operator==(const HexLiteral& a, const HexLiteral& b);

  bool readExpectedChar(std::istream& in, char expected);
}

#endif
