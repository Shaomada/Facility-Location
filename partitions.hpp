#ifndef PARTITIONS_HPP
#define PARTITIONS_HPP

#include <vector>
#include <functional>

typedef std::vector<unsigned> subvector;
typedef std::vector<subvector> partition;

typedef std::vector<unsigned> stack;

// applys f to all partititons of {0, 1, ..., n-1}
void apply_to_all_partitions( std::function< void(const partition &) > f, unsigned n );

// for the implementation of apply_to_all_partitions
void backtrack( partition &p, stack &s );

// test functions for apply_to_all_partitions
void empty( const partition& p );
void print_partition( const partition &v );

#endif
