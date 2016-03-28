#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "common.hpp"

using namespace std;

// typedef bool Direction;
/*
 * true: take the forward Edge in the Residual Graph
 * false: take the backward Edge in the Residual Graph
 */

class Vertex
{
public:
  Vertex( int b );

  // for creating the Graph
  void add_edge( Edge edge, bool dir );

  // void dij_visit()

private:
  // data as a Vertex in a Graph
  vector<Flag> _edges;
  // the bool is true iff you are supposed to take the forward edge in RG

  // additional data as Vertex in a Network
  int _b;

  // for dijkstra
  bool _dij_visited;
  float _dij_distance;
  pair<Flag> _dij_parent;
  vector<Flag> _dij_childs;
};

class Flag
{
public:
  Flag( bool is_head, Edge edge );

private:
  Edge _edge;
  bool _is_head;
};

class Edge
{
public:
  Edge( Vertex tail, Vertex head, unsigned capacity, float cost = 0 );

private:
  // data as an Edge of a Graph
  Vertex _tail;
  Vertex _head;

  // additional data as an Edge in a Network
  double _cost;
  unsigned _flow;
  unsigned _capacity;
};

class Graph
{
public:
  // read Input SSP
  Graph( const Instance &inst );

  // output SSP
  vector<unsigned> output( const Instance &inst );

private:
  // relevant data from Instance
  unsigned n_D;
  unsigned n_I;

  // for saving the vertices
  vector<Vertex> _vertices;
};

#endif
