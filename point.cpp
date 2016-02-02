#include "point.hpp"

Point::Point( double x, double y )
{
  _x = x;
  _y = y;
}

Point &Point::operator=( const Point &q )
{
  _x = q._x;
  _y = q._y;
  return *this;
}

void Point::operator+=( const Point &q )
{
  _x += q._x;
  _y += q._y;
}

void Point::operator-=( const Point &q )
{
  _x -= q._x;
  _y -= q._y;
}

void Point::operator*=( double d )
{
  _x *= d;
  _y *= d;
}

void Point::operator/=( double d )
{
  _x /= d;
  _y /= d;
}

Point Point::operator+( const Point &q ) const
{
  Point p( _x, _y );
  p._x += q._x;
  p._y += q._y;
  return p;
}

Point Point::operator-( const Point &q ) const
{
  Point p( _x, _y );
  p._x -= q._x;
  p._y -= q._y;
  return p;
}

Point Point::operator*( double d ) const
{
  Point p( _x, _y );
  p._x *= d;
  p._y *= d;
  return p;
}

Point Point::operator/( double d ) const
{
  Point p( _x, _y );
  p._x /= d;
  p._y /= d;
  return p;
}

double Point::norm_sqr() const
{
  return _x*_x + _y*_y;
}


std::string Point::show() const
{
  return "\t" + std::to_string( _x ) + "\t" + std::to_string( _y );
}

double distance_sqr( const Point &x, const Point &y )
{
  return ( x-y ).norm_sqr();
}

Point barycenter( const std::vector<unsigned> s, const std::vector<Point> &v )
{
  Point q( 0, 0 );
  for( unsigned i : s )
    q += v.at( i );
  return q / (double) s.size();
}
