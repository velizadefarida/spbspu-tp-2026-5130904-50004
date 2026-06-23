#include "dataStruct.hpp"
#include "utils.hpp"
#include <vector>

bool velizade::operator<(const velizade::DataStruct& lhs, const velizade::DataStruct& rhs)
{
  if (!(lhs.key1 == rhs.key1))
  {
    return lhs.key1 < rhs.key1;
  }
  if (!(lhs.key2 == rhs.key2))
  {
    return lhs.key2 < rhs.key2;
  }
  return lhs.key3.size() < rhs.key3.size();
}

std::ostream& velizade::operator<<(std::ostream& out, const velizade::DataStruct& ds)
{
  velizade::StreamGuard guard(out);
  out << "(:key1 " << ds.key1
      << ":key2 " << ds.key2
      << ":key3 \"" << ds.key3 << "\":)";
  return out;
}

std::istream& velizade::operator>>(std::istream& in, velizade::DataStruct& ds)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  velizade::StreamGuard guard(in);
  velizade::DataStruct temp;
  bool hasKey1 = false;
  bool hasKey2 = false;
  bool hasKey3 = false;

  if (!velizade::readExpectedChar(in, '('))
  {
    return in;
  }
  if (!velizade::readExpectedChar(in, ':'))
  {
    return in;
  }

  for (int i = 0; i < 3; ++i)
  {
    std::string key;
    in >> key;
    if (!in)
    {
      return in;
    }

    if (key == "key1")
    {
      if (hasKey1)
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      in >> temp.key1;
      hasKey1 = true;
    }
    else if (key == "key2")
    {
      if (hasKey2)
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      in >> temp.key2;
      hasKey2 = true;
    }
    else if (key == "key3")
    {
      if (hasKey3)
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      char quote;
      if (!(in >> quote) || quote != '"')
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      std::getline(in, temp.key3, '"');
      hasKey3 = true;
    }
    else
    {
      in.setstate(std::ios::failbit);
      return in;
    }

    if (!velizade::readExpectedChar(in, ':'))
    {
      return in;
    }
  }

  if (!velizade::readExpectedChar(in, ')'))
  {
    return in;
  }

  if (hasKey1 && hasKey2 && hasKey3)
  {
    ds = temp;
  }
  else
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}
