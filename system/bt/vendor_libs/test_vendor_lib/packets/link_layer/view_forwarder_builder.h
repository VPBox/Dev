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
#include <memory>

#include "base/logging.h"

#include "packets/packet_builder.h"
#include "packets/packet_view.h"

namespace test_vendor_lib {
namespace packets {

class ViewForwarderBuilder : public PacketBuilder<true> {
 public:
  virtual ~ViewForwarderBuilder() = default;

  static std::unique_ptr<ViewForwarderBuilder> Create(PacketView<true> view) {
    return std::unique_ptr<ViewForwarderBuilder>(new ViewForwarderBuilder(view));
  }

  virtual size_t size() const override {
    return view_.size();
  }

  virtual void Serialize(std::back_insert_iterator<std::vector<uint8_t>> it) const override {
    for (size_t i = 0; i < view_.size(); i++) {
      insert(view_[i], it);
    }
  }

 private:
  explicit ViewForwarderBuilder(PacketView<true> view) : view_(view) {}
  PacketView<true> view_;
};

}  // namespace packets
}  // namespace test_vendor_lib
