#ifndef PRINT_HPP
#define PRINT_HPP

#include "common.hpp"
#include <iostream>

// for recursive calls while printing a vector
void print_rek( unsigned u );
template< typename type >
void print_rek( const std::vector< type > &v );

// for between tow recursive calls while printing a vector
void print_after( unsigned u );
template< typename type >
void print_after( const std::vector< type > &v );

// for the actual non-recursive call
template< typename type >
void print( type x );

#endif
