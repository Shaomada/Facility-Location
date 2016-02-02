#include "partitions.hpp"
#include "print.hpp"

using namespace std;

void apply_to_all_partitions( function< void(const partition &) > f, unsigned n )
// applys f to all partititons of {0, 1, ..., n-1}
{
  // begin with the unique partition of {0}
  partition p( 1, subvector(1, 0) );
  // stack[i] remembers, in which subvector i was placed
  stack s(1, 0);
  // backtrack will leave an empty stack if and only if
  // we allready iterated over all partitions
  while( s.size() > 0 )
  {
    if( s.size() < n )
    // if p is only a partition of < n elements
    {
      // add the first unplaced element to p.at( 0 );
      // later backtrack iters over all other possible placements
      p.at( 0 ).push_back( s.size() );
      s.push_back( 0 );
    }
    else
    // if p is a partition of n elements
    {
      // the whole point is to call f for each partition p
      f( p );
      // backtrackmagic makes sure we get each partition once
      backtrack( p, s );
    }
  }
}

void backtrack( partition &p, stack &s )
/* for the implementation of apply_to_all_partitions
 *
 * takes a partition and the corresponding stack
 * backtracks as far as neccesary to create a partial partition which
 * have not yet iterated over, or untill the stack is empty,
 * if no such partial partitions exist
 */
{
  while( s.size() > 0 )
  // note this loop will break for all function calls but the final
  {
    // save in which sets we have allready placed the biggest element
    unsigned j = s.back();
    // remove the biggest element from the partition
    s.pop_back();
    p.at( j ).pop_back();
    if( p.at( j ).size() == 0 )
      p.pop_back();
    // if we can place the element in another set, do so and break
    if( j+1 <= p.size() )
    {
      if( j+1 < p.size() )
      // if we dont need to create a new set
        p.at( j+1 ).push_back( s.size() );
      else
      // if we do need to create a new set
        p.push_back( subvector( 1, s.size() ) );
      s.push_back( j+1 );
      break;
    }
  }
}

void empty( const partition& p )
// a test function for apply_all_partition
{
  ;
}

void print_partition( const partition &p )
// another test function for apply_all_partition
{
  print( p );
}
