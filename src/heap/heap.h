#ifndef __HEAP_H_
#define __HEAP_H_

#include <vector>

enum class HeapType {
  MIN = 1,
  MAX = 2,
};

template <class T, class HeapType>
class Heap {
public:
  Heap();
  ~Heap();
  T& Pop();
  void Push(T);
private:
  void heaplify();
  void swap(int first, int second);
  std::vector<T>* store_vec_;
};

#endif
