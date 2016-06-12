#ifndef POINT_HPP
#define POINT_HPP

#include "common.hpp"
#include <string>

class Point
/**
 * A weighted Point in the 2-dimensional Plane.
 * Addition is taking the (weighted) barycenter.
 * Implementation: _x/_y is _w-times the actual x/y coordinate.
 */
{
public:
  /// Neutral element of Addition.
  Point();
  ///Creates a Point with weight 1 at specified position.
  Point( double x, double y );
  // Creates a copy of a Point.
  Point( const Point &p );

  // Copies a point
  Point &operator=( const Point &p );
  /**
   * Takes the barycenter of 2 points, returns the cost.
   */
  double operator+=( const Point &p );
  /// Checks equality of weighted points.
  bool operator==( const Point &p ) const;

  std::string show() const;
  bool satisfies_capacity( unsigned u ) const;

  double dist( const Point& p ) const;
private:
  double _x;
  double _y;
  unsigned _w;

  /// the cost for moving p to this point
  double cost( const Point& p ) const;
};

#endif
