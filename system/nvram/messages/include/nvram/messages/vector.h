/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef NVRAM_MESSAGES_VECTOR_H_
#define NVRAM_MESSAGES_VECTOR_H_

extern "C" {
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
}

#include <new>

#include <nvram/messages/compiler.h>

namespace nvram {

// A bare-bones dynamically-sized array container, similar to std::vector.
//
// This class is intended for use in restricted environments where the C++
// standard library is not available. Prefer std::vector wherever possible.
template <typename ElementType> class Vector {
 public:
  Vector() = default;
  ~Vector() {
    for (size_t i = 0; i < size_; ++i) {
      data_[i].~ElementType();
    }
    free(data_);
    data_ = nullptr;
    size_ = 0;
    capacity_ = 0;
  }

  // Vector is not copyable as this would require memory allocations that may
  // fail. However, Vector supports move semantics.
  Vector(const Vector<ElementType>& other) = delete;
  Vector<ElementType>& operator=(const Vector<ElementType>& other) = delete;
  Vector(Vector<ElementType>&& other) : Vector() {
    swap(*this, other);
  }
  Vector<ElementType>& operator=(Vector<ElementType>&& other) {
    swap(*this, other);
    return *this;
  }
  friend void swap(Vector<ElementType>& first, Vector<ElementType>& second) {
    // This does not use std::swap since it needs to work in environments that
    // are lacking a standard library.
    ElementType* tmp_data = first.data_;
    size_t tmp_size = first.size_;
    first.data_ = second.data_;
    first.size_ = second.size_;
    second.data_ = tmp_data;
    second.size_ = tmp_size;
  }

  ElementType& operator[](size_t pos) {
    NVRAM_CHECK(pos < size_);
    return data_[pos];
  }
  const ElementType& operator[](size_t pos) const {
    NVRAM_CHECK(pos < size_);
    return data_[pos];
  }

  ElementType* begin() { return data_; }
  ElementType* end() { return data_ + size_; }

  const ElementType* begin() const { return data_; }
  const ElementType* end() const { return data_ + size_; }

  size_t size() const { return size_; }

  // Resizes the Vector. Truncates if |size| decreases. Pads the Vector with
  // value-constructed entries if |size| increases.
  bool Resize(size_t size) NVRAM_WARN_UNUSED_RESULT {
    // Check for capacity change.
    size_t new_capacity = capacity_;
    if (size < capacity_ / 2) {
      new_capacity = size;
    } else if (size > capacity_) {
      new_capacity = capacity_ * 2 > size ? capacity_ * 2 : size;
    }
    NVRAM_CHECK(new_capacity >= size);

    // Allocate new memory if necessary.
    ElementType* new_data = nullptr;
    if (new_capacity != capacity_) {
      if (new_capacity == 0) {
        new_data = nullptr;
      } else {
        new_data = static_cast<ElementType*>(
            calloc(new_capacity, sizeof(ElementType)));
        if (!new_data) {
          return false;
        }
      }
    } else {
      new_data = data_;
    }

    size_t min_size = (size < size_) ? size : size_;
    if (new_data != data_) {
      // Move elements that remain valid.
      for (size_t i = 0; i < min_size; ++i) {
        new (&new_data[i]) ElementType(static_cast<ElementType&&>(data_[i]));
      }
    }

    // Destroy elements that are no longer part of the list.
    for (size_t i = min_size; i < size_; ++i) {
      data_[i].~ElementType();
    }

    // Construct new elements that got appended.
    for (size_t i = min_size; i < size; ++i) {
      new (&new_data[i]) ElementType;
    }

    if (new_data != data_) {
      free(data_);
    }
    data_ = new_data;
    capacity_ = new_capacity;
    size_ = size;
    return true;
  }

  // Appends an element.
  bool Append(const ElementType& element) NVRAM_WARN_UNUSED_RESULT {
    if (!Resize(size_ + 1)) {
      return false;
    }
    data_[size_ - 1] = element;
    return true;
  }

  // Rvalue-reference version of Append.
  bool Append(ElementType&& element) NVRAM_WARN_UNUSED_RESULT {
    if (!Resize(size_ + 1)) {
      return false;
    }
    data_[size_ - 1] = element;
    return true;
  }

 private:
  size_t size_ = 0;
  size_t capacity_ = 0;
  ElementType* data_ = nullptr;
};

}  // namespace nvram

#endif  // NVRAM_MESSAGES_VECTOR_H_
