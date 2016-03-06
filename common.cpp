#include "common.hpp"

using namespace std;

double abs( double x )
// the absolute value of x
{
  return x < 0 ? -x : x;
}

double max( double a, double b )
// the maximum of a and b
{
  return b > a ? b : a;
}

void check( bool is_true, string file, int line )
/* for debugging
 * call with is_true an expression you expect to evaluate to true
 * an errormessage will be shown if it evaluates to false
 */
{
  if( !is_true )
    throw "CHECK failed in File " + file + " Line " + std::to_string( line );
}
