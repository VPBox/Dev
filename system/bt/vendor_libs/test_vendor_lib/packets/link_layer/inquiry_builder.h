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

#include "inquiry.h"
#include "packets/packet_builder.h"

namespace test_vendor_lib {
namespace packets {

class InquiryBuilder : public PacketBuilder<true> {
 public:
  virtual ~InquiryBuilder() = default;

  static std::unique_ptr<InquiryBuilder> Create(Inquiry::InquiryType inquiry_type) {
    return std::unique_ptr<InquiryBuilder>(new InquiryBuilder(inquiry_type));
  }

  virtual size_t size() const override {
    return sizeof(uint8_t);
  }

 protected:
  virtual void Serialize(std::back_insert_iterator<std::vector<uint8_t>> it) const override {
    insert(static_cast<uint8_t>(inquiry_type_), it);
  }

 private:
  explicit InquiryBuilder(Inquiry::InquiryType inquiry_type) : inquiry_type_(inquiry_type) {}
  Inquiry::InquiryType inquiry_type_;
};

}  // namespace packets
}  // namespace test_vendor_lib
