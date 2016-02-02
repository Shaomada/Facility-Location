#include "main.hpp"
#include "instance.hpp"
#include "point.hpp"
#include <iostream>

using namespace std;

void optimize( Instance &p )
{
  CHECK( p.is_first_partition(), "invalid parameter" );

  double mincost = std::numeric_limits<double>::infinity();
  do
  {
    double cost = p.cost();
    if( cost < mincost )
    {
      mincost = cost;
      p.save();
    }
    p.next_partition();
  } while( not p.is_first_partition() );
  p.load();
  p.print();
}

int main ( int argc, char **argv )
{
  if( argc == 1 )
  {
    cout << "no filename" << endl;
    return 1;
  }
  try
  {
    // read from command line
    string filename( argv[1] );
    bool flag_f = false;
    bool flag_u = false;
    double f = 0;
    double u = 0;
    string str_f( "-f" );
    string str_u( "-u" );
    
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
      else
      {
        cout << "unknown flag " << argv[i] << endl;
        return 1;
      }
    }
    
    cout << "read:" << endl
         << "\tfilename\t" << filename << endl
         << "\tf\t" << f << endl
         << "\tu\t" << u << endl
         << "\tflag_f\t" << flag_f << endl
         << "\tflag_u\t" << flag_u << endl;
    
    // create an Instance
    Instance p( filename, flag_f, f, flag_u, u );
    
    // solve the problem
    optimize( p );
  }
  catch( std::string exc ){
    std::cerr << "Exception caught: " << exc << std::endl;
  }

  return 0;
}
