#include "graph.hpp"
#include "instance.hpp"

// for initally creating the Graph

Graph::Graph( const Instance &inst )
{
  // Vertices
  _vertices = vector<Vertex>();

  // read
  n_D = inst.size_D();
  n_I = inst.size_I();
  unsigned u = inst.get_u();

  // Vertices for _D
  for( unsigned i = 0; i < n_D; i++ )
  {
    _vertices.push_back( Vertex( -1 ) );
  }

  // Vertices for _I
  for( unsigned j = 0; j < n_D; j++ )
  {
    _vertices.push_back( Vertex( 0 ) );
  }
  
  // Vertex as Source
  _vertices.push_back( Vertex( n_D ) );

  // Edges
  for( unsigned j = n_D; j < n_D+n_I; j++ ){
    for( unsigned i = 0; i < n_D; i++ )
      Edge( vertex.at( j ), vertex.at( i ), 1 );
    Edge( vertex.at( n_D+n_I ), vertex.at( j ), u )
  }
  
  // the costs will be set later for the Edge which cost /= 0
}

Vertex::Vertex( int b )
{
  
}
