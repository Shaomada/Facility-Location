#ifndef COMMON_HPP
#define COMMON_HPP

#include <vector>
#include <string>

using std::vector;
using std::string;
using std::to_string;

class Point;
class Instance;

double abs( double x );
double max( double a, double b );

void check( bool is_true, std::string file, int line );

/* debug macro
 * call if the value IS_TRUE should be true
 */
#if 1
#define LOCATION __FILE__ , __LINE__
#define CHECK( IS_TRUE ) check( IS_TRUE, LOCATION )

#else
#define CHECK( IS_TRUE, ERROR )

#endif

#endif
