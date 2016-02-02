#include "instance.hpp"
#include "point.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

void debug( vector<unsigned> x, double _cost )
{
  for( unsigned i = 0; i < x.size(); cout << ", ", i++ )
    cout << x.at(i);
  cout << endl;
  cout << "cost\t" << _cost << endl;
}

Instance::Instance ( const string &filename, bool flag_f, double f,
                                              bool flag_u, double u )
{
  loadFromTSPLIB( filename );

  if( flag_f )
    _f = f;
  else
    _f = 0; // TODO

  if( flag_u )
    _u = u;
  else
    _u = U_DEFAULT;

  _cost = 0;
  _best_cost = std::numeric_limits<double>::infinity();

  first_partition();
}

void Instance::loadFromTSPLIB( const string &filename )
{
  _D = vector<Point>();
  _x = assignment();

  std::ifstream in ( "testfile" );
  string line, word;
  unsigned i;
  double x, y;
  bool node_coord_section = false;

  CHECK( in );
  while( getline( in, line ) )
  {
    if( node_coord_section )
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

  // shouldn't be here as last line should read EOF
  CHECK( false );
}

void Instance::solve()
{
  while( not finished() )
  {
    if( _cost < _best_cost )
      save();
    next_partition();
  }
  load();
  print();
}

void Instance::next_partition()
/* Picture all partitions of sets {1, ..., m} with m <= |D| as an aborescens
 * with an order onoutgoing edges at every vertex. Basic idea is to back-
 * track untill there is a higher order outgoing edge where we are, then
 * allways take the lowest order edge possible.
 * To also consider things like capacities, we check legality and might need
 * to do additional steps backwards.
 * Iff we find no partition this way, we allready iterated over all
 * paritions and the while loop stops with _x.size() == 0.
 */
{
  backward();
  if( _x.size() == 0 )
    return ;

  while( _x.size() < _D.size() )
  {;
    forward();
    while( !legal() )
      backward();
    if( _x.size() == 0 )
      return ;
  }
}

void Instance::first_partition()
// does exactly the same as next_bartition apart from the initial step back
{
  while( _x.size() < _D.size() )
  {
    forward();
    while( !legal() )
      backward();
    if( _x.size() == 0 )
      return ;
  }
}

bool Instance::finished() const
// See expanation of next_partion for context
{
  return _x.size() == 0;
}

bool Instance::legal() const
// TODO
{
  if( _x.size() == 0 )
  {
    return true;
  }
  else if( not _I.at( _x.back() ).satisfies_capacity( _u ) )
  {
    return false;
  }
  else if ( _cost >= _best_cost && true ) // HERE
  {
    return false;
  }
  else
  {
    return true;
  }
}

void Instance::forward()
{
  place( 0 );
}

void Instance::backward()
{
  unsigned i;
  do
    i = unplace();
  while( _x.size() > 0 && i >= _I.size() );
  if( i < _I.size() )
    place( i+1 );
}

void Instance::place( unsigned i )
/* If we have a partial Assignment from {0, ..., m-1} to {0, ..., k-1}
 * this function takes an index 0 <= i <= k and creates a partial Assignment
 * {0, ..., m} by mapping m to i.
 * We also update the optimal faciltiy placement and cost for the assignment.
 */
{
  // check if the parameter was valid
  CHECK( i <= _I.size() );

  // for more readability
  int m = _x.size();

  // update assignment
  _x.push_back( i );

  // update _I and _cost
  if( i < _I.size() )
  {
    _cost += ( _I.at( i ) += _D.at( m ) );
  }
  else
  {
    _I.push_back( Point( _D.at( m ) ) );
    _cost += _f;
  }
}

unsigned Instance::unplace()
/* If we have a partial assignment of {0, ..., m} to {0, .. k}, we create
 * a partial assignment of {0, ..., m-1} by unplacing m.
 * Note that by convention the sets are ordered by smallest element ascending,
 * so if we need to delete any set, it should be _v[k]
 * We also update the facility placement and cost for the assignment.
 */
{
  // Check if we can uplace something
  CHECK( _x.size() > 0 );
  
  // for more readability
  unsigned m = _x.size() - 1;
  unsigned i = _x.back();

  // Update assignment
  _x.pop_back();
  
  // update 
  _I.at( i );
  _D.at( m );
  if( _I.at( i ) == _D.at( m ) )
  {
    CHECK( i+1 == _I.size() );
    _I.pop_back();
    _cost -= _f;
  }
  else
  {
    _cost -= ( _I.at( i ) -= _D.at( m ) );
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
// writes best solution so far
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
