#include "partition.hpp"
#include "point.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

// general Partiton stuff not specific to the problem

Partition::Partition ( const string &filename, bool flag_f, double f,
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

  create_singilton_subsets();
  update_facilities();
}

void Partition::loadFromTSPLIB( const string &filename )
{
  _D = vector<Point>();
  _x = assignment();
  _v = subsets();
  
  ifstream in ( filename.c_str() );
  CHECK( false, "CHECK is working!!!" );
  check( false, __FILE__, __LINE__, "pls" );
  CHECK( in, "Cannot open file." );
  
  string line;
  while( getline( in, line ) )
  {
    stringstream ss( line );
    string word;
    ss >> word;
    if( word == "NODE_COORD_SECTION" )
      break;
  }
  
  while( getline( in, line ) )
  {
    stringstream ss( line );
    string i;
    double x;
    double y;
    ss >> i >> x >> y;
    if( i == "EOF" )
      return;
    CHECK( ss, "invalid file format" );
    _D.push_back( Point( x, y ) );
    CHECK( i == to_str( _D.size() ), "invalid file format" );
  }
}

void Partition::next_partition()
/* backtrack untill we can place an element in a lower index subset
 * then fill all elements in maximum index subsest possible,
 * ie. call create_singilton_subsets. If we find no such possible placement,
 * we are allready at the last partition and call create_singilton_subsets
 * to create the first partition instead.
 * Note we will place an Element in a lower index Subset if we find one
 * regardless of capacity, but if it was not legal, we dont break, so the
 * very next run of the loop will begin with unplacing that element and
 * looking for even smaller index subsets to place it in.
 */
{
  while( _x.size() > 0 )
  {
    unsigned former_index = unplace();
    if( former_index > 0 )
      if( place( former_index-1 ) )
        break;
  }
  create_singilton_subsets();
}

bool Partition::is_first_partition() const
// the first partition is the one in which we have |D| singilton subsets
{
  return _v.size() == _D.size();
}

bool Partition::place( unsigned i )
/* places the lowest index unplaced element,
 * ie. i_x.size(), in the i-th subset
 * and returns true iff it was legal with respect to capacity
 * (but does not undo the placement if it was unlegal!)
 */
{ 
  if( i < _v.size() )
  {
    _v.at( i ).push_back( _x.size() );
    _x.push_back( i );
  }
  else
  {
    CHECK( i == _v.size(), "index too large" );
    _v.push_back( subset( 1, _x.size() ) );
    _x.push_back( i );
  }
  
  return _v.at( i ).size() <= _u;
}

unsigned Partition::unplace()
// unplaces the highest index Element, returns index of set it used to be in
{
  // unplace from _x
  CHECK( _x.size() > 0, "nothing to unplace" );
  unsigned i = _x.back();
  _x.pop_back();

  // unplace from _v
  CHECK( _v.at( i ).size() > 0 && _v.at( i ).back() == _x.size(), "data invalid" )
  _v.at( i ).pop_back();
  
  // remove the subset if empty
  if( _v.at( i ).size() == 0 )
  {
    _v.pop_back();
    CHECK( _v.size() == i, "data invalid" );
  }
  
  return i;
}

void Partition::create_singilton_subsets()
// places each unplaced Element in a new singilton subset
{
  while( _x.size() < _D.size() )
    place( _v.size() );
}

// specific to the Problem

void Partition::update_facilities()
{
  _I = vector<Point>();
  for( const subset &s : _v )
    _I.push_back( barycenter( s, _D ) );
}

double Partition::cost() const
{
  double cost = _I.size() * _f;
  for( unsigned i = 0; i < _D.size(); i++ )
  {
    cost += distance_sqr( _D.at( i ), _I.at( _x.at( i ) ) );
  }
  return cost;
}


void Partition::print() const
{
  print_cost();
  print_facilities();
  print_assignment();
}

void Partition::print_cost() const
{
  cout << "OBJECTIVE " << cost() << endl;
}

void Partition::print_facilities() const
{
  for( unsigned i = 0; i < _I.size(); i++ )
    cout << "FACILITY " << i+1 << " " << _I.at(i).show() << endl;
}

void Partition::print_assignment() const
{
  for( unsigned i = 0; i < _x.size(); i++ )
    cout << "ASSIGN " << i+1 << " " << _x.at( i )+1 << endl;
}
