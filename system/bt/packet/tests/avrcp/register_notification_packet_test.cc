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
#include "packet_test_helper.h"
#include "register_notification_packet.h"

namespace bluetooth {
namespace avrcp {

using TestRegNotifReqPacket = TestPacketType<RegisterNotificationRequest>;
using TestRegNotifRspPacket = TestPacketType<RegisterNotificationResponse>;

TEST(RegisterNotificationRequestTest, getterTest) {
  auto test_packet =
      TestRegNotifReqPacket::Make(register_play_status_notification);

  ASSERT_EQ(test_packet->GetEventRegistered(), Event::PLAYBACK_STATUS_CHANGED);
  ASSERT_EQ(test_packet->GetInterval(), 5u);
}

TEST(RegisterNotificationRequestTest, validTest) {
  auto test_packet =
      TestRegNotifReqPacket::Make(register_play_status_notification);
  ASSERT_TRUE(test_packet->IsValid());
}

TEST(RegisterNotificationRequestTest, invalidTest) {
  std::vector<uint8_t> packet_copy = register_play_status_notification;
  packet_copy.push_back(0x00);
  auto test_packet = TestRegNotifReqPacket::Make(packet_copy);
  ASSERT_FALSE(test_packet->IsValid());

  std::vector<uint8_t> short_packet = {0, 1, 2, 3, 4};
  test_packet = TestRegNotifReqPacket::Make(short_packet);
  ASSERT_FALSE(test_packet->IsValid());
}

TEST(RegisterNotificationRequestBuilderTest, builderTest) {
  auto builder =
      RegisterNotificationRequestBuilder::MakeBuilder(Event::VOLUME_CHANGED, 0);
  ASSERT_EQ(builder->size(), register_volume_changed_notification.size());

  auto test_packet = TestRegNotifReqPacket::Make();
  builder->Serialize(test_packet);
  ASSERT_EQ(test_packet->GetData(), register_volume_changed_notification);
}

TEST(RegisterNotificationResponseTest, volumeGetterTest) {
  auto test_packet =
      TestRegNotifRspPacket::Make(interim_volume_changed_notification);

  ASSERT_TRUE(test_packet->IsInterim());
  ASSERT_EQ(test_packet->GetEvent(), Event::VOLUME_CHANGED);
  ASSERT_EQ(test_packet->GetVolume(), 0x47);
}

TEST(RegisterNotificationResponseTest, validTest) {
  auto test_packet =
      TestRegNotifRspPacket::Make(interim_volume_changed_notification);

  ASSERT_TRUE(test_packet->IsValid());
}

TEST(RegisterNotificationResponseTest, invalidTest) {
  std::vector<uint8_t> packet_copy = interim_volume_changed_notification;
  packet_copy.push_back(0x00);
  auto test_packet = TestRegNotifRspPacket::Make(packet_copy);
  ASSERT_FALSE(test_packet->IsValid());

  std::vector<uint8_t> short_packet = {0, 1, 2, 3, 4};
  test_packet = TestRegNotifRspPacket::Make(short_packet);
  ASSERT_FALSE(test_packet->IsValid());

  auto wrong_ctype = interim_volume_changed_notification;
  wrong_ctype[0] = 0x00;
  test_packet = TestRegNotifRspPacket::Make(short_packet);
  ASSERT_FALSE(test_packet->IsValid());
}

TEST(RegisterNotificationResponseTest, wrongEventDeathTest) {
  auto wrong_event = interim_volume_changed_notification;
  wrong_event[10] = 0x00;
  auto test_packet = TestRegNotifRspPacket::Make(wrong_event);

  ASSERT_DEATH(test_packet->GetVolume(),
               "GetEvent\\(\\) == Event::VOLUME_CHANGED");
}

TEST(RegisterNotificationResponseBuilderTest, playStatusBuilderTest) {
  auto builder = RegisterNotificationResponseBuilder::MakePlaybackStatusBuilder(
      true, 0x00);
  ASSERT_EQ(builder->size(), interim_play_status_notification.size());
  auto test_packet = TestRegNotifReqPacket::Make();
  builder->Serialize(test_packet);
  ASSERT_EQ(test_packet->GetData(), interim_play_status_notification);
}

TEST(RegisterNotificationResponseBuilderTest, trackChangedBuilderTest) {
  auto builder = RegisterNotificationResponseBuilder::MakeTrackChangedBuilder(
      true, 0x0000000000000000);
  ASSERT_EQ(builder->size(), interim_track_changed_notification.size());
  auto test_packet = TestRegNotifReqPacket::Make();
  builder->Serialize(test_packet);
  ASSERT_EQ(test_packet->GetData(), interim_track_changed_notification);
}

TEST(RegisterNotificationResponseBuilderTest, playPositionBuilderTest) {
  auto builder =
      RegisterNotificationResponseBuilder::MakePlaybackPositionBuilder(
          false, 0x00000000);
  ASSERT_EQ(builder->size(), changed_play_pos_notification.size());
  auto test_packet = TestRegNotifReqPacket::Make();
  builder->Serialize(test_packet);
  ASSERT_EQ(test_packet->GetData(), changed_play_pos_notification);
}

TEST(RegisterNotificationResponseBuilderTest, nowPlayingBuilderTest) {
  auto builder =
      RegisterNotificationResponseBuilder::MakeNowPlayingBuilder(true);
  ASSERT_EQ(builder->size(), interim_now_playing_notification.size());
  auto test_packet = TestRegNotifReqPacket::Make();
  builder->Serialize(test_packet);
  ASSERT_EQ(test_packet->GetData(), interim_now_playing_notification);
}

TEST(RegisterNotificationResponseBuilderTest, availablePlayersBuilderTest) {
  auto builder =
      RegisterNotificationResponseBuilder::MakeAvailablePlayersBuilder(true);
  ASSERT_EQ(builder->size(), interim_available_players_notification.size());
  auto test_packet = TestRegNotifReqPacket::Make();
  builder->Serialize(test_packet);
  ASSERT_EQ(test_packet->GetData(), interim_available_players_notification);
}

TEST(RegisterNotificationResponseBuilderTest, addressedPlayerBuilderTest) {
  auto builder =
      RegisterNotificationResponseBuilder::MakeAddressedPlayerBuilder(true, 1,
                                                                      0x0000);
  ASSERT_EQ(builder->size(), interim_addressed_player_notification.size());
  auto test_packet = TestRegNotifReqPacket::Make();
  builder->Serialize(test_packet);
  ASSERT_EQ(test_packet->GetData(), interim_addressed_player_notification);
}

TEST(RegisterNotificationResponseBuilderTest, uidsChangedBuilderTest) {
  auto builder =
      RegisterNotificationResponseBuilder::MakeUidsChangedBuilder(true, 0x0000);
  ASSERT_EQ(builder->size(), interim_uids_notificaiton.size());
  auto test_packet = TestRegNotifReqPacket::Make();
  builder->Serialize(test_packet);
  ASSERT_EQ(test_packet->GetData(), interim_uids_notificaiton);
}

}  // namespace avrcp
}  // namespace bluetooth
