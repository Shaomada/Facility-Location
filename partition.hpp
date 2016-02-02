#ifndef INSTANCE_HPP
#define INSTANCE_HPP

#include "common.hpp"
#include <limits>

#define U_DEFAULT std::numeric_limits<double>::infinity()

typedef std::vector<unsigned> assignment;
typedef std::vector<unsigned> subset;
typedef std::vector<subset> subsets;

class Instance {
public:  
  Instance ( const std::string &filename, bool flag_f, double f,
                                          bool flag_u, double u );
  
  // for iterating over all partitions
  void next_partition();
  bool is_first_partition() const;
  
  // for handling a given partition
  double cost() const;
  void print() const;

private:
  // the Input
  double _u;
  double _f;
  std::vector<Point> _D;
  
  // both save the same partition for fast access
  assignment _x;
  subsets _v;
  
  // the optimal facilities for the current partition
  std::vector<Point> _I;

  void loadFromTSPLIB( const std::string &filename );

  // helpers, mainly for next_partition
  bool place( unsigned i );
  unsigned unplace();
  void create_singilton_subsets();

  void update_facilities();

  // output
  void print_cost() const;
  void print_assignment() const;
  void print_facilities() const;
};

#endif
