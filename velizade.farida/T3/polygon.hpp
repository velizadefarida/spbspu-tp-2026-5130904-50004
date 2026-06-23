#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <vector>
#include <iostream>
#include <set>
#include <array>

namespace velizade
{
  struct Point
  {
    int x, y;
  };

  bool operator==(const Point& a, const Point& b);
  bool operator<(const Point& a, const Point& b);

  struct Polygon
  {
    std::vector<Point> points;
  };

  bool operator==(const Polygon& a, const Polygon& b);
  bool operator!=(const Polygon& a, const Polygon& b);

  std::istream& operator>>(std::istream& in, Point& p);
  std::ostream& operator<<(std::ostream& out, const Point& p);
  std::istream& operator>>(std::istream& in, Polygon& p);
  std::ostream& operator<<(std::ostream& out, const Polygon& p);

  double getArea(const Polygon& p);
  bool isRectangle(const Polygon& p);
  bool isSameTranslation(const Polygon& a, const Polygon& b);

  struct PointInBox
  {
    int minX, maxX, minY, maxY;
    bool operator()(const Point& pt) const;
  };

  struct BoundingBox
  {
    int minX, maxX, minY, maxY;
    bool empty;

    BoundingBox();
    void update(const Polygon& p);
    bool contains(const Polygon& p) const;
  };

  struct IsEvenVertex
  {
    bool operator()(const Polygon& p) const;
  };

  struct IsOddVertex
  {
    bool operator()(const Polygon& p) const;
  };

  struct HasVertexCount
  {
    size_t count;
    bool operator()(const Polygon& p) const;
  };

  struct CompareAreaLess
  {
    bool operator()(const Polygon& a, const Polygon& b) const;
  };

  struct CompareAreaGreater
  {
    bool operator()(const Polygon& a, const Polygon& b) const;
  };

  struct CompareVertexLess
  {
    bool operator()(const Polygon& a, const Polygon& b) const;
  };

  struct CompareVertexGreater
  {
    bool operator()(const Polygon& a, const Polygon& b) const;
  };

  struct IsSameTranslationOf
  {
    Polygon target;
    bool operator()(const Polygon& p) const;
  };

  struct IsRectangleShape
  {
    bool operator()(const Polygon& p) const;
  };

  template <typename Pred>
  struct SumAreaIf
  {
    Pred pred;
    double operator()(double acc, const Polygon& p) const
    {
      return acc + (pred(p) ? getArea(p) : 0.0);
    }
  };

  struct AlwaysTrue
  {
    bool operator()(const Polygon&) const
    {
      return true;
    }
  };

  void readData(std::istream& in, std::vector<Polygon>& polygons);
}

#endif
