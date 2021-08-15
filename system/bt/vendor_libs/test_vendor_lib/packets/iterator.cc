/*
 * Copyright 2018 The Android Open Source Project
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

#include "iterator.h"

#include <base/logging.h>

namespace test_vendor_lib {
namespace packets {

template <bool little_endian>
Iterator<little_endian>::Iterator(std::forward_list<View> data, size_t offset) {
  data_ = data;
  index_ = offset;
  length_ = 0;
  for (auto& view : data) {
    length_ += view.size();
  }
}

template <bool little_endian>
Iterator<little_endian> Iterator<little_endian>::operator+(int offset) {
  auto itr(*this);

  return itr += offset;
}

template <bool little_endian>
Iterator<little_endian>& Iterator<little_endian>::operator+=(int offset) {
  index_ += offset;
  return *this;
}

template <bool little_endian>
Iterator<little_endian> Iterator<little_endian>::operator++(int) {
  auto itr(*this);
  index_++;
  return itr;
}

template <bool little_endian>
Iterator<little_endian>& Iterator<little_endian>::operator++() {
  index_++;
  return *this;
}

template <bool little_endian>
Iterator<little_endian> Iterator<little_endian>::operator-(int offset) {
  auto itr(*this);

  return itr -= offset;
}

template <bool little_endian>
int Iterator<little_endian>::operator-(Iterator<little_endian>& itr) {
  return index_ - itr.index_;
}

template <bool little_endian>
Iterator<little_endian>& Iterator<little_endian>::operator-=(int offset) {
  index_ -= offset;

  return *this;
}

template <bool little_endian>
Iterator<little_endian> Iterator<little_endian>::operator--(int) {
  auto itr(*this);
  if (index_ != 0) index_--;

  return itr;
}

template <bool little_endian>
Iterator<little_endian>& Iterator<little_endian>::operator--() {
  if (index_ != 0) index_--;

  return *this;
}

template <bool little_endian>
Iterator<little_endian>& Iterator<little_endian>::operator=(const Iterator<little_endian>& itr) {
  data_ = itr.data_;
  index_ = itr.index_;

  return *this;
}

template <bool little_endian>
bool Iterator<little_endian>::operator==(const Iterator<little_endian>& itr) const {
  return index_ == itr.index_;
}

template <bool little_endian>
bool Iterator<little_endian>::operator!=(const Iterator<little_endian>& itr) const {
  return !(*this == itr);
}

template <bool little_endian>
bool Iterator<little_endian>::operator<(const Iterator<little_endian>& itr) const {
  return index_ < itr.index_;
}

template <bool little_endian>
bool Iterator<little_endian>::operator>(const Iterator<little_endian>& itr) const {
  return index_ > itr.index_;
}

template <bool little_endian>
bool Iterator<little_endian>::operator<=(const Iterator<little_endian>& itr) const {
  return index_ <= itr.index_;
}

template <bool little_endian>
bool Iterator<little_endian>::operator>=(const Iterator<little_endian>& itr) const {
  return index_ >= itr.index_;
}

template <bool little_endian>
uint8_t Iterator<little_endian>::operator*() const {
  CHECK(index_ < length_) << "Index " << index_ << " out of bounds: " << length_;
  size_t index = index_;

  for (auto view : data_) {
    if (index < view.size()) {
      return view[index];
    }
    index -= view.size();
  }
  CHECK(false) << "Out of fragments searching for Index " << index_;
  return 0;
}

template <bool little_endian>
size_t Iterator<little_endian>::NumBytesRemaining() const {
  if (length_ > index_) {
    return length_ - index_;
  } else {
    return 0;
  }
}

// Explicit instantiations for both types of Iterators.
template class Iterator<true>;
template class Iterator<false>;
}  // namespace packets
}  // namespace test_vendor_lib
