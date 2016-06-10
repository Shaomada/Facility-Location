#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "heap.hpp"
#include "point.hpp"
#include <vector>
#include <limits>
#include <functional>

class Graph {
public:
  class Node;
  class Edge;
  class ForwardEdge;
  class BackwardEdge;

  /**
   * This Graph will be used to compute all min cost flows needed in one instance of the problem.
   * Thus we only create objects and set capacities here, as distances will be different every
   * time we need to compute a new flow.
   */
  Graph(unsigned n_D, unsigned n_I, unsigned u);
  ~Graph();
  /**
   * Will compute an optimal assignment for Customer D and Facilities I and save it in x.
   * You may only call this on a Graph which was initialised with the right numbers,
   * ie with n_D = D.size() and n_I = I.size().
   */
  void compute_optimal_assignment(const std::vector<Point> &D, const std::vector<Point> &I, std::vector<unsigned> &x);
private:
  /**
   * Read the optimal assignment from the min cost flow we computed
   */
  void read_assignment (unsigned n_D, unsigned n_I, std::vector<unsigned> &x);
  /**
   * Helper function for run_dijkstra.
   * Will be called when we visit e->tail() and consider the path to e->head()
   * using e. If it's shorter than e->head()->dij_dist, the length of an shortest
   * source - e->head() - path found so far, we will update dij_dist and dij_parent
   * to save the new path and afterwards handle neccessary heap operations.
   */
  void decrease_dij_dist (Edge *e);
  /**
   * Computes a shortest path aborescens rooted at the node n,
   * saved at each node in \em dij_parent and \em dij_children.
   */
  void run_dijkstra (Node *n);

  /**
   * Prepares distances, potential, bvalues and flow for \em run_successive_shortest_path.
   */
  void ssp_reset (const std::vector<Point> &D, const std::vector<Point> &I);
  /**
   * Finds a min cost flow using the successive shortest path algorithm.
   * Note we have exactly one source and each sink has an balance of -1.
   * Thus it is a good idea to get more than one shortest path out of each
   * dijkstra call, as we do using the helper function ssp_push
   */
  void run_successive_shortest_path ();
  /** Helper function for run_successive_shortest_path.
   * Pushes as much flow as possible along the shortest path tree
   * previously computed by \em run_dijkstra.
   * Returns the amount of flow we could push through this edge.
   */
  int ssp_push (Node *n, int amount);

#if 0
  /// for debugging purposes
  ///@{
  void print ();
  void print_dij_tree (Node *n, unsigned indent = 0);
  void print_dij_data ();
  void print_dij_visit (Node *n);
  ///@}
#endif
private:
  std::vector<Node> _nodes;
  std::vector<Edge> _edges;
  Heap<Node> _h;
};

/// A Node in the residual Graph.
class Graph::Node {
public:
  inline Node () : outgoing_edges(), dij_children() { b = 0;}
  std::vector<Edge *> outgoing_edges;
  double dij_dist;
  double pi;
  int b;
  Edge *dij_parent;
  std::vector<Edge *> dij_children;
  Heap<Node>::Node *heap_node;
public:
  inline bool operator<(const Node &rhs) { return dij_dist < rhs.dij_dist; }
};

/// Any Edge in the residual Graph.
class Graph::Edge {
public:
  ~Edge() { }
  /// True if the Edge exists in the residual Graph for current flow.
  inline bool is_valid () const { return capacity() > 0; }
  /// The Node in which the Edge ends.
  virtual Node *head () const = 0;
  /// The Node in which the Edge starts.
  virtual Node *tail () const = 0;
  /// The capacity of the Edge in the residual Graph.
  virtual int capacity () const = 0;
  /// The \em unreduced cost of the residual Edge.
  virtual double distance() const = 0;
  /// Increase_flow flow by and returns \param amount.
  virtual int increase_flow (int amount) = 0; // returning amount
};

/// A forward Edge in the residual Graph.
class Graph::ForwardEdge : public Edge {
public:
  ForwardEdge(Node *tail, Node *head, int capacity) : _head(head), _tail(tail)
  , _capacity(capacity), _distance(0) { this->_tail->outgoing_edges.push_back(this); }
  inline Node *head () const { return _head; }
  inline Node *tail () const { return _tail; }
  inline int capacity () const { return _capacity-_flow; }
  inline double distance() const { return _distance; }
  inline int increase_flow (int amount) { _flow += amount; return amount; }
  inline void reset_flow () { _flow = 0; }
  /// to set the cost new depending on the current position of Customer and  Facility and Facility
  inline void reset_distance (double distance) { _distance = distance; }
private:
  friend class BackwardEdge;
  Node *_head;
  Node *_tail;
  int _capacity;
  double _distance;
  int _flow;
};

/// A backward Edge in the residual Graph.
class Graph::BackwardEdge : public Edge {
public:
  BackwardEdge (ForwardEdge *e) : _e(e) { _e->_head->outgoing_edges.push_back(this); }
  inline double distance() const { return -_e->_distance; }
  inline Node *head () const { return _e->_tail; }
  inline Node *tail () const { return _e->_head; }
  inline int capacity () const { return _e->_flow; }
  inline int increase_flow (int amount) { _e->_flow-=amount; return amount; }
private:
  ForwardEdge *_e;
};

#endif