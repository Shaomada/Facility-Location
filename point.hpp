#ifndef POINT_HPP
#define POINT_HPP

#include "common.hpp"
#include <string>

class Point
// a weighted point in the 2-dimension Plane
// addition is taking the (weighted) barycenter
// subtraction is the inverse
{
public:
  Point( double x, double y );
  Point( const Point &p );

  Point &operator=( const Point &p );
  double operator+=( const Point &p );
  double operator-=( const Point &p );
  bool operator==( const Point &p ) const;

  std::string show() const;
  bool satisfies_capacity( unsigned u ) const;

  double dist( const Point& p ) const;
private:
  double _x;
  double _y;
  unsigned _w;
  
  double cost( const Point& p ) const;
};

#endif
