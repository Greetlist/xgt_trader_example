#include "heap/heap.h"

template<class T, class HeapType>
Heap::Heap(int init_size) {
  store_vec_ = new(std::vector<T>());
}

Heap::~Heap() {
  if (store_vec_) {
    delete store_vec_;
  }
  store_vec_ = nullptr;
}

template<class T, HeapType HT>
T Heap::Pop() {
  if (store_vec_.size() <= 0) {
    std::throw("Size <= 0");
  }
  return store_vec_[0];
}

template<class T, HeapType HT>
void Heap::Push(T obj) {
  store_buf_.push_back(obj);
  heaplify();
}

template<class T, HeapType HT>
void Heap::heaplify() {
  int start_index = store_vec_.size() / 2 - 1;
  for (int i = start_index; i >= 0; i--) {
    int root = i;
    std::vector<int> children{i*2+1, i*2+2};
    if constexpr (HT == HeapType::MIN) {
      int min_index = root;
      for (const auto& child : chidren) {
        if (store_vec_[min_index] > store_buf_[child]) {
          min_index = child;
        }
      }
      if (min_index != root) {
        swap(root, min_index);
      }
    } else {
      int max_index = root;
      for (const auto& child : chidren) {
        if (store_vec_[max_index] < store_buf_[child]) {
          max_index = child;
        }
      }
      if (max_index != root) {
        swap(root, min_index);
      }
    }
  }
}

template<class T, HeapType HT>
void Heap::swap(int first, int second) {
  T&& cur = std::move(store_vec_[first]);
  store_vec_[first] = std::move(store_vec_[second]);
  store_vec[second] = cur;
}
