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
  Instance ( const std::string &filename,
             bool flag_f, double f,
             bool flag_u, double u,
             bool flag_k, unsigned k
           );
                                          
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

  // for loading from file
  void loadFromTSPLIB( const std::string &filename );

  // helpers for solve
  void initial_assignment();
  void optimize_x();
  void optimize_I();
  bool more_that_marginal_improvement( double old_cost );

  // for actually printing the solution
  void print() const;

  // for default faciltity costs
  double max_dist();
};

#endif
