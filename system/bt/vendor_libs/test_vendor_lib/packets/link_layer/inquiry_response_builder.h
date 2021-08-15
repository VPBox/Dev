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

#include "include/inquiry.h"
#include "packets/packet_builder.h"
#include "types/class_of_device.h"

namespace test_vendor_lib {
namespace packets {

class InquiryResponseBuilder : public PacketBuilder<true> {
 public:
  virtual ~InquiryResponseBuilder() = default;

  static std::unique_ptr<InquiryResponseBuilder> CreateStandard(uint8_t page_scan_repetition_mode,
                                                                const ClassOfDevice& class_of_device,
                                                                uint16_t clock_offset) {
    return std::unique_ptr<InquiryResponseBuilder>(new InquiryResponseBuilder(
        Inquiry::InquiryType::STANDARD, page_scan_repetition_mode, class_of_device, clock_offset));
  }
  static std::unique_ptr<InquiryResponseBuilder> CreateRssi(uint8_t page_scan_repetition_mode,
                                                            const ClassOfDevice& class_of_device, uint16_t clock_offset,
                                                            uint8_t rssi) {
    return std::unique_ptr<InquiryResponseBuilder>(new InquiryResponseBuilder(
        Inquiry::InquiryType::RSSI, page_scan_repetition_mode, class_of_device, clock_offset, rssi));
  }
  static std::unique_ptr<InquiryResponseBuilder> CreateExtended(uint8_t page_scan_repetition_mode,
                                                                const ClassOfDevice& class_of_device,
                                                                uint16_t clock_offset, uint8_t rssi,
                                                                const std::vector<uint8_t>& extended_data) {
    return std::unique_ptr<InquiryResponseBuilder>(new InquiryResponseBuilder(
        Inquiry::InquiryType::EXTENDED, page_scan_repetition_mode, class_of_device, clock_offset, rssi, extended_data));
  }

  virtual size_t size() const override {
    size_t inquiry_size =
        sizeof(inquiry_type_) + sizeof(page_scan_repetition_mode_) + sizeof(class_of_device_) + sizeof(clock_offset_);
    if (inquiry_type_ == Inquiry::InquiryType::STANDARD) {
      return inquiry_size;
    }
    inquiry_size += sizeof(rssi_);
    if (inquiry_type_ == Inquiry::InquiryType::RSSI) {
      return inquiry_size;
    }

    return inquiry_size + extended_data_.size();
  }

 protected:
  virtual void Serialize(std::back_insert_iterator<std::vector<uint8_t>> it) const override {
    insert(static_cast<uint8_t>(inquiry_type_), it);
    insert(page_scan_repetition_mode_, it);
    insert_class_of_device(class_of_device_, it);
    insert(clock_offset_, it);
    if (inquiry_type_ == Inquiry::InquiryType::STANDARD) {
      return;
    }
    insert(rssi_, it);
    if (inquiry_type_ == Inquiry::InquiryType::RSSI) {
      return;
    }
    insert_vector(extended_data_, it);
  }

 private:
  Inquiry::InquiryType inquiry_type_;
  uint8_t page_scan_repetition_mode_;
  ClassOfDevice class_of_device_;
  uint16_t clock_offset_;
  uint8_t rssi_{0xff};
  std::vector<uint8_t> extended_data_;
  explicit InquiryResponseBuilder(Inquiry::InquiryType inquiry_type, uint8_t page_scan_repetition_mode,
                                  const ClassOfDevice& class_of_device, uint16_t clock_offset)
      : inquiry_type_(inquiry_type), page_scan_repetition_mode_(page_scan_repetition_mode),
        class_of_device_(class_of_device), clock_offset_(clock_offset) {}
  explicit InquiryResponseBuilder(Inquiry::InquiryType inquiry_type, uint8_t page_scan_repetition_mode,
                                  const ClassOfDevice& class_of_device, uint16_t clock_offset, uint8_t rssi)
      : inquiry_type_(inquiry_type), page_scan_repetition_mode_(page_scan_repetition_mode),
        class_of_device_(class_of_device), clock_offset_(clock_offset), rssi_(rssi) {}
  explicit InquiryResponseBuilder(Inquiry::InquiryType inquiry_type, uint8_t page_scan_repetition_mode,
                                  const ClassOfDevice& class_of_device, uint16_t clock_offset, uint8_t rssi,
                                  const std::vector<uint8_t>& extended_data)
      : inquiry_type_(inquiry_type), page_scan_repetition_mode_(page_scan_repetition_mode),
        class_of_device_(class_of_device), clock_offset_(clock_offset), rssi_(rssi), extended_data_(extended_data) {}
};

}  // namespace packets
}  // namespace test_vendor_lib
