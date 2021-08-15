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

#define LOG_TAG "device_properties"

#include "device_properties.h"

#include <memory>

#include <base/logging.h>
#include "base/files/file_util.h"
#include "base/json/json_reader.h"
#include "base/values.h"

#include "hci.h"
#include "osi/include/log.h"
#include "osi/include/osi.h"

using std::vector;

namespace {
// Functions used by JSONValueConverter to read stringified JSON.
bool ParseUint8t(base::StringPiece value, uint8_t* field) {
  *field = std::stoi(value.as_string());
  return true;
}

bool ParseUint16t(base::StringPiece value, uint16_t* field) {
  *field = std::stoi(value.as_string());
  return true;
}

}  // namespace

namespace test_vendor_lib {

DeviceProperties::DeviceProperties(const std::string& file_name)
    : acl_data_packet_size_(1024), sco_data_packet_size_(255), num_acl_data_packets_(10), num_sco_data_packets_(10),
      version_(static_cast<uint8_t>(hci::Version::V4_1)), revision_(0),
      lmp_pal_version_(static_cast<uint8_t>(hci::Version::V4_1)), manufacturer_name_(0), lmp_pal_subversion_(0),
      le_data_packet_length_(27), num_le_data_packets_(15), le_white_list_size_(15) {
  std::string properties_raw;

  CHECK(Address::FromString("BB:BB:BB:BB:BB:AD", address_));
  CHECK(Address::FromString("BB:BB:BB:BB:AD:1E", le_address_));
  name_ = {'D', 'e', 'f', 'a', 'u', 'l', 't'};

  supported_codecs_ = {0};  // Only SBC is supported.
  vendor_specific_codecs_ = {};

  for (int i = 0; i < 64; i++) supported_commands_.push_back(0xff);

  le_supported_features_ = 0x1f;
  le_supported_states_ = 0x3ffffffffff;
  le_vendor_cap_ = {};

  if (file_name.size() == 0) {
    return;
  }
  LOG_INFO(LOG_TAG, "Reading controller properties from %s.", file_name.c_str());
  if (!base::ReadFileToString(base::FilePath(file_name), &properties_raw)) {
    LOG_ERROR(LOG_TAG, "Error reading controller properties from file.");
    return;
  }

  std::unique_ptr<base::Value> properties_value_ptr = base::JSONReader::Read(properties_raw);
  if (properties_value_ptr.get() == nullptr)
    LOG_INFO(LOG_TAG, "Error controller properties may consist of ill-formed JSON.");

  // Get the underlying base::Value object, which is of type
  // base::Value::TYPE_DICTIONARY, and read it into member variables.
  base::Value& properties_dictionary = *(properties_value_ptr.get());
  base::JSONValueConverter<DeviceProperties> converter;

  if (!converter.Convert(properties_dictionary, this))
    LOG_INFO(LOG_TAG, "Error converting JSON properties into Properties object.");
}

// static
void DeviceProperties::RegisterJSONConverter(base::JSONValueConverter<DeviceProperties>* converter) {
// TODO(dennischeng): Use RegisterIntField() here?
#define REGISTER_UINT8_T(field_name, field) \
  converter->RegisterCustomField<uint8_t>(field_name, &DeviceProperties::field, &ParseUint8t);
#define REGISTER_UINT16_T(field_name, field) \
  converter->RegisterCustomField<uint16_t>(field_name, &DeviceProperties::field, &ParseUint16t);
  REGISTER_UINT16_T("AclDataPacketSize", acl_data_packet_size_);
  REGISTER_UINT8_T("ScoDataPacketSize", sco_data_packet_size_);
  REGISTER_UINT16_T("NumAclDataPackets", num_acl_data_packets_);
  REGISTER_UINT16_T("NumScoDataPackets", num_sco_data_packets_);
  REGISTER_UINT8_T("Version", version_);
  REGISTER_UINT16_T("Revision", revision_);
  REGISTER_UINT8_T("LmpPalVersion", lmp_pal_version_);
  REGISTER_UINT16_T("ManufacturerName", manufacturer_name_);
  REGISTER_UINT16_T("LmpPalSubversion", lmp_pal_subversion_);
#undef REGISTER_UINT8_T
#undef REGISTER_UINT16_T
}

}  // namespace test_vendor_lib
