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

#include "view.h"

#include <base/logging.h>

namespace test_vendor_lib {
namespace packets {

View::View(std::shared_ptr<const std::vector<uint8_t>> data, size_t begin, size_t end)
    : data_(data), begin_(begin < data_->size() ? begin : data_->size()),
      end_(end < data_->size() ? end : data_->size()) {}

View::View(const View& view, size_t begin, size_t end) : data_(view.data_) {
  begin_ = (begin < view.size() ? begin : view.size());
  begin_ += view.begin_;
  end_ = (end < view.size() ? end : view.size());
  end_ += view.begin_;
}

uint8_t View::operator[](size_t i) const {
  CHECK(i + begin_ < end_) << "Out of bounds access at " << i;
  return data_->operator[](i + begin_);
}

size_t View::size() const {
  return end_ - begin_;
}

}  // namespace packets
}  // namespace test_vendor_lib
