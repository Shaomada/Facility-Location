#include "point.hpp"

Point::Point()
{
  _x = 0;
  _y = 0;
  _w = 0;
}

Point::Point( double x, double y )
{
  _x = x;
  _y = y;
  _w = 1;
}

Point::Point( const Point &p )
{
  _x = p._x;
  _y = p._y;
  _w = p._w;
}

Point &Point::operator=( const Point &p )
{
  _x = p._x;
  _y = p._y;
  _w = p._w;
  return *this;
}

double Point::operator+=( const Point &p )
{
  Point q = *this;  
  
  _w += p._w;
  _x += p._x;
  _y += p._y;

  return cost( p ) + cost( q );
}

bool Point::operator==( const Point& p ) const
// checks if the points are equal
{
  return p._w == _w && p._x == _x && p._y == _y;
}

std::string Point::show() const
// for showing the coordinates of a point
{
  return "\t" + std::to_string( _x/_w ) + "\t" + std::to_string( _y/_w );
}

bool Point::satisfies_capacity( unsigned u ) const
// test if facility capacity is fullfilled
{
  return _w <= u;
}

double Point::dist( const Point &p ) const
// the (unweighted) distance of p from this
{
  double dx = _x/_w - p._x/p._w;
  double dy = _y/_w - p._y/p._w;
  return dx*dx + dy*dy;
}

double Point::cost( const Point &p ) const
{
  if( p._w == 0 )
    return 0;
  else
    return p._w * dist( p );
}
