/******************************************************************************
 *
 *  Copyright 2016 Google, Inc.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/
#pragma once

#include <memory>
#include <mutex>
#include <queue>

namespace bluetooth {

namespace common {

/*
 *   LeakyBondedQueue<T>
 *
 * - LeakyLondedQueue<T> is a fixed size queue that leaks oldest item when
 *   reaching its capacity. This is useful in creating memory bonded data
 *   structure where freshness is more important than full coverage.
 * - The queue is protected by a simple mutex and is thread-safe, although
 *   improvements could be made to lock enqueue and dequeue separately, it
 *   is not implemented at this moment due to lack of demand
 * - The queue uses unique_ptr to automatically free its content when it is
 *   destructed. It is the user's responsibility to implement T's destructor
 *   correctly.
 *
 */
template <class T>
class LeakyBondedQueue {
 public:
  LeakyBondedQueue(size_t capacity);
  /* Default destructor
   *
   * Call Clear() and free the queue structure itself
   */
  ~LeakyBondedQueue();
  /*
   * Add item NEW_ITEM to the underlining queue. If the queue is full, pop
   * the oldest item
   */
  void Enqueue(T* new_item);
  /*
   * Add item NEW_ITEM to the underlining queue. If the queue is full, dequeue
   * the oldest item and returns it to the caller. Return nullptr otherwise.
   */
  T* EnqueueWithPop(T* new_item);
  /*
   * Dequeues the oldest item from the queue. Return nullptr if queue is empty
   */
  T* Dequeue();
  /*
   * Returns the length of queue
   */
  size_t Length();
  /*
   * Returns the defined capacity of the queue
   */
  size_t Capacity();
  /*
   * Returns whether the queue is empty
   */
  bool Empty();
  /*
   * Pops all items from the queue
   */
  void Clear();

 private:
  // Put item in unique_ptr so that they get freed automatically when poped or
  // when queue_ is freed
  std::queue<std::unique_ptr<T>> queue_;
  std::mutex lock_;
  size_t capacity_;
};

/*
 * Definitions must be in the header for template classes
 */

template <class T>
LeakyBondedQueue<T>::LeakyBondedQueue(size_t capacity) {
  capacity_ = capacity;
}

template <class T>
LeakyBondedQueue<T>::~LeakyBondedQueue() {}

template <class T>
void LeakyBondedQueue<T>::Enqueue(T* new_item) {
  std::lock_guard<std::mutex> lock(lock_);
  if ((queue_.size() + 1) > capacity_) {
    queue_.pop();
  }
  std::unique_ptr<T> item_ptr(new_item);
  queue_.push(std::move(item_ptr));
}

template <class T>
T* LeakyBondedQueue<T>::EnqueueWithPop(T* new_item) {
  std::lock_guard<std::mutex> lock(lock_);
  T* old_item = nullptr;
  if ((queue_.size() + 1) > capacity_) {
    std::unique_ptr<T> item = std::move(queue_.front());
    queue_.pop();
    old_item = item.release();
  }
  std::unique_ptr<T> item_ptr(new_item);
  queue_.push(std::move(item_ptr));
  return old_item;
}

template <class T>
T* LeakyBondedQueue<T>::Dequeue() {
  std::lock_guard<std::mutex> lock(lock_);
  std::unique_ptr<T> item = std::move(queue_.front());
  queue_.pop();
  return item.release();
}

template <class T>
void LeakyBondedQueue<T>::Clear() {
  std::lock_guard<std::mutex> lock(lock_);
  while (!queue_.empty()) {
    // unique_ptr does not need to be freed
    queue_.pop();
  }
}

template <class T>
size_t LeakyBondedQueue<T>::Length() {
  std::lock_guard<std::mutex> lock(lock_);
  return queue_.size();
}

template <class T>
size_t LeakyBondedQueue<T>::Capacity() {
  return capacity_;
}

template <class T>
bool LeakyBondedQueue<T>::Empty() {
  std::lock_guard<std::mutex> lock(lock_);
  return queue_.empty();
}

}  // namespace common

}  // namespace bluetooth
