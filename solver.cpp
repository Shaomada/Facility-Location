#include "solver.hpp"
#include <iostream>
#include <functional>
#include <limits>

#ifdef PRINT_ADDITIONAL_INFORMATION
#include <chrono>
#endif

Solver::Solver(flow_t u, const std::vector< Point >& D, const std::vector< Point >& I, dist_t facility_cost)
  : _u(u)
  , _D()
  , _I()
  , _heap()
  , _cost(std::numeric_limits<dist_t>::infinity())
  , _facility_cost(facility_cost)
{
  for (const Point &p : D) {
    _D.emplace_back(p);
  }
  for (const Point &p : I) {
    _I.emplace_back(p);
  }
}

void Solver::solve () {
  dist_t old_cost;
  do {
    old_cost = _cost;
    optimize_x();
    optimize_I();
    compute_cost();

#ifdef PRINT_ADDITIONAL_INFORMATION
    std::cout << "cost improved to\t" << _cost << "\tfrom\t" << old_cost << std::endl;
#endif

  } while (old_cost - _cost > 1e-10 * old_cost || old_cost == std::numeric_limits<double>::infinity());
  print();
}

void Solver::optimize_I () {
  for (Facility &f : _I) {
    f.x = 0;
    f.y = 0;
  }
  for (const Customer &c : _D) {
    c.flow_parent->x += c.x;
    c.flow_parent->y += c.y;
  }
  for (Facility &f : _I) {
    if (f.outflow != 0) {
      f.x /= f.outflow;
      f.y /= f.outflow;
    } else {
      /* one may put code which moves unused facities to increase
       * quality of results for large values of _u*_I.size().
       * Currently for such instances sometimes facilites are unused.
       */
    }
  }
}

void Solver::optimize_x ()
{
  for (Customer &c : _D) {
    c.flow_parent = nullptr;
    c.dij_parent = nullptr;
  }
  for (Facility &f : _I) {
    f.dij_parent = nullptr;
    f.outflow = 0;
    f.pi = 0;
  }
  ssp_algorithm();
}

void Solver::compute_cost()
{
  _cost = _facility_cost * _I.size();
  for (Customer &c : _D) {
    _cost += Point::dist(c, *c.flow_parent);
  }
}

void Solver::print()
{
  std::cout << "OBJECTIVE " << _cost << std::endl;
  for (unsigned i = 0; i < _I.size(); i++) {
    std::cout << "FACILITY " << i+1 << _I.at(i).show() << std::endl;
  }
  for (unsigned j = 0; j < _D.size(); j++) {
    unsigned i = _D.at(j).flow_parent - &_I.front();
    std::cout << "ASSIGN " << j+1 << " " << i+1 << std::endl;
  }
}

/*
 * The source isn't explictly part of the graph. We will store flow at the Head of an Edge,
 * for Customers in the Facility *flow_parent, as they can only have inflow 0 or 1,
 * for Facilitys in flow_t outflow (=inflow).
 * Customer will not be added to the Heap if they have outdegree 0 in the residual Graph,
 * and not be consider Vertices of the Graph at all if they have outdegree 1, we instead
 * consider lenght 2 paths which have the Customer as inner Vertex as Edges.
 */
void Solver::ssp_algorithm () {
#ifdef PRINT_ADDITIONAL_INFORMATION
  const int print_every_n_seconds = 10;
  static std::chrono::time_point<std::chrono::system_clock> last_print;
  last_print = std::chrono::system_clock::now();
#endif

  std::vector<Customer *> unsupplied;
  for (Customer &c : _D) {
    unsupplied.push_back(&c);
  }
  while (!unsupplied.empty()) {
    dij_init(unsupplied);
    dij_algorithm();
    for (Facility &f : _I) {
      f.pi -= f.dij_dist;
      f.dij_dist = 0;
    }
    increase_flow(unsupplied);

#ifdef PRINT_ADDITIONAL_INFORMATION
    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    int elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(now-last_print).count();
    if (elapsed_seconds > print_every_n_seconds) {
      std::cout << "currently unsupplied\t" << unsupplied.size() << "\tof originally\t" << _D.size() << std::endl;
      last_print = now;
    }
#endif
  }
}

