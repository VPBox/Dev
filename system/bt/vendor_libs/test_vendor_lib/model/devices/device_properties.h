/*
 * Copyright 2015 The Android Open Source Project
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
#include <string>
#include <vector>

#include "base/json/json_value_converter.h"
#include "types/address.h"
#include "types/class_of_device.h"

namespace test_vendor_lib {

class DeviceProperties {
 public:
  explicit DeviceProperties(const std::string& file_name = "");

  // Access private configuration data

  // Specification Version 4.2, Volume 2, Part E, Section 7.4.1
  const std::vector<uint8_t>& GetVersionInformation() const;

  // Specification Version 4.2, Volume 2, Part E, Section 7.4.2
  const std::vector<uint8_t>& GetSupportedCommands() const {
    return supported_commands_;
  }

  // Specification Version 4.2, Volume 2, Part E, Section 7.4.3
  uint64_t GetSupportedFeatures() const {
    return extended_features_[0];
  }

  void SetSupportedFeatures(uint64_t features) {
    extended_features_[0] = features;
  }

  // Specification Version 4.2, Volume 2, Part E, Section 7.4.4
  uint8_t GetExtendedFeaturesMaximumPageNumber() const {
    return extended_features_.size() - 1;
  }

  uint64_t GetExtendedFeatures(uint8_t page_number) const {
    CHECK(page_number < extended_features_.size());
    return extended_features_[page_number];
  }

  // Specification Version 4.2, Volume 2, Part E, Section 7.4.5
  uint16_t GetAclDataPacketSize() const {
    return acl_data_packet_size_;
  }

  uint8_t GetSynchronousDataPacketSize() const {
    return sco_data_packet_size_;
  }

  uint16_t GetTotalNumAclDataPackets() const {
    return num_acl_data_packets_;
  }

  uint16_t GetTotalNumSynchronousDataPackets() const {
    return num_sco_data_packets_;
  }

  const Address& GetAddress() const {
    return address_;
  }

  void SetAddress(const Address& address) {
    address_ = address;
  }

  // Specification Version 4.2, Volume 2, Part E, Section 7.4.8
  const std::vector<uint8_t>& GetSupportedCodecs() const {
    return supported_codecs_;
  }

  const std::vector<uint32_t>& GetVendorSpecificCodecs() const {
    return vendor_specific_codecs_;
  }

  uint8_t GetVersion() const {
    return version_;
  }

  uint16_t GetRevision() const {
    return revision_;
  }

  uint8_t GetLmpPalVersion() const {
    return lmp_pal_version_;
  }

  uint16_t GetLmpPalSubversion() const {
    return lmp_pal_subversion_;
  }

  uint16_t GetManufacturerName() const {
    return manufacturer_name_;
  }

  uint8_t GetAuthenticationEnable() const {
    return authentication_enable_;
  }

  void SetAuthenticationEnable(uint8_t enable) {
    authentication_enable_ = enable;
  }

  ClassOfDevice GetClassOfDevice() const {
    return class_of_device_;
  }

  void SetClassOfDevice(uint8_t b0, uint8_t b1, uint8_t b2) {
    class_of_device_.cod[0] = b0;
    class_of_device_.cod[1] = b1;
    class_of_device_.cod[2] = b2;
  }

  void SetClassOfDevice(uint32_t class_of_device) {
    class_of_device_.cod[0] = class_of_device & 0xff;
    class_of_device_.cod[1] = (class_of_device >> 8) & 0xff;
    class_of_device_.cod[2] = (class_of_device >> 16) & 0xff;
  }

  void SetName(const std::vector<uint8_t>& name) {
    name_ = name;
  }

  const std::vector<uint8_t>& GetName() const {
    return name_;
  }

  void SetExtendedInquiryData(const std::vector<uint8_t>& eid) {
    extended_inquiry_data_ = eid;
  }

  const std::vector<uint8_t>& GetExtendedInquiryData() const {
    return extended_inquiry_data_;
  }

  uint8_t GetPageScanRepetitionMode() const {
    return page_scan_repetition_mode_;
  }

  void SetPageScanRepetitionMode(uint8_t mode) {
    page_scan_repetition_mode_ = mode;
  }

  uint16_t GetClockOffset() const {
    return clock_offset_;
  }

  void SetClockOffset(uint16_t offset) {
    clock_offset_ = offset;
  }

  // Low-Energy functions
  const Address& GetLeAddress() const {
    return le_address_;
  }

  void SetLeAddress(const Address& address) {
    le_address_ = address;
  }

  uint8_t GetLeAddressType() const {
    return le_address_type_;
  }

  void SetLeAddressType(uint8_t addr_type) {
    le_address_type_ = addr_type;
  }

  uint8_t GetLeAdvertisementType() const {
    return le_advertisement_type_;
  }

  void SetLeAdvertisementType(uint8_t ad_type) {
    le_advertisement_type_ = ad_type;
  }

  void SetLeAdvertisement(const std::vector<uint8_t>& ad) {
    le_advertisement_ = ad;
  }

  const std::vector<uint8_t>& GetLeAdvertisement() const {
    return le_advertisement_;
  }

  void SetLeScanResponse(const std::vector<uint8_t>& response) {
    le_scan_response_ = response;
  }

  const std::vector<uint8_t>& GetLeScanResponse() const {
    return le_scan_response_;
  }

  // Specification Version 4.2, Volume 2, Part E, Section 7.8.2
  uint16_t GetLeDataPacketLength() const {
    return le_data_packet_length_;
  }

  uint8_t GetTotalNumLeDataPackets() const {
    return num_le_data_packets_;
  }

  // Specification Version 4.2, Volume 2, Part E, Section 7.8.3
  uint64_t GetLeSupportedFeatures() const {
    return le_supported_features_;
  }

  void SetLeSupportedFeatures(uint64_t features) {
    le_supported_features_ = features;
  }

  // Specification Version 4.2, Volume 2, Part E, Section 7.8.14
  uint8_t GetLeWhiteListSize() const {
    return le_white_list_size_;
  }

  // Specification Version 4.2, Volume 2, Part E, Section 7.8.27
  uint64_t GetLeSupportedStates() const {
    return le_supported_states_;
  }

  // Vendor-specific commands
  const std::vector<uint8_t>& GetLeVendorCap() const {
    return le_vendor_cap_;
  }

  static void RegisterJSONConverter(base::JSONValueConverter<DeviceProperties>* converter);

 private:
  // Classic
  uint16_t acl_data_packet_size_;
  uint8_t sco_data_packet_size_;
  uint16_t num_acl_data_packets_;
  uint16_t num_sco_data_packets_;
  uint8_t version_;
  uint16_t revision_;
  uint8_t lmp_pal_version_;
  uint16_t manufacturer_name_;
  uint16_t lmp_pal_subversion_;
  uint64_t supported_features_;
  uint8_t authentication_enable_;
  std::vector<uint8_t> supported_codecs_;
  std::vector<uint32_t> vendor_specific_codecs_;
  std::vector<uint8_t> supported_commands_;
  std::vector<uint64_t> extended_features_{{0x875b3fd8fe8ffeff, 0x07}};
  ClassOfDevice class_of_device_{{0, 0, 0}};
  std::vector<uint8_t> extended_inquiry_data_;
  std::vector<uint8_t> name_;
  Address address_;
  uint8_t page_scan_repetition_mode_;
  uint16_t clock_offset_;

  // Low Energy
  uint16_t le_data_packet_length_;
  uint8_t num_le_data_packets_;
  uint8_t le_white_list_size_;
  uint64_t le_supported_features_{0x075b3fd8fe8ffeff};
  uint64_t le_supported_states_;
  std::vector<uint8_t> le_vendor_cap_;
  Address le_address_;
  uint8_t le_address_type_;
  uint8_t le_advertisement_type_;
  std::vector<uint8_t> le_advertisement_;
  std::vector<uint8_t> le_scan_response_;
};

}  // namespace test_vendor_lib
