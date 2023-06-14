#ifndef MPMC_QUEUE_H_
#define MPMC_QUEUE_H_
#include <atomic>
#include <thread>
#include <string.h>
#include <iostream>
#include <mutex>
#include <unordered_map>
#include <chrono>
#include <vector>

template <class T>
class MPMCQueue {
public:
  MPMCQueue(int queue_size) {
    while (mask_ < queue_size) mask_ *= 2;
    capacity_ = mask_;
    data_buff_ = static_cast<Node**>(calloc(mask_, sizeof(Node*)));
    for (int i = 0; i < mask_; ++i) {
      data_buff_[i] = new Node();
      data_buff_[i]->writer_version_.store(i);
      data_buff_[i]->reader_version_.store(std::numeric_limits<uint64_t>::max());
    }
    mask_--;
    fetch_start = std::chrono::system_clock::now();
  }
  MPMCQueue(const MPMCQueue& q) = delete;
  MPMCQueue operator=(const MPMCQueue& q) = delete;

  ~MPMCQueue() {
    for (auto i = 0; i < capacity_; ++i) {
      delete data_buff_[i];
    }
  }

  bool Push(T* new_data) {
    uint64_t cur_tail = tail_.load();
    if (data_buff_[cur_tail & mask_]->writer_version_.load() != cur_tail || !tail_.compare_exchange_weak(cur_tail, cur_tail + 1)) {
      return false;
    }
    data_buff_[cur_tail & mask_]->data_ = new_data;
    data_buff_[cur_tail & mask_]->reader_version_.store(cur_tail);
    return true;
  }

  T* Pop() {
    uint64_t cur_head = head_.load();
    if (data_buff_[cur_head & mask_]->reader_version_.load() != cur_head || !head_.compare_exchange_weak(cur_head, cur_head + 1)) {
      return nullptr;
    }

    T* res = data_buff_[cur_head & mask_]->data_;
    data_buff_[cur_head & mask_]->data_ = nullptr;
    data_buff_[cur_head & mask_]->writer_version_.store(cur_head + capacity_);
    return res;
  }

  inline bool reach_time_limit() {
    if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - fetch_start).count() >= 1000) {
      return true;
    }
    return false;
  }

private:
  struct Node {
    Node() : data_(nullptr), writer_version_(0), reader_version_(0) {}
    ~Node() {
      if (data_ != nullptr) {
        delete data_;
      }
    }
    T* data_;
    std::atomic<uint64_t> writer_version_;
    std::atomic<uint64_t> reader_version_;
  };
  std::atomic<uint64_t> tail_ = {0};
  std::atomic<uint64_t> head_ = {0};
  Node** data_buff_;
  int mask_ = 2;
  uint64_t capacity_;
  static constexpr uint64_t default_queue_size = 1000;
  std::chrono::system_clock::time_point fetch_start;
};
#endif
