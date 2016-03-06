#include "instance.hpp"
#include "point.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

Instance::Instance ( const string &filename, bool flag_f, double f,
                                              bool flag_u, double u )
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
    
  // make sure _best_cost is greater than any cost we'll find
  _best_cost = std::numeric_limits<double>::infinity();

  // intial assignment
  next_assignment();
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
  while( not finished() )
  {
    if( _cost < _best_cost )
      save();
    next_assignment();
  }
  load();
  print();
}

void Instance::next_assignment()
/* finds the next full assignments, where assignments are ordered
 * lexicographically or the empty assignment if there is no next
 * full assignment
 */
{
  if( _x.size() != 0 )
  /* if x is allready an assignment, not an empty vector,
   * we need to go backward to a point
   * where we can assign differntly
   */
  {
    backward();
    if( _x.size() == 0 )
    // so we stop after the last assignment
      return ;
  }

  while( _x.size() < _D.size() )
  {
    forward();
    while( !legal() )
      backward();

    if( _x.size() == 0 )
    // so we stop after the last assignment
      return ;
  }
}

bool Instance::finished() const
// To check if next_assignment allready ran through all assignments
{
  return _x.size() == 0;
}

bool Instance::legal() const
/* checks if the current assignment is legal
 * ASSUMING the assignment was legal before we
 * set the back of _x to it's current value.
 * we optimise runtime by also returning false if any assignment
 * we could find has allready higher cost then our optimum so far
 */
{
  if( _x.size() == 0 )
  // spectial case
  {
    return true;
  }
  else if( not _I.at( _x.back() ).satisfies_capacity( _u ) )
  // the actuall point of this function
  {
    return false;
  }
  else if ( _cost >= _best_cost )
  // the optimisation
  {
    return false;
  }
  else
  {
    return true;
  }
}

void Instance::forward()
// the lexicographically next partial assignment
{
  place( 0 );
}

void Instance::backward()
/* the lexicographically next partial assignment skipping
 * all partial assignments which start with the current _x
 */
{
  unsigned i;
  do
  // unplace until we can place at i+1
    i = unplace();
  while( _x.size() > 0 && i >= _I.size() );
  if( i < _I.size() )
    place( i+1 );
}

void Instance::place( unsigned i )
/* Given a partial assignment, assigns the next customer to the i-th facility
 * updates optimal position of the i-th facility and optimal cost
 */
{
  // check if the parameter was valid
  CHECK( i <= _I.size() );

  // for more readability
  int m = _x.size();

  // update _x
  _x.push_back( i );

  // memory 1
  _mem_cost.push_back( _cost );

  if( i == _I.size() )
  // special case: opening new facility
  {
    _I.push_back( Point() );
    _cost += _f;
  }
  else
  // otherwise: memory 2
  {
    _mem_I.push_back( _I.at( i ) );
  }
  
  // update _I and _cost
  _cost += ( _I.at( i ) += _D.at( m ) );
}

unsigned Instance::unplace()
// the inverse map to place
{
  // We can only unplace if something is placed
  CHECK( _x.size() > 0 );
  
  // for more readability
  unsigned m = _x.size() - 1;
  unsigned i = _x.back();

  // Update _x
  _x.pop_back();
  
  // update _cost
  _cost = _mem_cost.back();
  _mem_cost.pop_back();
  
  // update _I
  if( _I.at( i ) == _D.at( m ) )
  {
    CHECK( i+1 == _I.size() );
    _I.pop_back();
  }
  else
  {
    _I.at( i ) = _mem_I.back();
    _mem_I.pop_back();
  }

  return i;
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

void Instance::save()
// saves best solution so far
{
  _best_x = _x;
  _best_I = _I;
  _best_cost = _cost;
}

void Instance::load()
// loads best solution found
{
  _x = _best_x;
  _I = _best_I;
  _cost = _best_cost;
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
