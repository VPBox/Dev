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

#pragma once

#include <cstdint>
#include <vector>

namespace test_vendor_lib {
namespace packets {

// Base class that holds a shared pointer to data with bounds.
class View {
 public:
  View(std::shared_ptr<const std::vector<uint8_t>> data, size_t begin, size_t end);
  View(const View& view, size_t begin, size_t end);
  View(const View& view) = default;
  virtual ~View() = default;

  uint8_t operator[](size_t i) const;

  size_t size() const;

 private:
  std::shared_ptr<const std::vector<uint8_t>> data_;
  size_t begin_;
  size_t end_;
};

}  // namespace packets
}  // namespace test_vendor_lib
