#ifndef INSTANCE_HPP
#define INSTANCE_HPP

#include "common.hpp"
#include <limits>

#define U_DEFAULT -1

typedef std::vector<unsigned> assignment;
typedef std::vector<unsigned> subset;
typedef std::vector<subset> subsets;

class Instance {
public:  
  Instance ( const std::string &filename, bool flag_f, double f,
                                          bool flag_u, double u );
                                          
  void solve();

private:
  // the Input
  double _u;
  double _f;
  std::vector<Point> _D;
  
  // a possible Solution
  assignment _x;
  std::vector<Point> _I;
  double _cost;
  
  // for recalling the opitmal solution
  assignment _best_x;
  std::vector<Point> _best_I;
  double _best_cost;

  // for loading from file
  void loadFromTSPLIB( const std::string &filename );

  // for iterating over all partitions
  void first_partition();
  void next_partition();
  bool finished() const;

  // helpers for next_partition
  bool legal() const;
  void forward();
  void backward();
  void place( unsigned i );
  unsigned unplace();

  // for actually printing the best solution
  void save();
  void load();
  void print() const;
};

#endif
