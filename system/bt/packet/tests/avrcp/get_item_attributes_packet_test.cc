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
#include "get_item_attributes.h"
#include "packet_test_helper.h"

namespace bluetooth {
namespace avrcp {

using TestGetItemAttrsReqPacket = TestPacketType<GetItemAttributesRequest>;

TEST(GetItemAttributesResponseBuilderTest, builderSizeTest) {
  auto builder =
      GetItemAttributesResponseBuilder::MakeBuilder(Status::NO_ERROR, 0xFFFF);
  ASSERT_EQ(builder->size(), 5u);

  builder->AddAttributeEntry(Attribute::TITLE, "Test Song");
  ASSERT_EQ(builder->size(), 22u);

  builder->AddAttributeEntry(Attribute::ARTIST_NAME, "Test Artist");
  ASSERT_EQ(builder->size(), 41u);

  builder->AddAttributeEntry(Attribute::ALBUM_NAME, "Test Album");
  ASSERT_EQ(builder->size(), get_item_attributes_song_response.size());
}

TEST(GetItemAttributesResponseBuilderTest, builderTest) {
  auto builder =
      GetItemAttributesResponseBuilder::MakeBuilder(Status::NO_ERROR, 0xFFFF);
  builder->AddAttributeEntry(Attribute::TITLE, "Test Song");
  builder->AddAttributeEntry(Attribute::ARTIST_NAME, "Test Artist");
  builder->AddAttributeEntry(Attribute::ALBUM_NAME, "Test Album");

  auto test_packet = TestGetItemAttrsReqPacket::Make();
  builder->Serialize(test_packet);
  ASSERT_EQ(test_packet->GetData(), get_item_attributes_song_response);
}

TEST(GetItemAttributesResponseBuilderTest, truncateBuilderTest) {
  auto attribute = AttributeEntry(Attribute::TITLE, "1234");
  size_t truncated_size = BrowsePacket::kMinSize();
  truncated_size += 2;  // Status field  + Number of attributes field
  truncated_size += attribute.size();  // Attribute size

  auto truncated_builder = GetItemAttributesResponseBuilder::MakeBuilder(
      Status::NO_ERROR, truncated_size);
  ASSERT_TRUE(
      truncated_builder->AddAttributeEntry(Attribute::TITLE, "1234truncated"));
  ASSERT_EQ(truncated_builder->size(), truncated_size);

  ASSERT_FALSE(truncated_builder->AddAttributeEntry(Attribute::ARTIST_NAME,
                                                    "Can not add"));

  auto truncated_packet = TestGetItemAttrsReqPacket::Make();
  truncated_builder->Serialize(truncated_packet);

  auto builder =
      GetItemAttributesResponseBuilder::MakeBuilder(Status::NO_ERROR, 0xFFFF);
  builder->AddAttributeEntry(attribute);
  auto test_packet = TestGetItemAttrsReqPacket::Make();
  builder->Serialize(test_packet);

  ASSERT_EQ(truncated_packet->GetData(), test_packet->GetData());
}

TEST(GetItemAttributesResponseBuilderTest, errorStatusTest) {
  std::vector<uint8_t> does_not_exist_status = {0x73, 0x00, 0x01, 0x09};
  auto builder = GetItemAttributesResponseBuilder::MakeBuilder(
      Status::DOES_NOT_EXIST, 0xFFFF);
  ASSERT_EQ(builder->size(), does_not_exist_status.size());

  auto test_packet = TestGetItemAttrsReqPacket::Make();
  builder->Serialize(test_packet);
  ASSERT_EQ(test_packet->GetData(), does_not_exist_status);
}

TEST(GetItemAttributesRequestTest, getterTest) {
  auto test_packet = TestGetItemAttrsReqPacket::Make(
      get_item_attributes_request_all_attributes);

  ASSERT_EQ(test_packet->GetScope(), Scope::NOW_PLAYING);
  ASSERT_EQ(test_packet->GetUidCounter(), 0x0000u);
  ASSERT_EQ(test_packet->GetUid(), 0x0000000000000001u);
  ASSERT_EQ(test_packet->GetNumAttributes(), 7);
  std::vector<Attribute> attrs_requested = {Attribute::TITLE,
                                            Attribute::ARTIST_NAME,
                                            Attribute::ALBUM_NAME,
                                            Attribute::TRACK_NUMBER,
                                            Attribute::TOTAL_NUMBER_OF_TRACKS,
                                            Attribute::GENRE,
                                            Attribute::PLAYING_TIME};
  ASSERT_EQ(test_packet->GetAttributesRequested(), attrs_requested);
}

TEST(GetItemAttributesRequestTest, validTest) {
  auto test_packet = TestGetItemAttrsReqPacket::Make(
      get_item_attributes_request_all_attributes);
  ASSERT_TRUE(test_packet->IsValid());
}

TEST(GetItemAttributesRequestTest, invalidTest) {
  auto packet_copy = get_item_attributes_request_all_attributes;
  packet_copy.push_back(0x00);
  auto test_packet = TestGetItemAttrsReqPacket::Make(packet_copy);
  ASSERT_FALSE(test_packet->IsValid());

  std::vector<uint8_t> short_packet = {0x00, 0x01, 0x02, 0x03,
                                       0x04, 0x05, 0x06, 0x07};
  test_packet = TestGetItemAttrsReqPacket::Make(short_packet);
  ASSERT_FALSE(test_packet->IsValid());
}

}  // namespace avrcp
}  // namespace bluetooth