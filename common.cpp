#include "common.hpp"

using namespace std;

double abs( double x ){
  return x < 0 ? -x : x;
}

double max( double a, double b )
{
  return b > a ? b : a;
}

void check( bool is_true, string file, int line )
{
  if( !is_true )
    throw "CHECK failed in File " + file + " Line " + std::to_string( line );
}
