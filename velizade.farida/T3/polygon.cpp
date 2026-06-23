#include "polygon.hpp"
#include "utils.hpp"
#include <algorithm>
#include <numeric>
#include <cmath>
#include <set>
#include <limits>
#include <array>
#include <iterator>

struct CrossSum
{
  const velizade::Polygon& p;
  double operator()(double sum, size_t i) const
  {
    const velizade::Point& a = p.points[i];
    const velizade::Point& b = p.points[(i + 1) % p.points.size()];
    return sum + static_cast<double>(a.x * b.y - a.y * b.x);
  }
};

struct LessX
{
  bool operator()(const velizade::Point& a, const velizade::Point& b) const
  {
    return a.x < b.x;
  }
};

struct LessY
{
  bool operator()(const velizade::Point& a, const velizade::Point& b) const
  {
    return a.y < b.y;
  }
};

struct ShiftChecker
{
  const std::set<velizade::Point>& setB;
  int dx, dy;
  bool operator()(const velizade::Point& ap) const
  {
    velizade::Point shifted{ap.x + dx, ap.y + dy};
    return setB.find(shifted) != setB.end();
  }
};

struct FindShiftInB
{
  const velizade::Polygon& a;
  const std::set<velizade::Point>& setB;
  const velizade::Point& a0;
  bool operator()(const velizade::Point& bp) const
  {
    int dx = bp.x - a0.x;
    int dy = bp.y - a0.y;
    ShiftChecker checker{setB, dx, dy};
    return std::all_of(a.points.begin(), a.points.end(), checker);
  }
};

struct CheckRightAngle
{
  const velizade::Polygon& p;
  bool operator()(size_t i) const
  {
    const velizade::Point& a = p.points[i];
    const velizade::Point& b = p.points[(i + 1) % 4];
    const velizade::Point& c = p.points[(i + 2) % 4];
    int dx1 = b.x - a.x;
    int dy1 = b.y - a.y;
    int dx2 = c.x - b.x;
    int dy2 = c.y - b.y;
    return (dx1 * dx2 + dy1 * dy2) == 0;
  }
};

bool velizade::operator==(const Point& a, const Point& b)
{
  return a.x == b.x && a.y == b.y;
}

bool velizade::operator<(const Point& a, const Point& b)
{
  return a.x < b.x || (a.x == b.x && a.y < b.y);
}

bool velizade::operator==(const Polygon& a, const Polygon& b)
{
  return a.points.size() == b.points.size() &&
         std::equal(a.points.begin(), a.points.end(), b.points.begin());
}

bool velizade::operator!=(const Polygon& a, const Polygon& b)
{
  return !(a == b);
}

std::istream& velizade::operator>>(std::istream& in, Point& p)
{
  StreamGuard guard(in);
  char c1, c2, c3;
  if (!(in >> c1) || c1 != '(')
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  if (!(in >> p.x))
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  if (!(in >> c2) || c2 != ';')
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  if (!(in >> p.y))
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  if (!(in >> c3) || c3 != ')')
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  return in;
}

std::ostream& velizade::operator<<(std::ostream& out, const Point& p)
{
  StreamGuard guard(out);
  out << '(' << p.x << ';' << p.y << ')';
  return out;
}

std::istream& velizade::operator>>(std::istream& in, Polygon& p)
{
  StreamGuard guard(in);
  size_t n;
  if (!(in >> n))
  {
    return in;
  }
  p.points.clear();
  p.points.reserve(n);
  std::copy_n(std::istream_iterator<Point>(in), n, std::back_inserter(p.points));
  if (in.fail() || p.points.size() != n || n < 3)
  {
    in.setstate(std::ios::failbit);
    p.points.clear();
  }
  return in;
}

std::ostream& velizade::operator<<(std::ostream& out, const Polygon& p)
{
  StreamGuard guard(out);
  out << p.points.size();
  std::copy(p.points.begin(), p.points.end(), std::ostream_iterator<Point>(out, " "));
  return out;
}

