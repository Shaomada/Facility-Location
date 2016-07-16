#ifndef HEAP_HPP
#define HEAP_HPP

#include <vector>

/**
 * A standard fibbonacciheap with minimal keys as additional feature.
 * Requires T::operator< to work.
 */
template <typename T>
class Heap {
public:
  class Node;
  Heap ();
  Node *add (T *content);
  void decrease (Node *n);
  T *extract_min();
  void free_zeros();
private:
  /// helper for iterating along the path in decrease
  void iter(Node *n);
  void plant(Node *n);
  std::vector<Node *> roots;
  /// saves Nodes with minimal possbile key in an extra data stucture
  std::vector<Node *> zeros;
};

template <typename T>
void Heap<T>::free_zeros() {
  while (zeros.size() > 0) {
    delete zeros.back();
    zeros.pop_back();
  }
}

template <typename T>
class Heap<T>::Node {
  friend class Heap<T>;
private:
  Node (T *t);
  /// adds an Edge from this to n
  void push (Node *n);
  /// removes the Edge from parent to this
  void pop();
  bool operator<(const Node &other) { return *content < *other.content; }
   
  T *content;
  bool phi;
  /// number of children
  unsigned size;
  /// parent in the branching or nullptr if root
  Node *parent;
  /// as the childs of this vertex are saved in a recursive list, we'll only need the first
  Node *first;
  /// neighbours in the recursive list this is saved in, or meaningless if this is a root
  ///@{
  Node *next;
  Node *prev;
  ///@}
};

template <typename T>
Heap<T>::Node::Node (T *t)
: content(t)
, phi(false)
, size(0)
, parent(nullptr)
, first(nullptr)
, next(nullptr)
, prev(nullptr)
{ }

template <typename T>
void Heap<T>::Node::push (Node *n)
{
  n->parent = this;
  n->prev = nullptr;
  n->next = first;
  first = n;
  if (n->next) {
    n->next->prev = n;
  }
  ++size;
}

template <typename T>
void Heap<T>::Node::pop ()
{
  if (prev) {
    prev->next = next;
  } else {
    parent->first = next;
  }
  if (next) {
    next->prev = prev;
  }
  prev = nullptr;
  next = nullptr;
  parent->size--;
  parent = nullptr;
}

template <typename T>
Heap<T>::Heap() : roots(), zeros() {}

template <typename T>
typename Heap<T>::Node *Heap<T>::add (T *content) {
  Node *n = new Node(content);
  if (content->is_minimal_possible()) {
    zeros.push_back(n);
  } else {
    plant(n);
  }
  return n;
}

template <typename T>
void Heap<T>::iter (Node *n)
{
  if (!n->parent) {
    // n->size was decreased in previous step of iter/decrease
    roots.at(1+n->size) = nullptr;
    plant(n);
  } else {
    if (n->phi) {
      Node *parent = n->parent;
      n->pop();
      iter(parent);
      plant(n);
    } else {
      n->phi = true;
    }
  }
}

template <typename T>
void Heap<T>::decrease (Node *n)
{
  if (n->parent) {
    Node *parent = n->parent;
    n->pop();
    iter(parent);
    plant(n);
  }
}

template <typename T>
T *Heap<T>::extract_min()
{
  if (!zeros.empty()) {
    T *retval = zeros.back()->content;
    delete zeros.back();
    zeros.pop_back();
    return retval;
  }
  Node *min = nullptr;
  for (Node *n : roots) {
    if (n && (!min || *n < *min)) {
      min = n;
    }
  }
  if (!min) {
    return nullptr;
  }
  roots.at(min->size) = nullptr;
  Node *next;
  for (Node *n = min->first; n != nullptr; n = next) {
    next = n->next;
    n->parent = nullptr;
    n->next = nullptr;
    n->prev = nullptr;
    plant(n);
  }
  T *retval = min->content;
  delete min;
  return retval;
}

template <typename T>
void Heap<T>::plant (Node *n) {
  n->phi = false;
  unsigned d = n->size;
  if (roots.size() == d) {
    roots.push_back(n);
  } else if (!roots.at(d)) {
    roots.at(d) = n;
  } else {
    Node *m = roots.at(d);
    roots.at(d) = nullptr;
    if (*n < *m) {
      n->push(m);
      plant(n);
    } else {
      m->push(n);
      plant(m);
    }
  }
}
#endif
