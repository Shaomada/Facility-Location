#ifndef INSTANCE_HPP
#define INSTANCE_HPP

#include "common.hpp"
#include "graph.hpp"

#define U_DEFAULT -1

typedef std::vector<unsigned> assignment;

class Instance
/*
 * an Instance of the problem together with a (partial) assignment,
 * optimal facility locations and cost for the assignment,
 * as well as the best placement found so far
 */
{

public:
  ~Instance ();
  Instance ( const std::string &filename,
             bool flag_f, double f,
             bool flag_u, double u,
             bool flag_k, unsigned k
           );

  /// Finds a "good" solution for this Instance.
  void solve();

private:
  /// the actual Instance of the problem
  /// @{
  double _u;
  double _f;
  std::vector<Point> _D;
  /// @}
  
  /// the solution we consider right now
  /// @{
  assignment _x;
  std::vector<Point> _I;
  double _cost;
  /// @}

  /// for minimum cost flow via ssp
  Graph *_graph;

  /// for loading from file
  void loadFromTSPLIB( const std::string &filename );

  /// TODO: random assignemnt?
  void initial_assignment();
  
  /// finds optimal x for current I
  void optimize_x();
  
  /// finds optimal I for current x
  void optimize_I();
  
  /// to decide if we should continue trying to improve our solution
  bool more_that_marginal_improvement( double old_cost );

  /// prints the current solution to std::cout
  void print() const;

  /// the maximal distance of two customers
  double max_dist();
};

#endif
