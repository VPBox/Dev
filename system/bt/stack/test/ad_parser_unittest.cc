/******************************************************************************
 *
 *  Copyright 2017 The Android Open Source Project
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

#include <gtest/gtest.h>
#include "advertise_data_parser.h"

TEST(AdvertiseDataParserTest, IsValidEmpty) {
  const std::vector<uint8_t> data0;
  EXPECT_TRUE(AdvertiseDataParser::IsValid(data0));

  // Single empty field allowed (treated as zero padding).
  const std::vector<uint8_t> data1{0x00};
  EXPECT_TRUE(AdvertiseDataParser::IsValid(data1));
}

TEST(AdvertiseDataParserTest, IsValidBad) {
  // Single field, field empty.
  const std::vector<uint8_t> data0{0x01};
  EXPECT_FALSE(AdvertiseDataParser::IsValid(data0));

  // Single field, first field length too long.
  const std::vector<uint8_t> data1{0x05, 0x02, 0x00, 0x00, 0x00};
  EXPECT_FALSE(AdvertiseDataParser::IsValid(data1));

  // Two fields, second field length too long.
  const std::vector<uint8_t> data2{0x02, 0x02, 0x00, 0x02, 0x00};
  EXPECT_FALSE(AdvertiseDataParser::IsValid(data2));

  // Two fields, second field empty.
  const std::vector<uint8_t> data3{0x02, 0x02, 0x00, 0x01};
  EXPECT_FALSE(AdvertiseDataParser::IsValid(data3));

  // Non-zero padding at end of packet.
  const std::vector<uint8_t> data4{0x03, 0x02, 0x01, 0x02, 0x02, 0x03, 0x01,
                                   0x00, 0x00, 0xBA, 0xBA, 0x00, 0x00};
  EXPECT_FALSE(AdvertiseDataParser::IsValid(data1));

  // Non-zero padding at end of packet.
  const std::vector<uint8_t> data5{0x03, 0x02, 0x01, 0x02, 0x02,
                                   0x03, 0x01, 0x00, 0xBA};
  EXPECT_FALSE(AdvertiseDataParser::IsValid(data1));
}

TEST(AdvertiseDataParserTest, IsValidGood) {
  // Single field.
  const std::vector<uint8_t> data0{0x03, 0x02, 0x01, 0x02};
  EXPECT_TRUE(AdvertiseDataParser::IsValid(data0));

  // Two fields.
  const std::vector<uint8_t> data1{0x03, 0x02, 0x01, 0x02, 0x02, 0x03, 0x01};
  EXPECT_TRUE(AdvertiseDataParser::IsValid(data1));

  // Zero padding at end of packet.
  const std::vector<uint8_t> data2{0x03, 0x02, 0x01, 0x02,
                                   0x02, 0x03, 0x01, 0x00};
  EXPECT_TRUE(AdvertiseDataParser::IsValid(data2));

  // zero padding at end of packet, sample data from real device
  const std::vector<uint8_t> data3{
      0x10, 0x096, 0x85, 0x44, 0x32, 0x04, 0x74, 0x32, 0x03, 0x13, 0x93,
      0xa,  0x32,  0x39, 0x3a, 0x65, 0x32, 0x05, 0x12, 0x50, 0x00, 0x50,
      0x00, 0x02,  0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  EXPECT_TRUE(AdvertiseDataParser::IsValid(data3));

  // Test Quirk for Traxxas (bad name length, should be 0x11 is 0x14)
  const std::vector<uint8_t> data4{0x14, 0x09, 0x54, 0x52, 0x58, 0x20,
                                   0x42, 0x4C, 0x45, 0x05, 0x12, 0x60,
                                   0x00, 0xE8, 0x03, 0x02, 0x0A, 0x00};
  EXPECT_TRUE(AdvertiseDataParser::IsValid(data4));

  // Test Quirk for Traxxas (bad name length, should be 0x11 is 0x14)
  const std::vector<uint8_t> data5{0x14, 0x09, 0x54, 0x51, 0x69, 0x20,
                                   0x42, 0x4C, 0x45, 0x05, 0x12, 0x64,
                                   0x00, 0xE8, 0x03, 0x02, 0x0A, 0x00};
  EXPECT_TRUE(AdvertiseDataParser::IsValid(data5));

  // Test Quirk for Traxxas (bad name length, should be 0x11 is 0x14)
  const std::vector<uint8_t> data6{0x14, 0x09, 0x54, 0x51, 0x69, 0x20,
                                   0x42, 0x4C, 0x45, 0x05, 0x12, 0x60,
                                   0x00, 0xE8, 0x03, 0x02, 0x0A, 0x00};
  EXPECT_TRUE(AdvertiseDataParser::IsValid(data6));

  // Test Quirk for Traxxas (bad length, should be 0x11 is 0x14)
  // scan response glued after advertise data
  const std::vector<uint8_t> data7{
      0x02, 0x01, 0x06, 0x11, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x64, 0xB1, 0x73, 0x41, 0xE7, 0xF3, 0xC4, 0xB4, 0x80,
      0x08, 0x14, 0x09, 0x54, 0x51, 0x69, 0x20, 0x42, 0x4C, 0x45,
      0x05, 0x12, 0x60, 0x00, 0xE8, 0x03, 0x02, 0x0A, 0x00};
  EXPECT_TRUE(AdvertiseDataParser::IsValid(data7));
}

TEST(AdvertiseDataParserTest, GetFieldByType) {
  // Single field.
  const std::vector<uint8_t> data0{0x03, 0x02, 0x01, 0x02};

  uint8_t p_length;
  const uint8_t* data =
      AdvertiseDataParser::GetFieldByType(data0, 0x02, &p_length);
  EXPECT_EQ(data0.data() + 2, data);
  EXPECT_EQ(2, p_length);

  // Two fields, second field length too long.
  const std::vector<uint8_t> data1{0x02, 0x02, 0x00, 0x03, 0x00};

  // First field is ok.
  data = AdvertiseDataParser::GetFieldByType(data1, 0x02, &p_length);
  EXPECT_EQ(data1.data() + 2, data);
  EXPECT_EQ(0x01, p_length);

  // Second field have bad length.
  data = AdvertiseDataParser::GetFieldByType(data1, 0x03, &p_length);
  EXPECT_EQ(nullptr, data);
  EXPECT_EQ(0, p_length);
}

// This test makes sure that RemoveTrailingZeros is working correctly. It does
// run the RemoveTrailingZeros for ad data, then glue scan response at end of
// it, and checks that the resulting data is good.
TEST(AdvertiseDataParserTest, RemoveTrailingZeros) {
  std::vector<uint8_t> podo_ad_data{
      0x02, 0x01, 0x02, 0x11, 0x06, 0x66, 0x9a, 0x0c, 0x20, 0x00, 0x08,
      0x37, 0xa8, 0xe5, 0x11, 0x81, 0x8b, 0xd0, 0xf0, 0xf0, 0xf0, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  std::vector<uint8_t> podo_scan_resp{
      0x03, 0x19, 0x00, 0x80, 0x09, 0x09, 0x50, 0x6f, 0x64, 0x6f, 0x51,
      0x35, 0x56, 0x47, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  AdvertiseDataParser::RemoveTrailingZeros(podo_ad_data);
  AdvertiseDataParser::RemoveTrailingZeros(podo_scan_resp);

  std::vector<uint8_t> glued(podo_ad_data);
  glued.insert(glued.end(), podo_scan_resp.begin(), podo_scan_resp.end());

  EXPECT_TRUE(AdvertiseDataParser::IsValid(glued));
}

// This test makes sure that RemoveTrailingZeros is removing all bytes after
// first zero length field. It does run the RemoveTrailingZeros for data with
// non-zero bytes after zero length field, then glue scan response at end of it,
// and checks that the resulting data is good. Note: specification requires all
// bytes after zero length field to be zero padding, but many legacy devices got
// this wrong, causing us to have this workaround.
TEST(AdvertiseDataParserTest, RemoveTrailingZerosMalformed) {
  std::vector<uint8_t> ad_data{0x02, 0x01, 0x02, 0x11, 0x06, 0x66, 0x9a, 0x0c,
                               0x20, 0x00, 0x08, 0x37, 0xa8, 0xe5, 0x11, 0x81,
                               0x8b, 0xd0, 0xf0, 0xf0, 0xf0, 0x00, 0xFF, 0xFF,
                               0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  std::vector<uint8_t> scan_resp{0x03, 0x19, 0x00, 0x80, 0x09, 0x09, 0x50, 0x6f,
                                 0x64, 0x6f, 0x51, 0x35, 0x56, 0x47, 0x00, 0xFF,
                                 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

  AdvertiseDataParser::RemoveTrailingZeros(ad_data);
  AdvertiseDataParser::RemoveTrailingZeros(scan_resp);

  std::vector<uint8_t> glued(ad_data);
  glued.insert(glued.end(), scan_resp.begin(), scan_resp.end());

  EXPECT_TRUE(AdvertiseDataParser::IsValid(glued));
}