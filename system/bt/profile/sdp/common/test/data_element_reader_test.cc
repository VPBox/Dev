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

#include <base/logging.h>
#include <gtest/gtest.h>

#include "common/data_element_reader.h"

namespace bluetooth {
namespace sdp {

using namespace testing;
using DataElement = DataElementReader::DataElement;

// A helper class to help work with the Data Element classes.
class ReaderPacket : public ::bluetooth::Packet {
 public:
  using Packet::Packet;

  static std::shared_ptr<ReaderPacket> Make(std::vector<uint8_t> payload) {
    auto pkt = std::shared_ptr<ReaderPacket>(new ReaderPacket());
    pkt->packet_start_index_ = 0;
    pkt->packet_end_index_ = payload.size();
    pkt->data_ = std::make_shared<std::vector<uint8_t>>(std::move(payload));
    return pkt;
  }

  std::string ToString() const override { return ""; }
  bool IsValid() const override { return true; }
  std::pair<size_t, size_t> GetPayloadIndecies() const override {
    return std::pair<size_t, size_t>(packet_start_index_, packet_end_index_);
  }
};

bool operator!=(DataElementReader a, DataElementReader b);

// A helper function to help compare DataElementReader objects.
bool operator==(DataElementReader a, DataElementReader b) {
  while (true) {
    DataElement a_elem = a.ReadNext();
    DataElement b_elem = b.ReadNext();

    if (a_elem != b_elem) return false;

    // If we get here that means both a and b have reached the end.
    if (a_elem == DataElement(std::monostate())) break;
  }

  return true;
}

bool operator!=(DataElementReader a, DataElementReader b) { return !(a == b); }

// A helper function to convert a type and a size to a descriptor byte.
constexpr uint8_t Desc(DataElementType t, DataElementSize s) {
  return static_cast<uint8_t>(t) << 3 | static_cast<uint8_t>(s);
}

// Helper that can create a Data Element reader from a vector.
DataElementReader CreateReader(std::vector<uint8_t> payload) {
  auto packet = ReaderPacket::Make(std::move(payload));
  return DataElementReader(packet->begin(), packet->end());
}

// Test all the valid cases of reading the next Data Element.
using ValidTestParam = std::tuple<std::vector<uint8_t>, DataElement>;
class ValidReadTest : public TestWithParam<ValidTestParam> {};

std::vector<ValidTestParam> valid_values = {
    // Boolean Tests
    ValidTestParam{
        {Desc(DataElementType::BOOLEAN, DataElementSize::BYTE1), 0x01},
        true,
    },
    ValidTestParam{
        {Desc(DataElementType::BOOLEAN, DataElementSize::BYTE1), 0x00},
        false,
    },

    // Signed Integer Tests
    ValidTestParam{
        {Desc(DataElementType::SIGNED_INT, DataElementSize::BYTE1), 0xFF},
        static_cast<int8_t>(-1)},
    ValidTestParam{
        {Desc(DataElementType::SIGNED_INT, DataElementSize::BYTE2), 0xFF, 0xFF},
        static_cast<int16_t>(-1)},
    ValidTestParam{{Desc(DataElementType::SIGNED_INT, DataElementSize::BYTE4),
                    0xFF, 0xFF, 0xFF, 0xFF},
                   static_cast<int32_t>(-1)},
    ValidTestParam{{Desc(DataElementType::SIGNED_INT, DataElementSize::BYTE8),
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
                   static_cast<int64_t>(-1)},
    ValidTestParam{{Desc(DataElementType::SIGNED_INT, DataElementSize::BYTE16),
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
                   std::array<uint8_t, 16>{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                           0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                           0xFF, 0xFF, 0xFF, 0xFF}},

    // Unsigned Integer Tests
    ValidTestParam{
        {Desc(DataElementType::UNSIGNED_INT, DataElementSize::BYTE1), 0x01},
        static_cast<uint8_t>(1)},
    ValidTestParam{{Desc(DataElementType::UNSIGNED_INT, DataElementSize::BYTE2),
                    0x00, 0x01},
                   static_cast<uint16_t>(1)},
    ValidTestParam{{Desc(DataElementType::UNSIGNED_INT, DataElementSize::BYTE4),
                    0x00, 0x00, 0x00, 0x01},
                   static_cast<uint32_t>(1)},
    ValidTestParam{{Desc(DataElementType::UNSIGNED_INT, DataElementSize::BYTE8),
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01},
                   static_cast<uint64_t>(1)},
    ValidTestParam{
        {Desc(DataElementType::UNSIGNED_INT, DataElementSize::BYTE16), 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x01},
        std::array<uint8_t, 16>{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x01}},

    // UUID Tests
    ValidTestParam{
        {Desc(DataElementType::UUID, DataElementSize::BYTE2), 0x01, 0x02},
        Uuid::From16Bit(0x0102)},
    ValidTestParam{{Desc(DataElementType::UUID, DataElementSize::BYTE4), 0x01,
                    0x02, 0x03, 0x04},
                   Uuid::From32Bit(0x01020304)},
    ValidTestParam{
        {Desc(DataElementType::UUID, DataElementSize::BYTE16), 0x00, 0x01, 0x02,
         0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
         0x0F},
        Uuid::From128BitBE({0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                            0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F})},

    // String Tests
    ValidTestParam{
        {Desc(DataElementType::STRING, DataElementSize::ADDITIONAL_8BIT), 0x05,
         'T', 'e', 's', 't', '1'},
        std::string("Test1")},
    ValidTestParam{
        {Desc(DataElementType::STRING, DataElementSize::ADDITIONAL_16BIT), 0x00,
         0x05, 'T', 'e', 's', 't', '2'},
        std::string("Test2")},
    ValidTestParam{
        {Desc(DataElementType::STRING, DataElementSize::ADDITIONAL_32BIT), 0x00,
         0x00, 0x00, 0x05, 'T', 'e', 's', 't', '3'},
        std::string("Test3")},

    // Nested Data Element List Tests
    ValidTestParam{
        {Desc(DataElementType::DATA_ELEMENT_SEQUENCE,
              DataElementSize::ADDITIONAL_8BIT),
         0x04, Desc(DataElementType::BOOLEAN, DataElementSize::BYTE1), 0x01,
         Desc(DataElementType::BOOLEAN, DataElementSize::BYTE1), 0x00},
        CreateReader(
            {Desc(DataElementType::BOOLEAN, DataElementSize::BYTE1), 0x01,
             Desc(DataElementType::BOOLEAN, DataElementSize::BYTE1), 0x00})},
    ValidTestParam{
        {Desc(DataElementType::DATA_ELEMENT_SEQUENCE,
              DataElementSize::ADDITIONAL_16BIT),
         0x00, 0x04, Desc(DataElementType::BOOLEAN, DataElementSize::BYTE1),
         0x01, Desc(DataElementType::BOOLEAN, DataElementSize::BYTE1), 0x00},
        CreateReader(
            {Desc(DataElementType::BOOLEAN, DataElementSize::BYTE1), 0x01,
             Desc(DataElementType::BOOLEAN, DataElementSize::BYTE1), 0x00})},
    ValidTestParam{
        {Desc(DataElementType::DATA_ELEMENT_SEQUENCE,
              DataElementSize::ADDITIONAL_32BIT),
         0x00, 0x00, 0x00, 0x04,
         Desc(DataElementType::BOOLEAN, DataElementSize::BYTE1), 0x01,
         Desc(DataElementType::BOOLEAN, DataElementSize::BYTE1), 0x00},
        CreateReader(
            {Desc(DataElementType::BOOLEAN, DataElementSize::BYTE1), 0x01,
             Desc(DataElementType::BOOLEAN, DataElementSize::BYTE1), 0x00})},
};

INSTANTIATE_TEST_CASE_P(ReadNext, ValidReadTest, ValuesIn(valid_values));
TEST_P(ValidReadTest, Test) {
  auto packet = ReaderPacket::Make(std::get<0>(GetParam()));
  auto value = std::get<1>(GetParam());

  DataElementReader reader(packet->begin(), packet->end());
  auto read_value = reader.ReadNext();

  ASSERT_EQ(value, read_value);

  // Test that there is no additional data to read.
  ASSERT_EQ(reader.ReadNext(), DataElement(std::monostate()));
}

// Test that a nested reader is correctly bounded and can't read past its
// defined end.
TEST(ReadNext, BoundedSubreaderTest) {
  std::vector<uint8_t> payload = {
      // Subsequence descriptor byte.
      Desc(DataElementType::DATA_ELEMENT_SEQUENCE,
           DataElementSize::ADDITIONAL_8BIT),
      // Subsequence length.
      0x04,
      // Subsequence that contains two booleans with values true and false.
      Desc(DataElementType::BOOLEAN, DataElementSize::BYTE1), 0x01,
      Desc(DataElementType::BOOLEAN, DataElementSize::BYTE1), 0x00,
      // Additional int16 at the end of the original sequence.
      Desc(DataElementType::SIGNED_INT, DataElementSize::BYTE2), 0x01, 0x23};

  auto packet = ReaderPacket::Make(payload);
  DataElementReader reader(packet->begin(), packet->end());

  // The first thing read should be the subsequence.
  auto data_element = reader.ReadNext();
  ASSERT_TRUE(std::holds_alternative<DataElementReader>(data_element));

  // Check that the subsequence matches the premade sequence.
  auto subreader = std::get<DataElementReader>(data_element);
  data_element = subreader.ReadNext();
  ASSERT_TRUE(std::holds_alternative<bool>(data_element));
  ASSERT_TRUE(std::get<bool>(data_element));
  data_element = subreader.ReadNext();
  ASSERT_TRUE(std::holds_alternative<bool>(data_element));
  ASSERT_FALSE(std::get<bool>(data_element));

  // Check that there is no additional data to be read from the subreader.
  ASSERT_EQ(subreader.ReadNext(), DataElement(std::monostate()));

  // Check that we can still read the int16 from the original reader.
  data_element = reader.ReadNext();
  ASSERT_TRUE(std::holds_alternative<int16_t>(data_element));
  auto int16_value = std::get<int16_t>(data_element);
  ASSERT_EQ(int16_value, 0x0123);

  // Check that there is no additional data to be read from the base reader.
  ASSERT_EQ(reader.ReadNext(), DataElement(std::monostate()));
}

// Test that trying to read an empty packet fails.
TEST(ReadNext, NoDataTest) {
  auto packet = ReaderPacket::Make({});
  DataElementReader reader(packet->begin(), packet->end());

  ASSERT_EQ(reader.ReadNext(), DataElement(std::monostate()));
}

// Test that using a reserved value for type fails.
TEST(ReadNext, InvalidTypeTest) {
  auto packet = ReaderPacket::Make({0xFF});
  DataElementReader reader(packet->begin(), packet->end());

  ASSERT_EQ(reader.ReadNext(), DataElement(std::monostate()));
}

// Test all invalid parses due to incorrect lengths or invalid sizes. All tests
// should return std::monostate.
using InvalidTestParam = std::vector<uint8_t>;
class InvalidReadTest : public TestWithParam<InvalidTestParam> {};

std::vector<InvalidTestParam> invalid_values = {
    // Boolean Tests:
    //   Invalid size field.
    InvalidTestParam{
        Desc(DataElementType::BOOLEAN, DataElementSize::BYTE2),
    },
    //   Insufficient data.
    InvalidTestParam{Desc(DataElementType::BOOLEAN, DataElementSize::BYTE1)},

    // Signed Integer Tests:
    //   Invalid size field.
    InvalidTestParam{
        Desc(DataElementType::SIGNED_INT, DataElementSize::ADDITIONAL_8BIT)},
    //   1 byte insufficient data.
    InvalidTestParam{Desc(DataElementType::SIGNED_INT, DataElementSize::BYTE1)},
    //   2 byte insufficient data.
    InvalidTestParam{Desc(DataElementType::SIGNED_INT, DataElementSize::BYTE2),
                     0x00},
    //   4 byte insufficient data.
    InvalidTestParam{Desc(DataElementType::SIGNED_INT, DataElementSize::BYTE4),
                     0x00, 0x00, 0x00},
    //  8 Byte insufficient data.
    InvalidTestParam{Desc(DataElementType::SIGNED_INT, DataElementSize::BYTE8),
                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    //  16 Byte insufficient data.
    InvalidTestParam{Desc(DataElementType::SIGNED_INT, DataElementSize::BYTE16),
                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                     0x00, 0x00, 0x00},

    // Unsigned Integer Tests:
    //   Invalid size field.
    InvalidTestParam{
        Desc(DataElementType::UNSIGNED_INT, DataElementSize::ADDITIONAL_8BIT)},
    //   1 byte insufficient data.
    InvalidTestParam{
        Desc(DataElementType::UNSIGNED_INT, DataElementSize::BYTE1)},
    //   2 byte insufficient data.
    InvalidTestParam{
        Desc(DataElementType::UNSIGNED_INT, DataElementSize::BYTE2), 0x00},
    //   4 byte insufficient data.
    InvalidTestParam{
        Desc(DataElementType::UNSIGNED_INT, DataElementSize::BYTE4), 0x00, 0x00,
        0x00},
    //  8 Byte insufficient data.
    InvalidTestParam{
        Desc(DataElementType::UNSIGNED_INT, DataElementSize::BYTE8), 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00},
    //  16 Byte insufficient data.
    InvalidTestParam{
        Desc(DataElementType::UNSIGNED_INT, DataElementSize::BYTE16), 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},

    // UUID Tests:
    //   Invalid size field.
    InvalidTestParam{
        Desc(DataElementType::UUID, DataElementSize::ADDITIONAL_8BIT)},
    //   2 byte insufficient data.
    InvalidTestParam{Desc(DataElementType::UUID, DataElementSize::BYTE2), 0x00},
    //   4 byte insufficient data.
    InvalidTestParam{Desc(DataElementType::UUID, DataElementSize::BYTE4), 0x00,
                     0x00, 0x00},
    //  16 Byte insufficient data.
    InvalidTestParam{Desc(DataElementType::UUID, DataElementSize::BYTE16), 0x00,
                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                     0x00, 0x00},

    // String Tests:
    //   Invalid size field.
    InvalidTestParam{Desc(DataElementType::STRING, DataElementSize::BYTE1)},
    //   Insufficient data for additional 8 bits len.
    InvalidTestParam{
        Desc(DataElementType::STRING, DataElementSize::ADDITIONAL_8BIT)},
    //   Insufficient data for additional 16 bits len.
    InvalidTestParam{
        Desc(DataElementType::STRING, DataElementSize::ADDITIONAL_16BIT),
        0x00,
    },
    //   Insufficient data for additional 32 bit len.
    InvalidTestParam{
        Desc(DataElementType::STRING, DataElementSize::ADDITIONAL_32BIT),
        0x00,
        0x00,
        0x00,
    },
    //   Insufficient data for reported length.
    InvalidTestParam{
        Desc(DataElementType::STRING, DataElementSize::ADDITIONAL_8BIT), 0x04,
        '1', '2', '3'},

    // Nested Data Element List Tests:
    //   Invalid size field.
    InvalidTestParam{
        Desc(DataElementType::DATA_ELEMENT_SEQUENCE, DataElementSize::BYTE1)},
    //   Insufficient data for additional 8 bits len.
    InvalidTestParam{Desc(DataElementType::DATA_ELEMENT_SEQUENCE,
                          DataElementSize::ADDITIONAL_8BIT)},
    //   Insufficient data for additional 16 bits len.
    InvalidTestParam{
        Desc(DataElementType::DATA_ELEMENT_SEQUENCE,
             DataElementSize::ADDITIONAL_16BIT),
        0x00,
    },
    //   Insufficient data for additional 32 bit len.
    InvalidTestParam{
        Desc(DataElementType::DATA_ELEMENT_SEQUENCE,
             DataElementSize::ADDITIONAL_32BIT),
        0x00,
        0x00,
        0x00,
    },
    //   Insufficient data for reported length.
    InvalidTestParam{Desc(DataElementType::DATA_ELEMENT_SEQUENCE,
                          DataElementSize::ADDITIONAL_8BIT),
                     0x04, 0x00, 0x00, 0x00},

    // Unhandled Data Element Types Tests:
    // NOTE: These tests should go away as we begin to handle the types.
    //   Nil Type.
    InvalidTestParam{Desc(DataElementType::NIL, DataElementSize::BYTE1)},
    //   Data Element Alternative List Type.
    InvalidTestParam{Desc(DataElementType::DATA_ELEMENT_ALTERNATIVE,
                          DataElementSize::ADDITIONAL_8BIT),
                     0x00},
    //   URL Type.
    InvalidTestParam{
        Desc(DataElementType::URL, DataElementSize::ADDITIONAL_8BIT), 0x00}};

INSTANTIATE_TEST_CASE_P(ReadNext, InvalidReadTest, ValuesIn(invalid_values));
TEST_P(InvalidReadTest, Test) {
  auto packet = ReaderPacket::Make(GetParam());
  DataElementReader reader(packet->begin(), packet->end());

  ASSERT_EQ(reader.ReadNext(), DataElement(std::monostate()));
}

// Test that trying to read from a reader with start > end crashes.
TEST(DataElementReader, BadBoundsDeathTest) {
  auto packet = ReaderPacket::Make({0x00, 0x00, 0x00, 0x00});
  DataElementReader reader(packet->end(), packet->begin());
  ASSERT_DEATH(reader.ReadNext(), "Beginning of buffer is past end of buffer.");
}

}  // namespace sdp
}  // namespace bluetooth
