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

void check( bool is_true, std::string file, int line, std::string error );

#if 1
#define LOCATION __FILE__ , __LINE__
#define CHECK( IS_TRUE, ERROR ) check( IS_TRUE, LOCATION, ERROR )

#else
#define CHECK( IS_TRUE, ERROR )

#endif

#endif
