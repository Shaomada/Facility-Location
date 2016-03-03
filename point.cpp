#include "point.hpp"

Point::Point( double x, double y )
// creates a point with weight 1
{
  _x = x;
  _y = y;
  _w = 1;
}

Point &Point::operator=( const Point &p )
// copys a point
{
  _x = p._x;
  _y = p._y;
  _w = p._w;
  return *this;
}

Point::Point( const Point &p )
// copys a point
{
  _x = p._x;
  _y = p._y;
  _w = p._w;
}

#include <iostream>
using namespace std;

double Point::operator+=( const Point &p )
// takes the barycenter of 2 points, returns the cost
{
  Point q = *this;

  _w = q._w + p._w;
  _x = ( q._w*q._x + p._w*p._x ) / _w;
  _y = ( q._w*q._y + p._w*p._y ) / _w;

  return cost( p ) + cost( q );
}

double Point::operator-=( const Point &p )
// the inverse to +=
{
  CHECK( p._w < _w );

  Point q = *this;

  _w = q._w - p._w;
  _x = ( q._w*q._x - p._w*p._x ) / _w;
  _y = ( q._w*q._y - p._w*p._y ) / _w;

  return q.cost( p ) + q.cost( *this );
}

bool Point::operator==( const Point& p ) const
// checks if the weights are equal
{
  return p._w == _w;
}

std::string Point::show() const
// for showing the coordinates of a point
{
  return "\t" + std::to_string( _x ) + "\t" + std::to_string( _y );
}

bool Point::satisfies_capacity( unsigned u ) const
// test if facility capacity is fullfilled
{
  return _w <= u;
}

double Point::dist( const Point &p ) const
// the distance of p from this
{
  return ( _x-p._x )*( _x-p._x) + ( _y-p._y )*( _y-p._y );
}

double Point::cost( const Point &p ) const
// the cost for moving p to this point
{
  return p._w * ( ( _x-p._x )*( _x-p._x) + ( _y-p._y )*( _y-p._y ) );
}
