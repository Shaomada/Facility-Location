#ifndef INSTANCE_HPP
#define INSTANCE_HPP

#include "common.hpp"
#include <limits>

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
  Instance ( const std::string &filename, bool flag_f, double f,
                                          bool flag_u, double u );
                                          
  void solve();

private:
  // the actual Instance of the problem
  double _u;
  double _f;
  std::vector<Point> _D;
  
  // for the (possibly partial) assignemnt we consider right now
  assignment _x;
  std::vector<Point> _I;
  double _cost;
  
  // memory for backtracking
  std::vector<double> _mem_cost;
  std::vector<Point> _mem_I;

  // for the best assignment found so far
  assignment _best_x;
  std::vector<Point> _best_I;
  double _best_cost;

  // for loading from file
  void loadFromTSPLIB( const std::string &filename );

  // for iterating over all assignments in solve
  void next_assignment();
  bool finished() const;

  // helpers for next_assignment
  bool legal() const;
  void forward();
  void backward();
  void place( unsigned i );
  unsigned unplace();

  // for actually printing the best solution
  void save();
  void load();
  void print() const;

  // for default faciltity costs
  double max_dist();
};

#endif
