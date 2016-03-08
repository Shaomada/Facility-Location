#include "main.hpp"
#include "instance.hpp"
#include "point.hpp"
#include <iostream>

using namespace std;

int main ( int argc, char **argv )
{
  if( argc == 1 )
  {
    cout << "Usage: " << argv[0] << " FILE [OPTION]" << endl;
    return 1;
  }
  try
  {
    // read from command line
    string filename( argv[1] );
    bool flag_f = false;
    bool flag_u = false;
    bool flag_k = false;
    double f = 0;
    double u = 0;
    unsigned k = 0;
    string str_f( "-f" );
    string str_u( "-u" );
    string str_k( "-k" );
    
    for( int i = 2; i < argc; i++ )
    {
      if( str_f.compare( argv[i] ) == 0 )
      {
        i++;
        f = std::stod( argv[i] );
        flag_f = true;
      }
      else if( str_u.compare( argv[i] ) == 0 )
      {
        i++;
        u = std::stod( argv[i] );
        flag_u = true;
      }
      else if( str_k.compare( argv[i] ) == 0 )
      {
        i++;
        k = std::stod( argv[i] );
        flag_k = true;
      }
      else
      {
        cout << "unknown flag " << argv[i] << endl;
        return 1;
      }
    }
    
    // create an Instance
    Instance x( filename, flag_f, f, flag_u, u, flag_k, k );
    
    // solve the problem
    x.solve();
  }
  catch( std::string exc ){
    std::cerr << "Exception caught: " << exc << std::endl;
  }

  return 0;
}
