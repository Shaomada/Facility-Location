#ifndef POINT_HPP
#define POINT_HPP

#include "common.hpp"
#include <string>

class Point
{
public:
  Point( double x, double y );

  Point &operator=( const Point &q );
  void operator+=( const Point &q );
  void operator-=( const Point &q );
  void operator*=( double d );
  void operator/=( double d );
  Point operator+( const Point &q ) const;
  Point operator-( const Point &q ) const;
  Point operator*( double d ) const;
  Point operator/( double d ) const;

  double norm_sqr() const;
  
  std::string show() const;
private:
  double _x;
  double _y;
};

double distance_sqr( const Point &x, const Point &y );

Point barycenter( const std::vector<unsigned> s, const std::vector<Point> &v );
Point median( const std::vector<Point> &v );

#endif
