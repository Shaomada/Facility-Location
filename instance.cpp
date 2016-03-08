#include "instance.hpp"
#include "point.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

Instance::Instance ( const std::string &filename,
                     bool flag_f, double f,
                     bool flag_u, double u,
                     bool flag_k, unsigned k
                   )
// creates an Instance from flags and filename
{
  // set _D
  loadFromTSPLIB( filename );

  // set _f
  if( flag_f )
    _f = f;
  else
  {
    _f = 2 * max_dist();
  }

  // set _u
  if( flag_u )
    _u = u;
  else
    _u = U_DEFAULT;

  // set _k
  if( not flag_k )
    k = max( ceil( (double) _D.size() / _u ), (double) 1 );
  
  // start with empty assignment
  _x = assignment();
  _I = vector<Point>( k, Point() );
  _cost = 0;
}

void Instance::loadFromTSPLIB( const string &filename )
// loads the points from file
{
  _D = vector<Point>();

  std::ifstream in ( "testfile" );
  string line, word;
  unsigned i;
  double x, y;
  bool node_coord_section = false;

  CHECK( in );
  while( getline( in, line ) )
  {
    if( node_coord_section )
    // second part: read the coordinates of customers
    {
      {
        std::stringstream ss ( line );
        ss >> word;
        if( word == "EOF" )
        {
          return;
        }
      }

      std::stringstream ss( line );
      ss >> i >> x >> y;
      _D.push_back( Point( x, y ) );

      CHECK( ss );
      CHECK( i == _D.size() );
    }
    else
    // first part: wait for node_coord_section to start
    {
      std::stringstream ss( line );
      ss >> word;
      if( word == "NODE_COORD_SECTION" )
      {
        node_coord_section = true;
        CHECK( ss );
      }
    }
  }

  // this check wont be exectued for valid input files
  CHECK( false );
}

void Instance::solve()
// finds the optimal solution for this Instance and prints it
{
  initial_assignment();
  optimize_I();
  double old_cost;
  do
  {
    old_cost = _cost;
    optimize_x();
    optimize_I();
  }
  while( more_that_marginal_improvement( old_cost ) );
  print();
}

void Instance::initial_assignment()
{
  for( unsigned i = 0; i < _D.size(); i++ )
  {
    _x.push_back( i % _I.size() ); // TODO: better initial assignment
  }
}

void Instance::optimize_x()
// finds optimal _x for current _I
{
  ; // TODO: implement this
}

void Instance::optimize_I()
// finds optimal _I and corresponding cost for current _x
{
  // reset _I and _cost
  for( Point &p : _I )
    p = Point();
  _cost = _I.size() * _f;

  // find optimal _I and _cost
  for( unsigned m = 0; m < _x.size(); m++ )
    _cost += ( _I.at( _x.at( m ) ) += _D.at( m ) );
}

bool Instance::more_that_marginal_improvement( double old_cost )
// use any heuristic for when to stop here
{
  return _cost < 0.999999 * old_cost;
}

void Instance::print() const
// prints a solution
{
  cout << "OBJECTIVE " << _cost << endl;
  for( unsigned i = 0; i < _I.size(); i++ )
    cout << "FACILITY " << i+1 << " " << _I.at(i).show() << endl;
  for( unsigned i = 0; i < _x.size(); i++ )
    cout << "ASSIGN " << i+1 << " " << _x.at( i )+1 << endl;
}

double Instance::max_dist()
/* returns the maximum over all pairs of customers
 * of the squared euclidean distance
 */
{
  double maximum = 0;
  for( unsigned i = 0; i < _D.size(); i++ )
  {
    for( unsigned j = 0; j < _D.size(); j++ )
    {
      double distance = _D.at( i ).dist( _D.at( j ) );
      if( distance > maximum )
        maximum = distance;
    }
  }
  return maximum;
}