double velizade::getArea(const Polygon& p)
{
  if (p.points.size() < 3)
  {
    return 0.0;
  }
  std::vector<size_t> idx(p.points.size());
  std::iota(idx.begin(), idx.end(), 0);
  double twice = std::accumulate(idx.begin(), idx.end(), 0.0, CrossSum{p});
  return std::abs(twice) / 2.0;
}

bool velizade::isRectangle(const Polygon& p)
{
  if (p.points.size() != 4)
  {
    return false;
  }
  std::array<size_t, 4> idx = {0, 1, 2, 3};
  CheckRightAngle checker{p};
  return std::all_of(idx.begin(), idx.end(), checker);
}

bool velizade::PointInBox::operator()(const Point& pt) const
{
  return pt.x >= minX && pt.x <= maxX && pt.y >= minY && pt.y <= maxY;
}

velizade::BoundingBox::BoundingBox():
    minX(std::numeric_limits<int>::max()),
    maxX(std::numeric_limits<int>::min()),
    minY(std::numeric_limits<int>::max()),
    maxY(std::numeric_limits<int>::min()),
    empty(true)
{}

void velizade::BoundingBox::update(const Polygon& p)
{
  if (p.points.empty())
  {
    return;
  }
  auto xMinIt = std::min_element(p.points.begin(), p.points.end(), LessX());
  auto xMaxIt = std::max_element(p.points.begin(), p.points.end(), LessX());
  auto yMinIt = std::min_element(p.points.begin(), p.points.end(), LessY());
  auto yMaxIt = std::max_element(p.points.begin(), p.points.end(), LessY());
  if (empty)
  {
    minX = xMinIt->x;
    maxX = xMaxIt->x;
    minY = yMinIt->y;
    maxY = yMaxIt->y;
    empty = false;
  }
  else
  {
    minX = std::min(minX, xMinIt->x);
    maxX = std::max(maxX, xMaxIt->x);
    minY = std::min(minY, yMinIt->y);
    maxY = std::max(maxY, yMaxIt->y);
  }
}

bool velizade::BoundingBox::contains(const Polygon& p) const
{
  if (empty || p.points.empty())
  {
    return false;
  }
  PointInBox checker{minX, maxX, minY, maxY};
  return std::all_of(p.points.begin(), p.points.end(), checker);
}

bool velizade::isSameTranslation(const Polygon& a, const Polygon& b)
{
  if (a.points.size() != b.points.size() || a.points.empty())
  {
    return false;
  }
  std::set<Point> setB(b.points.begin(), b.points.end());
  const Point& a0 = a.points[0];
  FindShiftInB finder{a, setB, a0};
  return std::any_of(b.points.begin(), b.points.end(), finder);
}

bool velizade::IsEvenVertex::operator()(const Polygon& p) const
{
  return p.points.size() % 2 == 0;
}

bool velizade::IsOddVertex::operator()(const Polygon& p) const
{
  return p.points.size() % 2 == 1;
}

bool velizade::HasVertexCount::operator()(const Polygon& p) const
{
  return p.points.size() == count;
}

bool velizade::CompareAreaLess::operator()(const Polygon& a, const Polygon& b) const
{
  return getArea(a) < getArea(b);
}

bool velizade::CompareAreaGreater::operator()(const Polygon& a, const Polygon& b) const
{
  return getArea(a) > getArea(b);
}

bool velizade::CompareVertexLess::operator()(const Polygon& a, const Polygon& b) const
{
  return a.points.size() < b.points.size();
}

bool velizade::CompareVertexGreater::operator()(const Polygon& a, const Polygon& b) const
{
  return a.points.size() > b.points.size();
}

bool velizade::IsSameTranslationOf::operator()(const Polygon& p) const
{
  return isSameTranslation(p, target);
}

bool velizade::IsRectangleShape::operator()(const Polygon& p) const
{
  return isRectangle(p);
}

void velizade::readData(std::istream& in, std::vector<Polygon>& polygons)
{
  while (!in.eof())
  {
    Polygon p;
    if (in >> p)
    {
      polygons.push_back(p);
    }
    else
    {
      in.clear();
      in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }
}
