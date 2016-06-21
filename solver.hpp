#ifndef SOLVER_HPP
#define SOLVER_HPP

#include "heap.hpp"
#include <vector>
#include <string>

class Solver {
public:
  typedef unsigned flow_t;
  typedef double dist_t;
  class Point;

  Solver(flow_t u, const std::vector<Point> &D, const std::vector<Point> &I, dist_t facility_cost);
  void solve ();

private:
  class Customer;
  class Facility;

  /// finds the optimal Facility locations for current assignment (Barycenters)
  void optimize_I ();
  
  /// finds the optimal assignment for the current Facility locations (MaxFlow/SSP)
  void optimize_x ();
  
  /// computes the cost of the current assignment
  void compute_cost ();
  
  // prints the current assignment
  void print ();
  
#if 0
  // prints the current dij_tree
  void print_tree ();
#endif
  
  /// Successive Shortest Path Algorithm Implementation
  void ssp_algorithm ();
  void ssp_init ();
  flow_t ssp_push ();
  bool try_push (Customer *c);
  void dij_init ();
  void dij_algorithm ();
  void dij_compute_children ();
  
  const flow_t _u;
  std::vector<Customer> _D;
  std::vector<Facility> _I;
  Heap<Facility> _heap;
  dist_t _cost;
  dist_t _facility_cost;
};

struct Solver::Point
{
  Point (dist_t x, dist_t y) : x(x), y(y) { }
  inline std::string show() const { return "\t"+std::to_string(x)+"\t"+std::to_string(y); }
  inline static dist_t dist(const Point &p, const Point &q) { return (p.x-q.x)*(p.x-q.x)+(p.y-q.y)*(p.y-q.y); };
  dist_t x;
  dist_t y;
};

struct Solver::Customer : public Point {
  inline Customer(Point p)
    : Point(p.x, p.y) { }
  Facility *flow_parent;
  Facility *dij_parent;
  dist_t dij_dist;
};

struct Solver::Facility : public Point {
  inline Facility(Point p)
    : Point(p.x, p.y) { }
  inline bool operator<(const Facility &rhs) const { return dij_dist < rhs.dij_dist; } // for heap in dijkstra
  flow_t outflow;
  Customer *dij_parent;
  std::vector<Customer *> dij_children;
  dist_t dij_dist;
  dist_t pi;
  Heap<Facility>::Node *heap_node;
};

#endif
