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
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "avrcp_test_packets.h"
#include "change_path.h"
#include "packet_test_helper.h"

namespace bluetooth {
namespace avrcp {

using TestChangePathReqPacket = TestPacketType<ChangePathRequest>;

TEST(ChangePathResponseBuilderTest, builderTest) {
  auto builder = ChangePathResponseBuilder::MakeBuilder(Status::NO_ERROR, 2);
  ASSERT_EQ(builder->size(), change_path_response.size());

  auto test_packet = TestChangePathReqPacket::Make();
  builder->Serialize(test_packet);
  ASSERT_EQ(test_packet->GetData(), change_path_response);
}

TEST(ChangePathResponseBuilderTest, builderErrorStatusTest) {
  // NOTE: The num items in folder field doesn't matter when the status is
  // not NO_ERROR
  auto builder =
      ChangePathResponseBuilder::MakeBuilder(Status::INVALID_DIRECTION, 2);
  ASSERT_EQ(builder->size(), change_path_error_response.size());

  auto test_packet = TestChangePathReqPacket::Make();
  builder->Serialize(test_packet);
  ASSERT_EQ(test_packet->GetData(), change_path_error_response);
}

TEST(ChangePathRequestTest, getterTest) {
  auto test_packet = TestChangePathReqPacket::Make(change_path_request);

  ASSERT_EQ(test_packet->GetUidCounter(), 0x0000u);
  ASSERT_EQ(test_packet->GetDirection(), Direction::DOWN);
  ASSERT_EQ(test_packet->GetUid(), 0x0000000000000002u);
}

TEST(ChangePathRequestTest, validTest) {
  auto test_packet = TestChangePathReqPacket::Make(change_path_request);
  ASSERT_TRUE(test_packet->IsValid());
}

TEST(ChangePathRequestTest, invalidTest) {
  auto packet_copy = change_path_request;
  packet_copy.push_back(0x00);
  auto test_packet = TestChangePathReqPacket::Make(packet_copy);
  ASSERT_FALSE(test_packet->IsValid());

  std::vector<uint8_t> short_packet = {0x00, 0x01, 0x02, 0x03, 0x04};
  test_packet = TestChangePathReqPacket::Make(short_packet);
  ASSERT_FALSE(test_packet->IsValid());
}

TEST(ChangePathRequestBuilder, builderTest) {
  auto builder = ChangePathRequestBuilder::MakeBuilder(0, Direction::DOWN, 2);
  ASSERT_EQ(builder->size(), change_path_request.size());

  auto test_packet = TestChangePathReqPacket::Make();
  builder->Serialize(test_packet);
  ASSERT_EQ(test_packet->GetData(), change_path_request);
}

}  // namespace avrcp
}  // namespace bluetooth