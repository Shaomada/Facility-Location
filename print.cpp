#include "print.hpp"

using namespace std;


void print_rek( unsigned u )
{
  cout << u;
}

void print_after( unsigned u )
{
  cout << ' ';
}

template< typename type >
void print_rek( const vector< type > &v )
{
  for( unsigned i = 0; i < v.size(); print_after( v.at(i) ), i++ )
    print_rek( v.at( i ) );
}

template< typename type >
void print_after( const vector< type > &v )
{
  cout << endl;
}

template< typename type >
void print( type x )
{
  print_rek( x );
  cout << endl;
}

void load()
// hacked: make sure template gets compiled for this data type
// TODO: find better solution for this
{
  partition p;
  print( p );
}
