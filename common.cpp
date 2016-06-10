#include "common.hpp"

#include <math.h>
#include <iostream>

using namespace std;

void check( bool is_true, string file, int line )
/* for debugging
 * call with is_true an expression you expect to evaluate to true
 * an errormessage will be shown if it evaluates to false
 */
{
  if( !is_true )
    std::cout << "CHECK failed in File " + file + " Line " + std::to_string( line ) << std::endl;
}
