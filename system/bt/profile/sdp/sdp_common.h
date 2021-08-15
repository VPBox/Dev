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

namespace bluetooth {
namespace sdp {

enum class PduId : uint8_t {
  RESERVED = 0x00,
  ERROR = 0x01,
  SERVICE_SEARCH_REQUEST = 0x02,
  SERVICE_SEARCH_RESPONSE = 0x03,
  SERVICE_ATTRIBUTE_REQUEST = 0x04,
  SERVICE_ATTRIBUTE_RESPONSE = 0x05,
  SERVICE_SEARCH_ATTRIBUTE_REQUEST = 0x06,
  SERVICE_SEARCH_ATTRIBUTE_RESPONSE = 0x07,
  MAX_VALUE = 0x07,
};

enum class AttributeId : uint16_t {
  SERVICE_RECORD_HANDLE = 0x0000,
  SERVICE_CLASS_ID_LIST = 0x0001,
  SERVICE_RECORD_STATE = 0x0002,
  SERVICE_ID = 0x0003,
  PROTOCOL_DESCRIPTOR_LIST = 0x0004,
  BROWSE_GROUP_LIST = 0x0005,
  LANGUAGE_BASE_ATTRIBUTE_ID_LIST = 0x0006,
  SERVICE_INFO_TIME_TO_LIVE = 0x0007,
  SERVICE_AVAILABILITY = 0x0008,
  PROFILE_DESCRIPTOR_LIST = 0x0009,
  DOCUMENTATION_URL = 0x000A,
  CLIENT_EXECUTABLE_URL = 0x000B,
  ICON_URL = 0x000C,
  ADDITIONAL_PROTOCOL_DESCRIPTOR_LIST = 0x000D,

  // The following attributes are only used in the SDP server service record.
  // They are only valid if ServiceDiscoveryServerServiceClassID is in the
  // ServiceClassIDList. See Bluetooth Core v5.0 Section 5.2.
  VERSION_NUMBER_LIST = 0x0200,
  SERVICE_DATABASE_STATE = 0x0201,
};

// The Attribute ID's of these attributes are calculated by adding the offset
// value for the attribute to the attribute ID base (contained in the
// LanguageBaseAttributeIDList attribute value).
enum AttributeIdOffset : uint16_t {
  SERVICE_NAME = 0x0000,
  SERVICE_DESCRIPTION = 0x0001,
  PROVIDER_NAME = 0x0002,
};

// Constant that define the different types of data element.
enum class DataElementType : uint8_t {
  NIL = 0x00,
  UNSIGNED_INT = 0x01,
  SIGNED_INT = 0x02,
  UUID = 0x03,
  STRING = 0x04,
  BOOLEAN = 0x05,
  DATA_ELEMENT_SEQUENCE = 0x06,
  DATA_ELEMENT_ALTERNATIVE = 0x07,
  URL = 0x08,
  MAX_VALUE = 0x08,
};

// Constant that define the different sizes of data element.
enum class DataElementSize : uint8_t {
  BYTE1 = 0x0,  // Exception: If the data element is NIL then size is 0 bytes
  BYTE2 = 0x1,
  BYTE4 = 0x2,
  BYTE8 = 0x3,
  BYTE16 = 0x4,
  // The size types below represent that the first X bits of the value
  // represents the size of the remaining data.
  ADDITIONAL_8BIT = 0x5,
  ADDITIONAL_16BIT = 0x6,
  ADDITIONAL_32BIT = 0x7,
};

}  // namespace sdp
}  // namespace bluetooth
