#ifndef HEAP_HPP
#define HEAP_HPP

#include <vector>

typedef __SIZE_TYPE__ size_t;

template <typename T>
class Heap {
public:
  class Node {
  public:
    Node (T *content, size_t pos)
      : content(content), pos(pos) {}
    T *content;
    size_t pos;
  };
  Heap () : _heap() {};
  ~Heap () { for (Node *n : _heap) delete n; }
  Node *add (T *content);
  void decrease (Node *n);
  T *extract_min();

  std::vector<Node *> get_heap () {
    return _heap;
  }
  unsigned size () const {
    return _heap.size();
  }
private:
  std::vector<Node *> _heap;
  
  bool cmp (const Node *n, const Node *m) const {
    return *n->content < *m->content;
  }
};

template <typename T>
typename Heap<T>::Node *Heap<T>::add (T *content) {
  size_t i, j = _heap.size();
  Node *n = new Node(content, j);
  _heap.push_back(n);
  while (i=j,j=(i-1)/2,i>0&&cmp(_heap.at(i),_heap.at(j))) {
    std::swap(_heap.at(i), _heap.at(j));
    std::swap(_heap.at(i)->pos, _heap.at(j)->pos);
  }
  return n;
}

template <typename T>
void Heap<T>::decrease (Node *n) {
  if (n == nullptr) {
    return; // TODO: find out why this happens
  }
  size_t i, j = n->pos;
  while ((i = j) && cmp(_heap.at(i), _heap.at((j = (i-1)/2)))) {
    std::swap(_heap.at(i), _heap.at(j));
    std::swap(_heap.at(i)->pos, _heap.at(j)->pos);
  }
}

template <typename T>
T *Heap<T>::extract_min () {
  if (_heap.size() == 0) {
    return nullptr;
  }
  T *minimal = _heap.front()->content;
  delete _heap.front();
  _heap.front() = _heap.back();
  _heap.pop_back();
  if (_heap.size() > 0) {
    _heap.front()->pos = 0;
    size_t i = 0;
    while(true) {
      size_t min_index = i;
      if (2*i+1 < _heap.size() && cmp(_heap.at(2*i+1), _heap.at(min_index))) {
        min_index = 2*i+1;
      }
      if (2*i+2 < _heap.size() && cmp(_heap.at(2*i+2), _heap.at(min_index))) {
        min_index = 2*i+2;
      }
      if (i == min_index) {
        break;
      }
      else {
        std::swap (_heap.at(i), _heap.at(min_index));
        std::swap (_heap.at(i)->pos, _heap.at(min_index)->pos);
        i = min_index;
      }
    }
  }
  return minimal;
}

#endif