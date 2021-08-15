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

#include "data_element_reader.h"

#include <base/logging.h>
#include <type_traits>

#include "sdp_logging_helper.h"

// A helper macro that can verify that there is enough data remaining in the
// reader to extract without overflowing. end_ - it_ should never be negative
// so casting it to a size_t is always safe. If it does fail, set it_ to end_
// so that all additional readings fail.
#define CHECK_REMAINING_LEN(x)                                              \
  do {                                                                      \
    if ((size_t)(end_ - it_) < x) {                                         \
      LOG(WARNING) << __func__ << ": Extract would read past end of data."; \
      return ParseFail();                                                   \
    }                                                                       \
  } while (0)

namespace bluetooth {
namespace sdp {

DataElementReader::DataElement DataElementReader::ReadNext() {
  if (it_ > end_) LOG(FATAL) << "Beginning of buffer is past end of buffer.";
  if (it_ == end_) return std::monostate();

  uint8_t descriptor = *it_++;
  DataElementType type = static_cast<DataElementType>(descriptor >> 3);
  DataElementSize size = static_cast<DataElementSize>(descriptor & 0b00000111);

  // All types with a value greater than URL are currently reserved.
  if (type > DataElementType::MAX_VALUE) {
    LOG(WARNING) << __func__ << ": Trying to use a reserved data element type";
    return ParseFail();
  }

  switch (type) {
    case DataElementType::BOOLEAN:
      if (size != DataElementSize::BYTE1) {
        LOG(WARNING) << __func__ << ": Invalid size for bool: " << size;
        return ParseFail();
      }

      CHECK_REMAINING_LEN(1);
      return (it_.extract<uint8_t>() != 0);
    case DataElementType::SIGNED_INT:
      return ReadSignedInt(size);
    case DataElementType::UNSIGNED_INT:
      return ReadUnsignedInt(size);
    case DataElementType::UUID:
      return ReadUuid(size);
    case DataElementType::STRING:
      return ReadString(size);
    case DataElementType::DATA_ELEMENT_SEQUENCE:
      return ReadSequence(size);
    default:
      // TODO: The other data element types are never used in the previous SDP
      // implementation. We should properly handle them in the future though
      // for completeness.
      LOG(ERROR) << __func__ << ": Unhandled Data Element Type: " << type;
  }

  return ParseFail();
}

DataElementReader::DataElement DataElementReader::ParseFail() {
  it_ = end_;
  return std::monostate();
}

template <class IntegerType>
DataElementReader::DataElement DataElementReader::ReadInteger() {
  static_assert(std::is_integral<IntegerType>::value,
                "ReadInteger requires an integral type.");

  CHECK_REMAINING_LEN(sizeof(IntegerType));
  return it_.extractBE<IntegerType>();
}

DataElementReader::DataElement DataElementReader::ReadLargeInt() {
  CHECK_REMAINING_LEN(16);

  std::array<uint8_t, 16> array;
  for (size_t i = 0; i < sizeof(uint8_t[16]); i++) {
    array[i] = it_.extract<uint8_t>();
  }

  return array;
}

DataElementReader::DataElement DataElementReader::ReadSignedInt(
    DataElementSize size) {
  switch (size) {
    case DataElementSize::BYTE1:
      return ReadInteger<int8_t>();
    case DataElementSize::BYTE2:
      return ReadInteger<int16_t>();
    case DataElementSize::BYTE4:
      return ReadInteger<int32_t>();
    case DataElementSize::BYTE8:
      return ReadInteger<int64_t>();
    case DataElementSize::BYTE16:
      return ReadLargeInt();
    default:
      LOG(WARNING) << __func__ << ": Invalid size for int: " << size;
  }

  return ParseFail();
}

DataElementReader::DataElement DataElementReader::ReadUnsignedInt(
    DataElementSize size) {
  switch (size) {
    case DataElementSize::BYTE1:
      return ReadInteger<uint8_t>();
    case DataElementSize::BYTE2:
      return ReadInteger<uint16_t>();
    case DataElementSize::BYTE4:
      return ReadInteger<uint32_t>();
    case DataElementSize::BYTE8:
      return ReadInteger<uint64_t>();
    case DataElementSize::BYTE16:
      return ReadLargeInt();
    default:
      LOG(WARNING) << __func__ << ": Invalid size for uint: " << size;
  }

  return ParseFail();
}

DataElementReader::DataElement DataElementReader::ReadUuid(
    DataElementSize size) {
  if (size == DataElementSize::BYTE2) {
    CHECK_REMAINING_LEN(2);
    return Uuid::From16Bit(it_.extractBE<uint16_t>());
  }

  if (size == DataElementSize::BYTE4) {
    CHECK_REMAINING_LEN(4);
    return Uuid::From32Bit(it_.extractBE<uint32_t>());
  }

  if (size == DataElementSize::BYTE16) {
    CHECK_REMAINING_LEN(16);

    Uuid::UUID128Bit uuid_array;
    for (int i = 0; i < 16; i++) {
      uuid_array[i] = it_.extract<uint8_t>();
    }

    return Uuid::From128BitBE(uuid_array);
  }

  LOG(WARNING) << __func__ << ": Invalid size for UUID: " << size;
  return ParseFail();
}

DataElementReader::DataElement DataElementReader::ReadString(
    DataElementSize size) {
  uint32_t num_bytes = 0;

  switch (size) {
    case DataElementSize::ADDITIONAL_8BIT:
      CHECK_REMAINING_LEN(1);
      num_bytes = it_.extractBE<uint8_t>();
      break;
    case DataElementSize::ADDITIONAL_16BIT:
      CHECK_REMAINING_LEN(2);
      num_bytes = it_.extractBE<uint16_t>();
      break;
    case DataElementSize::ADDITIONAL_32BIT:
      CHECK_REMAINING_LEN(4);
      num_bytes = it_.extractBE<uint32_t>();
      break;
    default:
      LOG(WARNING) << __func__ << ": Invalid size for string: " << size;
      return ParseFail();
  }

  CHECK_REMAINING_LEN(num_bytes);

  std::string str;
  for (uint32_t i = 0; i < num_bytes; i++) {
    str.push_back(it_.extractBE<uint8_t>());
  }

  return str;
}

DataElementReader::DataElement DataElementReader::ReadSequence(
    DataElementSize size) {
  uint32_t num_bytes = 0;

  switch (size) {
    case DataElementSize::ADDITIONAL_8BIT:
      CHECK_REMAINING_LEN(1);
      num_bytes = it_.extractBE<uint8_t>();
      break;
    case DataElementSize::ADDITIONAL_16BIT:
      CHECK_REMAINING_LEN(2);
      num_bytes = it_.extractBE<uint16_t>();
      break;
    case DataElementSize::ADDITIONAL_32BIT:
      CHECK_REMAINING_LEN(4);
      num_bytes = it_.extractBE<uint32_t>();
      break;
    default:
      LOG(WARNING) << __func__ << ": Invalid size for string: " << size;
      return ParseFail();
  }

  CHECK_REMAINING_LEN(num_bytes);

  // Create a parser that points to the beginning of the next sequence and move
  // the iterator to past the end of the new sequence.
  auto&& temp = DataElementReader(it_, it_ + num_bytes);
  it_ += num_bytes;
  return std::move(temp);
}

}  // namespace sdp
}  // namespace bluetooth
