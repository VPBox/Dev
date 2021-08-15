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

#include "packet_view.h"

#include <algorithm>

#include <base/logging.h>

namespace test_vendor_lib {
namespace packets {

template <bool little_endian>
PacketView<little_endian>::PacketView(const std::forward_list<class View> fragments)
    : fragments_(fragments), length_(0) {
  for (auto fragment : fragments_) {
    length_ += fragment.size();
  }
}

template <bool little_endian>
PacketView<little_endian>::PacketView(std::shared_ptr<std::vector<uint8_t>> packet)
    : fragments_({View(packet, 0, packet->size())}), length_(packet->size()) {}

template <bool little_endian>
Iterator<little_endian> PacketView<little_endian>::begin() const {
  return Iterator<little_endian>(this->fragments_, 0);
}

template <bool little_endian>
Iterator<little_endian> PacketView<little_endian>::end() const {
  return Iterator<little_endian>(this->fragments_, size());
}

template <bool little_endian>
uint8_t PacketView<little_endian>::operator[](size_t index) const {
  return at(index);
}

template <bool little_endian>
uint8_t PacketView<little_endian>::at(size_t index) const {
  CHECK(index < length_) << "Index " << index << " out of bounds";
  for (const auto& fragment : fragments_) {
    if (index < fragment.size()) {
      return fragment[index];
    }
    index -= fragment.size();
  }
  CHECK(false) << "Out of fragments searching for Index " << index;
  return 0;
}

template <bool little_endian>
size_t PacketView<little_endian>::size() const {
  return length_;
}

template <bool little_endian>
std::forward_list<View> PacketView<little_endian>::SubViewList(size_t begin, size_t end) const {
  CHECK(begin <= end) << "Begin " << begin << " is past end";
  CHECK(end <= length_) << "End " << end << " is too large";
  std::forward_list<View> view_list;
  std::forward_list<View>::iterator it = view_list.before_begin();
  size_t length = end - begin;
  for (const auto& fragment : fragments_) {
    if (begin >= fragment.size()) {
      begin -= fragment.size();
    } else {
      View view(fragment, begin, begin + std::min(length, fragment.size() - begin));
      length -= view.size();
      it = view_list.insert_after(it, view);
      begin = 0;
    }
  }
  return view_list;
}

template <bool little_endian>
PacketView<true> PacketView<little_endian>::SubViewLittleEndian(size_t begin, size_t end) const {
  return PacketView<true>(SubViewList(begin, end));
}

template <bool little_endian>
PacketView<false> PacketView<little_endian>::SubViewBigEndian(size_t begin, size_t end) const {
  return PacketView<false>(SubViewList(begin, end));
}

// Explicit instantiations for both types of PacketViews.
template class PacketView<true>;
template class PacketView<false>;

}  // namespace packets
}  // namespace test_vendor_lib
