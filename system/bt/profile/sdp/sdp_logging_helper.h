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

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>

#include "bt_trace.h"
#include "sdp_common.h"

namespace bluetooth {
namespace sdp {

#ifndef CASE_RETURN_TEXT
#define CASE_RETURN_TEXT(code) \
  case code:                   \
    return #code
#endif

inline std::string PduIdText(const PduId& id) {
  switch (id) {
    CASE_RETURN_TEXT(PduId::RESERVED);
    CASE_RETURN_TEXT(PduId::ERROR);
    CASE_RETURN_TEXT(PduId::SERVICE_SEARCH_REQUEST);
    CASE_RETURN_TEXT(PduId::SERVICE_SEARCH_RESPONSE);
    CASE_RETURN_TEXT(PduId::SERVICE_ATTRIBUTE_REQUEST);
    CASE_RETURN_TEXT(PduId::SERVICE_ATTRIBUTE_RESPONSE);
    CASE_RETURN_TEXT(PduId::SERVICE_SEARCH_ATTRIBUTE_REQUEST);
    CASE_RETURN_TEXT(PduId::SERVICE_SEARCH_ATTRIBUTE_RESPONSE);
    default:
      return "Unknown PduId: " + loghex((uint8_t)id);
  }
}

inline std::ostream& operator<<(std::ostream& os, const PduId& id) {
  return os << PduIdText(id);
}

inline std::string AttributeIdText(const AttributeId& id) {
  switch (id) {
    CASE_RETURN_TEXT(AttributeId::SERVICE_RECORD_HANDLE);
    CASE_RETURN_TEXT(AttributeId::SERVICE_CLASS_ID_LIST);
    CASE_RETURN_TEXT(AttributeId::SERVICE_RECORD_STATE);
    CASE_RETURN_TEXT(AttributeId::SERVICE_ID);
    CASE_RETURN_TEXT(AttributeId::PROTOCOL_DESCRIPTOR_LIST);
    CASE_RETURN_TEXT(AttributeId::BROWSE_GROUP_LIST);
    CASE_RETURN_TEXT(AttributeId::LANGUAGE_BASE_ATTRIBUTE_ID_LIST);
    CASE_RETURN_TEXT(AttributeId::SERVICE_INFO_TIME_TO_LIVE);
    CASE_RETURN_TEXT(AttributeId::SERVICE_AVAILABILITY);
    CASE_RETURN_TEXT(AttributeId::PROFILE_DESCRIPTOR_LIST);
    CASE_RETURN_TEXT(AttributeId::DOCUMENTATION_URL);
    CASE_RETURN_TEXT(AttributeId::CLIENT_EXECUTABLE_URL);
    CASE_RETURN_TEXT(AttributeId::ICON_URL);
    CASE_RETURN_TEXT(AttributeId::ADDITIONAL_PROTOCOL_DESCRIPTOR_LIST);
    CASE_RETURN_TEXT(AttributeId::VERSION_NUMBER_LIST);
    CASE_RETURN_TEXT(AttributeId::SERVICE_DATABASE_STATE);
    default:
      return "Unknown AttributeId: " + loghex((uint16_t)id);
  }
}

inline std::ostream& operator<<(std::ostream& os, const AttributeId& id) {
  return os << AttributeIdText(id);
}

inline std::string DataElementTypeText(const DataElementType& type) {
  switch (type) {
    CASE_RETURN_TEXT(DataElementType::NIL);
    CASE_RETURN_TEXT(DataElementType::UNSIGNED_INT);
    CASE_RETURN_TEXT(DataElementType::SIGNED_INT);
    CASE_RETURN_TEXT(DataElementType::UUID);
    CASE_RETURN_TEXT(DataElementType::STRING);
    CASE_RETURN_TEXT(DataElementType::BOOLEAN);
    CASE_RETURN_TEXT(DataElementType::DATA_ELEMENT_SEQUENCE);
    CASE_RETURN_TEXT(DataElementType::DATA_ELEMENT_ALTERNATIVE);
    CASE_RETURN_TEXT(DataElementType::URL);
    default:
      return "Unknown DataElementType: " + loghex((uint8_t)type);
  }
}

inline std::ostream& operator<<(std::ostream& os, const DataElementType& type) {
  return os << DataElementTypeText(type);
}

inline std::string DataElementSizeText(const DataElementSize& size) {
  switch (size) {
    CASE_RETURN_TEXT(DataElementSize::BYTE1);
    CASE_RETURN_TEXT(DataElementSize::BYTE2);
    CASE_RETURN_TEXT(DataElementSize::BYTE4);
    CASE_RETURN_TEXT(DataElementSize::BYTE8);
    CASE_RETURN_TEXT(DataElementSize::BYTE16);
    CASE_RETURN_TEXT(DataElementSize::ADDITIONAL_8BIT);
    CASE_RETURN_TEXT(DataElementSize::ADDITIONAL_16BIT);
    CASE_RETURN_TEXT(DataElementSize::ADDITIONAL_32BIT);
    default:
      return "Unknown DataElementSize: " + loghex((uint8_t)size);
  }
}

inline std::ostream& operator<<(std::ostream& os, const DataElementSize& size) {
  return os << DataElementSizeText(size);
}

}  // namespace sdp
}  // namespace bluetooth
