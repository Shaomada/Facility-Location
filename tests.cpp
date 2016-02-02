#include "common.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

using std::cerr;
using std::endl;
using std::cout;

int main ()
{
  try
  {
    std::ifstream in ( "testfile" );
    string line, word;
    int i;
    double x, y;
    std::stringstream ss;
    bool node_coord_section = false;
    
    CHECK( in, "Cannot open file." );
    while( getline( in, line ) )
    {
      if( node_coord_section )
      {
        ss = std::stringstream( line );
        ss >> word;
        if( word == "EOF" )
        {
          return;
        }

        ss = st::stringstream( line );
        ss >> i >> x >> y;
        _D.push_back( Point( x, y ) );

        CHECK( ss, "invalid file format" );
        CHECK( i == _D.size(), "invalid file format" );
      }
      else
      {
        ss = std::stringstream( line );
        ss >> word;
        if( word == "NODE_COORD_SECTION" )
        {
          node_coord_section = true;
          
          CHECK( ss, "invalid file format" );
        }
      }
    }

    CHECK( false, "invalid file format" );
  }
  catch( std::string exc ){
    cerr << "Exception caught: " << exc << endl;
  }
  
  return 0;
}
