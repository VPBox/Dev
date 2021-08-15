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

#include "packet.h"

namespace bluetooth {

Iterator::Iterator(std::shared_ptr<const Packet> packet, size_t i) {
  packet_ = packet;
  index_ = i;

  CHECK_GE(index_, packet->packet_start_index_);
  CHECK_LE(index_, packet->packet_end_index_);
}

Iterator::Iterator(const Iterator& itr) { *this = itr; }

Iterator Iterator::operator+(size_t offset) {
  auto itr(*this);

  return itr += offset;
}

Iterator& Iterator::operator+=(size_t offset) {
  size_t new_offset = index_ + offset;
  index_ = new_offset > packet_->packet_end_index_ ? packet_->packet_end_index_
                                                   : new_offset;
  return *this;
}

Iterator Iterator::operator++(int) {
  auto itr(*this);
  index_++;

  if (index_ > packet_->packet_end_index_) index_ = packet_->packet_end_index_;

  return itr;
}

Iterator& Iterator::operator++() {
  index_++;

  if (index_ > packet_->packet_end_index_) index_ = packet_->packet_end_index_;

  return *this;
}

Iterator Iterator::operator-(size_t offset) {
  auto itr(*this);

  return itr -= offset;
}

int Iterator::operator-(const Iterator& itr) { return index_ - itr.index_; }

Iterator& Iterator::operator-=(size_t offset) {
  index_ = (index_ < offset || index_ - offset < packet_->packet_start_index_)
               ? packet_->packet_start_index_
               : index_ - offset;

  return *this;
}

Iterator Iterator::operator--(int) {
  auto itr(*this);
  if (index_ != packet_->packet_start_index_) index_--;

  return itr;
}

Iterator& Iterator::operator--() {
  if (index_ != packet_->packet_start_index_) index_--;

  return *this;
}

Iterator& Iterator::operator=(const Iterator& itr) {
  packet_ = itr.packet_;
  index_ = itr.index_;

  return *this;
}

bool Iterator::operator==(const Iterator& itr) const {
  return ((packet_ == itr.packet_) && (index_ == itr.index_));
}

bool Iterator::operator!=(const Iterator& itr) const { return !(*this == itr); }

bool Iterator::operator<(const Iterator& itr) const {
  return ((packet_ == itr.packet_) && (index_ < itr.index_));
}

bool Iterator::operator>(const Iterator& itr) const {
  return ((packet_ == itr.packet_) && (index_ > itr.index_));
}

bool Iterator::operator<=(const Iterator& itr) const {
  return ((packet_ == itr.packet_) && (index_ <= itr.index_));
}

bool Iterator::operator>=(const Iterator& itr) const {
  return ((packet_ == itr.packet_) && (index_ >= itr.index_));
}

uint8_t Iterator::operator*() const {
  CHECK_NE(index_, packet_->packet_end_index_);

  return packet_->get_at_index(index_);
}

}  // namespace bluetooth