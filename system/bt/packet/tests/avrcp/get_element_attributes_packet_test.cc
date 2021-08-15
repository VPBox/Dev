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

#include <gtest/gtest.h>

#include "avrcp_test_packets.h"
#include "get_element_attributes_packet.h"
#include "packet_test_helper.h"

namespace bluetooth {
namespace avrcp {

using TestGetElemAttrReqPacket = TestPacketType<GetElementAttributesRequest>;

TEST(GetElementAttributesRequestPacketTest, getterTest) {
  // Only Title is requested
  auto test_packet =
      TestGetElemAttrReqPacket::Make(get_element_attributes_request_partial);

  ASSERT_EQ(test_packet->GetIdentifier(), 0u);

  auto attribute_list = test_packet->GetAttributesRequested();
  ASSERT_EQ(attribute_list.size(), 1u);
  ASSERT_EQ(attribute_list[0], Attribute::TITLE);

  // Title, Artist, Album, Media Numer, Playing Time, Total Number of Media,
  // and Genre requested
  test_packet =
      TestGetElemAttrReqPacket::Make(get_element_attributes_request_full);

  ASSERT_EQ(test_packet->GetIdentifier(), 0u);

  attribute_list = test_packet->GetAttributesRequested();
  ASSERT_EQ(attribute_list.size(), 7u);
  ASSERT_EQ(attribute_list[0], Attribute::TITLE);
  ASSERT_EQ(attribute_list[1], Attribute::ARTIST_NAME);
  ASSERT_EQ(attribute_list[2], Attribute::ALBUM_NAME);
  ASSERT_EQ(attribute_list[3], Attribute::TRACK_NUMBER);
  ASSERT_EQ(attribute_list[4], Attribute::PLAYING_TIME);
  ASSERT_EQ(attribute_list[5], Attribute::TOTAL_NUMBER_OF_TRACKS);
  ASSERT_EQ(attribute_list[6], Attribute::GENRE);
}

TEST(GetElementAttributesRequestPacketTest, validTest) {
  auto test_packet =
      TestGetElemAttrReqPacket::Make(get_element_attributes_request_partial);
  ASSERT_TRUE(test_packet->IsValid());

  test_packet =
      TestGetElemAttrReqPacket::Make(get_element_attributes_request_full);
  ASSERT_TRUE(test_packet->IsValid());
}

TEST(GetElementAttributesRequestPacketTest, invalidTest) {
  std::vector<uint8_t> packet_copy = get_element_attributes_request_partial;
  packet_copy.push_back(0x00);
  auto test_packet = TestGetElemAttrReqPacket::Make(packet_copy);
  ASSERT_FALSE(test_packet->IsValid());

  std::vector<uint8_t> short_packet = {0x00, 0x00, 0x00, 0x00, 0x00,
                                       0x00, 0x00, 0x00, 0x00, 0x00};
  test_packet = TestGetElemAttrReqPacket::Make(short_packet);
  ASSERT_FALSE(test_packet->IsValid());
}

TEST(GetElementAttributesResponseBuilderTest, builderLengthTest) {
  // Attributes have a size of 8 + string length
  auto builder = GetElementAttributesResponseBuilder::MakeBuilder(0xFFFF);
  ASSERT_EQ(builder->size(), 11u);
  builder->AddAttributeEntry(Attribute::TITLE, "test");
  ASSERT_EQ(builder->size(), 23u);
  builder->AddAttributeEntry(Attribute::ARTIST_NAME, "test");
  ASSERT_EQ(builder->size(), 35u);
}

TEST(GetElementAttributesResponseBuilderTest, builderTest) {
  auto builder = GetElementAttributesResponseBuilder::MakeBuilder(0xFFFF);
  builder->AddAttributeEntry(Attribute::TITLE, "Test Song");
  builder->AddAttributeEntry(Attribute::ARTIST_NAME, "Test Artist");
  builder->AddAttributeEntry(Attribute::ALBUM_NAME, "Test Album");
  builder->AddAttributeEntry(Attribute::TRACK_NUMBER, "1");
  builder->AddAttributeEntry(Attribute::TOTAL_NUMBER_OF_TRACKS, "2");
  builder->AddAttributeEntry(Attribute::GENRE, "Test Genre");
  builder->AddAttributeEntry(Attribute::PLAYING_TIME, "1000");

  ASSERT_EQ(builder->size(), get_elements_attributes_response_full.size());

  auto test_packet = TestGetElemAttrReqPacket::Make();
  builder->Serialize(test_packet);
  ASSERT_EQ(test_packet->GetData(), get_elements_attributes_response_full);
}

TEST(GetElementAttributesResponseBuilderTest, truncateBuilderTest) {
  auto attribute = AttributeEntry(Attribute::TITLE, "1234");
  size_t truncated_size = VendorPacket::kMinSize();
  truncated_size += 1;                 // Number of attributes
  truncated_size += attribute.size();  // Attribute size

  auto truncated_builder =
      GetElementAttributesResponseBuilder::MakeBuilder(truncated_size);
  ASSERT_TRUE(
      truncated_builder->AddAttributeEntry(Attribute::TITLE, "1234truncated"));
  ASSERT_EQ(truncated_builder->size(), truncated_size);

  ASSERT_FALSE(truncated_builder->AddAttributeEntry(Attribute::ARTIST_NAME,
                                                    "Can not add"));

  auto truncated_packet = TestGetElemAttrReqPacket::Make();
  truncated_builder->Serialize(truncated_packet);

  auto builder = GetElementAttributesResponseBuilder::MakeBuilder(0xFFFF);
  builder->AddAttributeEntry(attribute);
  auto test_packet = TestGetElemAttrReqPacket::Make();
  builder->Serialize(test_packet);

  ASSERT_EQ(truncated_packet->GetData(), test_packet->GetData());
}

}  // namespace avrcp
}  // namespace bluetooth