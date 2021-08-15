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
#include "get_total_number_of_items.h"
#include "packet_test_helper.h"

namespace bluetooth {
namespace avrcp {

using TestGetTotalNumItemsReqPacket =
    TestPacketType<GetTotalNumberOfItemsRequest>;

TEST(GetTotalNumberOfItemsResponseBuilderTest, builderTest) {
  auto builder = GetTotalNumberOfItemsResponseBuilder::MakeBuilder(
      Status::NO_ERROR, 0x0000u, 0x00000005u);
  ASSERT_EQ(builder->size(), get_total_number_of_items_response.size());

  auto test_packet = TestGetTotalNumItemsReqPacket::Make();
  builder->Serialize(test_packet);
  ASSERT_EQ(test_packet->GetData(), get_total_number_of_items_response);
}

TEST(GetTotalNumberOfItemsResponseBuilderTest, errorStatusTest) {
  std::vector<uint8_t> inv_scope_status_packet = {0x75, 0x00, 0x01, 0x0a};
  auto builder = GetTotalNumberOfItemsResponseBuilder::MakeBuilder(
      Status::INVALID_SCOPE, 0x1234u, 0x56789abcu);
  ASSERT_EQ(builder->size(), inv_scope_status_packet.size());

  auto test_packet = TestGetTotalNumItemsReqPacket::Make();
  builder->Serialize(test_packet);
  ASSERT_EQ(test_packet->GetData(), inv_scope_status_packet);
}

TEST(GetTotalNumberOfItemsRequestTest, getterTest) {
  auto test_packet = TestGetTotalNumItemsReqPacket::Make(
      get_total_number_of_items_request_now_playing);
  ASSERT_EQ(test_packet->GetScope(), Scope::NOW_PLAYING);
}

TEST(GetTotalNumberOfItemsRequestTest, validTest) {
  auto test_packet = TestGetTotalNumItemsReqPacket::Make(
      get_total_number_of_items_request_now_playing);
  ASSERT_TRUE(test_packet->IsValid());
}

TEST(GetTotalNumberOfItemsRequestTest, invalidTest) {
  auto packet_copy = get_total_number_of_items_request_now_playing;
  packet_copy.push_back(0x00);
  auto test_packet = TestGetTotalNumItemsReqPacket::Make(packet_copy);
  ASSERT_FALSE(test_packet->IsValid());

  std::vector<uint8_t> short_packet = {0x00, 0x01, 0x02, 0x03};
  ASSERT_FALSE(test_packet->IsValid());
}

}  // namespace avrcp
}  // namespace bluetooth