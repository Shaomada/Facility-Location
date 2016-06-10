#include "graph.hpp"
#include <iostream>

Graph::Graph(unsigned n_D, unsigned n_I, unsigned u)
  : _nodes(n_D+n_I+1)
  , _edges()
  , _h()
{
  for (auto it_facilities = _nodes.begin()+n_D; it_facilities < _nodes.begin()+n_D+n_I; it_facilities++) {
    for (auto it_customers = _nodes.begin(); it_customers < _nodes.begin()+n_D; it_customers++) {
      new BackwardEdge(new ForwardEdge(&*it_facilities, &*it_customers, 1));
    }
    new BackwardEdge(new ForwardEdge(&*_nodes.begin()+n_D+n_I, &*it_facilities, u));
  }
}

Graph::~Graph()
{
  for (Node n : _nodes) {
    for (Edge *e : n.outgoing_edges) {
      delete e;
    }
  }
}

void Graph::compute_optimal_assignment(const std::vector<Point> &D, const std::vector<Point> &I, std::vector<unsigned> &x)
{
  ssp_reset(D, I);
  run_successive_shortest_path();
  read_assignment(D.size(), I.size(), x);
}

void Graph::read_assignment (unsigned n_D, unsigned n_I, std::vector<unsigned> &x) {
  for (size_t i = 0; i < n_D; i++) {
    for (size_t j = 0; j < n_I; j++) {
      if (_nodes.at(i).outgoing_edges.at(j)->capacity() > 0) {
        x.at(i) = j;
        break;
      }
    }
  }
}

void Graph::decrease_dij_dist (Edge *e)
{
  if (e->is_valid()) {
    Node *n = e->head();
    double dist = e->tail()->dij_dist + e->tail()->pi + e->distance() - n->pi;
    if (dist < n->dij_dist) {
      n->dij_dist = dist;
      n->dij_parent = e;
      if (n->heap_node) {
        _h.decrease(n->heap_node);
      } else {
        n->heap_node = _h.add (n);
      }
    }
  }
}

void Graph::run_dijkstra (Node *n)
{
  // reset variables
  for (Node &m : _nodes) {
    m.dij_dist = std::numeric_limits<double>::infinity();
    m.dij_parent = nullptr;
    m.dij_children.clear();
    m.heap_node = nullptr;
  }
  // special case first visit
  n->dij_dist = 0;
  for (Edge *e : n->outgoing_edges) {
    decrease_dij_dist(e);
  }
  // all further visits
  while (n = _h.extract_min(), n) {
    n->dij_parent->tail()->dij_children.push_back(n->dij_parent);
    for (Edge *e : n->outgoing_edges) {
      decrease_dij_dist(e);
    }
  }
}

void Graph::ssp_reset (const std::vector<Point> &D, const std::vector<Point> &I)
{
  for (size_t i = 0; i < D.size(); i++) {
    for (size_t j = 0; j < I.size(); j++) {
      ForwardEdge *e = dynamic_cast<ForwardEdge *>(_nodes.at(j+D.size()).outgoing_edges.at(i));
      e->reset_flow();
      e->reset_distance(D.at(i).dist(I.at(j)));
    }
    _nodes.at(i).b = 1;
  }
  for (Edge *e : _nodes.back().outgoing_edges) {
    dynamic_cast<ForwardEdge *>(e)->reset_flow();
  }
  _nodes.back().b = -D.size();
  for (Node &n : _nodes) {
    n.pi = 0;
  }
}

int Graph::ssp_push (Node *n, int amount)
{
  int pushed = std::min (amount, std::max(0, n->b));
  n->b -= pushed;
  for (Edge *e : n->dij_children) {
    if (pushed < amount) {
      pushed += e->increase_flow (ssp_push (e->head(), std::min(amount, e->capacity())));
    } else {
      break;
    }
  }
  return pushed;
}

void Graph::run_successive_shortest_path ()
{
  while (_nodes.back().b != 0) {
    run_dijkstra(&_nodes.back());
    for (Node &n : _nodes) {
      n.pi += n.dij_dist;
    }
    _nodes.back().b += ssp_push (&_nodes.back(), -_nodes.back().b);
  }
}

#if 0
void Graph::print () {
  for (unsigned i = 0; i < _nodes.size(); i++) {
    std::cout << "\t" << i << "th Node" << "\tb=" << _nodes.at(i).b << "\toutgoing edges:" << std::endl;
    for (Edge *e : _nodes.at(i).outgoing_edges) {
      std::cout << "\t\t(" << e->tail()-&_nodes.front()
                <<     "," << e->head()-&_nodes.front()
                << ")\t" << e->capacity() << "\t" << e->is_valid() << std::endl;
    }
  }
  std::cout << std::endl;
}

void Graph::print_dij_tree (Node *n, unsigned indent)
{
  for (unsigned i = 0; i < indent; i++) {
    std::cout << "\t";
  }
  std::cout <<  n-&_nodes.front() << std::endl;
  ++indent;
  for (Edge *e : n->dij_children) {
    print_dij_tree(e->head(), indent);
  }
}

void Graph::print_dij_data () {
  for (size_t i = 0; i < _nodes.size(); i++) {
    std::cout << i << "th nodes dij data:" << std::endl;
    std::cout << "\tdij_dist: " << _nodes.at(i).dij_dist;
    std::cout << "\tdij_parent: ";
    if (_nodes.at(i).dij_parent) {
      std::cout << "(" << _nodes.at(i).dij_parent->tail()-&_nodes.front() 
                << "," << _nodes.at(i).dij_parent->head()-&_nodes.front()
                << ")" << std::endl;
    } else {
      std::cout << "none" << std::endl;
    }
  }
  std::cout << std::endl;
}

void Graph::print_dij_visit (Node *n) {
  if (n->dij_parent) {
    std::cout << "dij visit "
                << n-&_nodes.front()
              << "\tthrough ("
                << n->dij_parent->tail()-&_nodes.front() << "," 
                  << n->dij_parent->head()-&_nodes.front()
              << ")" << std::endl;
  } else {
    std::cout << "initial dij visit " << n-&_nodes.front() << std::endl;
  }
}
#endif