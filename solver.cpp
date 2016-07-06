#include "solver.hpp"
#include <iostream>
#include <functional>
#include <limits>

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
  dist_t old_cost = _cost;
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
      ; // one may put code which move unused facities to facilities with high usage here
    }
  }
}

void Solver::optimize_x ()
{
  for (Customer &c : _D) {
    c.flow_parent = nullptr;
  }
  for (Facility &f : _I) {
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
 * for Facilitys in flow_t outflow (=inflow)
 * Customer will not be added to the Heap if they have outdegree 0 in the residual Graph,
 * and not be consider Vertices of the Graph at all if they have outdegree 1, we instead
 * consider lenght 2 paths which have the Customer as inner Vertex as Edges.
 */
void Solver::ssp_algorithm () {
  flow_t pushed = 0;
  while (pushed < _D.size()) {
    dij_init();
    dij_algorithm();
    dij_compute_children();
    for (Facility &f : _I) {
      f.pi += f.dij_dist;
    }
    pushed += increase_flow();
#ifdef PRINT_ADDITIONAL_INFORMATION
    std::cout << "currently pushed\t" << pushed << "\tof\t" << _D.size() << "\tneccessary." << std::endl;
#endif
  }
}

Solver::flow_t Solver::increase_flow () {
  flow_t retval = 0;
  for (Facility &f : _I) {
    for (Customer *c : f.dij_children) {
      if (f.outflow == _u) {
        break;
      }
      if (search_sink(c)) {
        c->flow_parent = &f;
        ++f.outflow;
        ++retval;
      }
    }
  }
  return retval;
}

bool Solver::search_sink (Customer *c) {
  if (c->flow_parent) {
    for (Customer *c2 : c->flow_parent->dij_children) {
      if (search_sink(c2)) {
        c2->flow_parent = c->flow_parent;
        return true;
      }
    }
    return false;
  } else {
    return true;
  }
}

void Solver::dij_init () {
  for (Customer &c : _D) {
    c.dij_dist = std::numeric_limits<dist_t>::infinity();
    c.dij_parent = nullptr;
  }
  for (Facility &f : _I) {
    f.dij_children.clear();
    f.dij_parent = nullptr;
    if (f.outflow < _u) {
      f.dij_dist = 0;
      f.heap_node = _heap.add(&f);
    } else {
      f.dij_dist = std::numeric_limits<dist_t>::infinity();
      f.heap_node = nullptr;
    }
  }
}

void Solver::dij_algorithm () {
  Facility *f;
  while (f = _heap.extract_min(), f) {
    for (Customer &c : _D) {
      if (c.flow_parent == f) {
        continue;
      }
      dist_t dist = f->dij_dist + f->pi + Point::dist(*f, c);
      if (dist < 0) std::cout << "What\n";
      if (c.flow_parent) {
        Facility *f2 = c.flow_parent;
        dist = dist - Point::dist(c, *f2) - f2->pi;
        if (dist < f2->dij_dist) {
          f2->dij_dist = dist;
          f2->dij_parent = &c;
          c.dij_parent = f;
          if (f2->heap_node) {
            _heap.decrease(f2->heap_node);
          } else {
            f2->heap_node = _heap.add(f2);
          }
        }
      } else if (dist < c.dij_dist) {
        c.dij_dist = dist;
        c.dij_parent = f;
      }
    }
  }
}

void Solver::dij_compute_children () {
  for (Facility &f : _I) {
    if (f.dij_parent) {
      f.dij_parent->dij_parent->dij_children.push_back(f.dij_parent);
    }
  }
  for (Customer &c : _D) {
    if (c.dij_parent && !c.flow_parent) {
      c.dij_parent->dij_children.push_back(&c);
    }
  }
}