void Solver::increase_flow (std::vector<Customer *> &unsupplied) {

  // tries to find an augmenting path to c in the rest of current dij_tree.
  auto try_supply = [&] (Customer *c) -> bool {
    // avoid recursion by using
    std::vector<Customer *> stack;
    stack.push_back(c);
    bool retval = false;

    while (!stack.empty()) {
      c = stack.back();
      Facility *f = c->dij_parent; // Note: f != nullptr by kind of Customers we push onto the stack.
      if (f->outflow < _u) {
	// found a way to supply the original c
	retval = true;
	f->outflow++;
	c->flow_parent = f;
	stack.pop_back();
	// also destruct dij_tree as the edge will be no longer in the residual Graph
	c->dij_parent = nullptr;
      } else if (f->dij_parent && f->dij_parent->dij_parent) {
	// can continue "recursion".
	stack.push_back(f->dij_parent);
      } else if (retval) {
	// have allready done recursion and found an augmenting path
	c->flow_parent = f;
	stack.pop_back();
	// also destruct dij_tree as edges will be no longer in the residual Graph
	f->dij_parent = nullptr;
	c->dij_parent = nullptr;
      } else {
	// can't find augmenting path. May destruct parts of dij_tree no longer connected to source.
	f->dij_parent = nullptr;
	c->dij_parent = nullptr;
	stack.pop_back();
      }
    }

    return retval;
  };

  for (flow_t i = 0; i < unsupplied.size();) {
    if (try_supply (unsupplied.at(i))) {
      std::swap (unsupplied.at(i), unsupplied.back());
      unsupplied.pop_back();
    } else {
      i++;
    }
  }
}

void Solver::dij_init(std::vector<Customer *> &unsupplied)
{
  for (Customer *c : unsupplied) {
    c->dij_dist = std::numeric_limits<double>::infinity();
  }
  for (Facility &f : _I) {
    f.dij_dist = std::numeric_limits<double>::infinity();
    f.heap_node = nullptr;
  }

  /*
   * Tries to backtrack from f to the source.
   * Adds all facilities to heap on success.
   * Removes all Edges of last dij_tree considered on failure.
   */
  auto backtrack = [&] (Facility *f) -> void {
    // avoid recursion
    std::vector<Facility *> stack;
    stack.push_back(f);
    bool retval = false;

    while (!stack.empty()) {
      f = stack.back();
      if (f->heap_node) {
        // f was allready marked as connected to source
        retval = true;
        stack.pop_back();
      } else if (retval) {
        // we have found a connection to source and f is not marked connected
        f->dij_dist = 0;
        f->heap_node = _heap.add(f);
        stack.pop_back();
      } else if (f->outflow < _u) {
        // f is connected to source
        retval = true;
        // we don't call stack.pop_back here. This is intentional, we want to repeat the loop with retval true.
      } else if (f->dij_parent && f->dij_parent->dij_parent) {
        Facility *f2 = f->dij_parent->dij_parent;
        if (f2->heap_node || f2->dij_parent || f->outflow < _u) {
          // we have not allready recursivly visited f2 from f
          stack.push_back(f2);
        } else {
          // we have allready recursivly visited f2 from f. Not connected to source
          f->dij_parent->dij_parent = nullptr;
          f->dij_parent = nullptr;
          stack.pop_back();
        }
      } else {
        // we cant backtrack further and have found no connection
        f->dij_parent = nullptr;
        stack.pop_back();
      }
    }
  };

  for (Facility &f : _I) {
    backtrack(&f);
  }
}

void Solver::dij_algorithm () {
  // Cost of the Edge from f to c.
  auto edge_cost = [&] (Facility *f, Customer *c) -> double {
    // Note c->pi is not stored as we may assume it allways is 0 (see below for why we may).
    return Point::dist(*f, *c) - f->pi;
  };

  // we may ignore customers that have an flow_parent which allready has minimal dij_dist.
  std::vector<Customer *> not_ignored_customers;
  for (Customer &c : _D) {
    if (!c.flow_parent || c.flow_parent->dij_dist != 0) {
      not_ignored_customers.push_back(&c);
    }
  }

  Facility *f;
  while (f = _heap.extract_min(), f) {
    for (Customer *c : not_ignored_customers) {
      if (c->flow_parent == f) {
	// In this case (c, f) is no edge in the residual Graph.
	// Note we don't lose any relevant runtime here.
        continue;
      } else if (c->flow_parent) {
        // In this case we imagine c to be removed from the Graph as it has b-value 0 and only 1 outgoing Edge.
	// Simply shortcut using the unique outgoing Edge to leave c.
        Facility *f2 = c->flow_parent;
	double dist = f->dij_dist + edge_cost (f, c) - edge_cost (f2, c);
        if (dist < f2->dij_dist) {
          f2->dij_dist = dist;
          f2->dij_parent = c;
          c->dij_parent = f;
          if (f2->heap_node) {
            _heap.decrease(f2->heap_node);
          } else {
            f2->heap_node = _heap.add(f2);
          }
        }
      } else {
	double dist = f->dij_dist + edge_cost(f, c);
	if (dist < c->dij_dist) {
	  c->dij_dist = dist;
	  c->dij_parent = f;
	  // No heap operations: Note c has no outgoing edges, so propagating it would be pointless.
	}
      }
    }
  }
}
