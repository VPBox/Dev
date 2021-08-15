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

#include <algorithm>
#include <iostream>

#include <base/bind.h>
#include <base/logging.h>
#include <base/threading/thread.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "avrcp_packet.h"
#include "avrcp_test_helper.h"
#include "device.h"
#include "stack_config.h"
#include "tests/avrcp/avrcp_test_packets.h"
#include "tests/packet_test_helper.h"

namespace bluetooth {
namespace avrcp {

// TODO (apanicke): All the tests below are just basic positive unit tests.
// Add more tests to increase code coverage.

using AvrcpResponse = std::unique_ptr<::bluetooth::PacketBuilder>;
using TestAvrcpPacket = TestPacketType<Packet>;
using TestBrowsePacket = TestPacketType<BrowsePacket>;

using ::testing::_;
using ::testing::Mock;
using ::testing::MockFunction;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::SaveArg;

bool get_pts_avrcp_test(void) { return false; }

const stack_config_t interface = {
    nullptr, get_pts_avrcp_test, nullptr, nullptr, nullptr, nullptr, nullptr,
    nullptr};

// TODO (apanicke): All the tests below are just basic positive unit tests.
// Add more tests to increase code coverage.
class AvrcpDeviceTest : public ::testing::Test {
 public:
  virtual void SetUp() override {
    // NOTE: We use a wrapper lambda for the MockFunction in order to
    // add a const qualifier to the response. Otherwise the MockFunction
    // type doesn't match the callback type and a compiler error occurs.
    base::Callback<void(uint8_t, bool, AvrcpResponse)> cb = base::Bind(
        [](MockFunction<void(uint8_t, bool, const AvrcpResponse&)>* a,
           uint8_t b, bool c, AvrcpResponse d) { a->Call(b, c, d); },
        &response_cb);

    // TODO (apanicke): Test setting avrc13 to false once we have full
    // functionality.
    test_device = new Device(RawAddress::kAny, true, cb, 0xFFFF, 0xFFFF);
  }

  virtual void TearDown() override {
    delete test_device;
    Mock::VerifyAndClear(&response_cb);
  }

  void SendMessage(uint8_t label, std::shared_ptr<Packet> message) {
    test_device->MessageReceived(label, message);
  }

  void SendBrowseMessage(uint8_t label, std::shared_ptr<BrowsePacket> message) {
    test_device->BrowseMessageReceived(label, message);
  }

  MockFunction<void(uint8_t, bool, const AvrcpResponse&)> response_cb;
  Device* test_device;
};

TEST_F(AvrcpDeviceTest, addressTest) {
  base::Callback<void(uint8_t, bool, AvrcpResponse)> cb =
      base::Bind([](MockFunction<void(uint8_t, bool, const AvrcpResponse&)>* a,
                    uint8_t b, bool c, AvrcpResponse d) { a->Call(b, c, d); },
                 &response_cb);

  Device device(RawAddress::kAny, true, cb, 0xFFFF, 0xFFFF);
  ASSERT_EQ(device.GetAddress(), RawAddress::kAny);
}

TEST_F(AvrcpDeviceTest, trackChangedTest) {
  MockMediaInterface interface;
  NiceMock<MockA2dpInterface> a2dp_interface;

  test_device->RegisterInterfaces(&interface, &a2dp_interface, nullptr);

  SongInfo info = {"test_id",
                   {// The attribute map
                    AttributeEntry(Attribute::TITLE, "Test Song"),
                    AttributeEntry(Attribute::ARTIST_NAME, "Test Artist"),
                    AttributeEntry(Attribute::ALBUM_NAME, "Test Album"),
                    AttributeEntry(Attribute::TRACK_NUMBER, "1"),
                    AttributeEntry(Attribute::TOTAL_NUMBER_OF_TRACKS, "2"),
                    AttributeEntry(Attribute::GENRE, "Test Genre"),
                    AttributeEntry(Attribute::PLAYING_TIME, "1000")}};
  std::vector<SongInfo> list = {info};

  EXPECT_CALL(interface, GetNowPlayingList(_))
      .Times(2)
      .WillRepeatedly(InvokeCb<0>("test_id", list));

  // Test the interim response for track changed
  auto interim_response =
      RegisterNotificationResponseBuilder::MakeTrackChangedBuilder(true, 0x01);
  EXPECT_CALL(response_cb,
              Call(1, false, matchPacket(std::move(interim_response))))
      .Times(1);

  auto request =
      RegisterNotificationRequestBuilder::MakeBuilder(Event::TRACK_CHANGED, 0);
  auto pkt = TestAvrcpPacket::Make();
  request->Serialize(pkt);
  SendMessage(1, pkt);

  // Test the changed response for track changed
  auto changed_response =
      RegisterNotificationResponseBuilder::MakeTrackChangedBuilder(false, 0x01);
  EXPECT_CALL(response_cb,
              Call(1, false, matchPacket(std::move(changed_response))))
      .Times(1);

  test_device->HandleTrackUpdate();
}

TEST_F(AvrcpDeviceTest, playStatusTest) {
  MockMediaInterface interface;
  NiceMock<MockA2dpInterface> a2dp_interface;

  test_device->RegisterInterfaces(&interface, &a2dp_interface, nullptr);

  PlayStatus status1 = {0x1234, 0x5678, PlayState::PLAYING};
  PlayStatus status2 = {0x1234, 0x5678, PlayState::STOPPED};

  EXPECT_CALL(interface, GetPlayStatus(_))
      .Times(2)
      .WillOnce(InvokeCb<0>(status1))
      .WillOnce(InvokeCb<0>(status2));

  // Pretend the device is active
  EXPECT_CALL(a2dp_interface, active_peer())
      .WillRepeatedly(Return(test_device->GetAddress()));

  // Test the interim response for play status changed
  auto interim_response =
      RegisterNotificationResponseBuilder::MakePlaybackStatusBuilder(
          true, PlayState::PLAYING);
  EXPECT_CALL(response_cb,
              Call(1, false, matchPacket(std::move(interim_response))))
      .Times(1);

  auto request = RegisterNotificationRequestBuilder::MakeBuilder(
      Event::PLAYBACK_STATUS_CHANGED, 0);
  auto pkt = TestAvrcpPacket::Make();
  request->Serialize(pkt);
  SendMessage(1, pkt);

  // Test the changed response for play status changed
  auto changed_response =
      RegisterNotificationResponseBuilder::MakePlaybackStatusBuilder(
          false, PlayState::STOPPED);
  EXPECT_CALL(response_cb,
              Call(1, false, matchPacket(std::move(changed_response))))
      .Times(1);
  test_device->HandlePlayStatusUpdate();
}

TEST_F(AvrcpDeviceTest, playPositionTest) {
  MockMediaInterface interface;
  NiceMock<MockA2dpInterface> a2dp_interface;

  test_device->RegisterInterfaces(&interface, &a2dp_interface, nullptr);

  // TODO (apanicke): Add an underlying message loop so we can test the playing
  // state.
  PlayStatus status1 = {0x1234, 0x5678, PlayState::PAUSED};
  PlayStatus status2 = {0x5678, 0x9ABC, PlayState::STOPPED};

  EXPECT_CALL(interface, GetPlayStatus(_))
      .Times(2)
      .WillOnce(InvokeCb<0>(status1))
      .WillOnce(InvokeCb<0>(status2));

  // Pretend the device is active
  EXPECT_CALL(a2dp_interface, active_peer())
      .WillRepeatedly(Return(test_device->GetAddress()));

  // Test the interim response for play position changed
  auto interim_response =
      RegisterNotificationResponseBuilder::MakePlaybackPositionBuilder(true,
                                                                       0x1234);
  EXPECT_CALL(response_cb,
              Call(1, false, matchPacket(std::move(interim_response))))
      .Times(1);

  auto request = RegisterNotificationRequestBuilder::MakeBuilder(
      Event::PLAYBACK_POS_CHANGED, 0);
  auto pkt = TestAvrcpPacket::Make();
  request->Serialize(pkt);
  SendMessage(1, pkt);

  // Test the changed response for play position changed
  auto changed_response =
      RegisterNotificationResponseBuilder::MakePlaybackPositionBuilder(false,
                                                                       0x5678);
  EXPECT_CALL(response_cb,
              Call(1, false, matchPacket(std::move(changed_response))))
      .Times(1);
  test_device->HandlePlayPosUpdate();
}

TEST_F(AvrcpDeviceTest, trackChangedBeforeInterimTest) {
  MockMediaInterface interface;
  NiceMock<MockA2dpInterface> a2dp_interface;

  test_device->RegisterInterfaces(&interface, &a2dp_interface, nullptr);

  // Pretend the device is active
  EXPECT_CALL(a2dp_interface, active_peer())
      .WillRepeatedly(Return(test_device->GetAddress()));

  SongInfo info = {"test_id",
                   {// The attribute map
                    AttributeEntry(Attribute::TITLE, "Test Song"),
                    AttributeEntry(Attribute::ARTIST_NAME, "Test Artist"),
                    AttributeEntry(Attribute::ALBUM_NAME, "Test Album"),
                    AttributeEntry(Attribute::TRACK_NUMBER, "1"),
                    AttributeEntry(Attribute::TOTAL_NUMBER_OF_TRACKS, "2"),
                    AttributeEntry(Attribute::GENRE, "Test Genre"),
                    AttributeEntry(Attribute::PLAYING_TIME, "1000")}};
  std::vector<SongInfo> list = {info};

  MediaInterface::NowPlayingCallback interim_cb;
  MediaInterface::NowPlayingCallback changed_cb;

  EXPECT_CALL(interface, GetNowPlayingList(_))
      .Times(2)
      .WillOnce(SaveArg<0>(&interim_cb))
      .WillOnce(SaveArg<0>(&changed_cb));

  // Test that the changed response doesn't get sent before the interim
  ::testing::InSequence s;
  auto interim_response =
      RegisterNotificationResponseBuilder::MakeTrackChangedBuilder(true, 0x01);
  EXPECT_CALL(response_cb,
              Call(1, false, matchPacket(std::move(interim_response))))
      .Times(1);
  auto changed_response =
      RegisterNotificationResponseBuilder::MakeTrackChangedBuilder(false, 0x01);
  EXPECT_CALL(response_cb,
              Call(1, false, matchPacket(std::move(changed_response))))
      .Times(1);

  // Register for the update, sets interim_cb
  auto request =
      RegisterNotificationRequestBuilder::MakeBuilder(Event::TRACK_CHANGED, 0);
  auto pkt = TestAvrcpPacket::Make();
  request->Serialize(pkt);
  SendMessage(1, pkt);

  // Try to send track changed update, should fail and do nothing
  test_device->HandleTrackUpdate();

  // Send the interim response
  interim_cb.Run("test_id", list);

  // Try to send track changed update, should succeed
  test_device->HandleTrackUpdate();
  changed_cb.Run("test_id", list);
}

TEST_F(AvrcpDeviceTest, playStatusChangedBeforeInterimTest) {
  MockMediaInterface interface;
  NiceMock<MockA2dpInterface> a2dp_interface;

  // Pretend the device is active
  EXPECT_CALL(a2dp_interface, active_peer())
      .WillRepeatedly(Return(test_device->GetAddress()));

  test_device->RegisterInterfaces(&interface, &a2dp_interface, nullptr);

  MediaInterface::PlayStatusCallback interim_cb;
  MediaInterface::PlayStatusCallback changed_cb;

  EXPECT_CALL(interface, GetPlayStatus(_))
      .Times(2)
      .WillOnce(SaveArg<0>(&interim_cb))
      .WillOnce(SaveArg<0>(&changed_cb));

  // Test that the changed response doesn't get sent before the interim
  ::testing::InSequence s;
  auto interim_response =
      RegisterNotificationResponseBuilder::MakePlaybackStatusBuilder(
          true, PlayState::PLAYING);
  EXPECT_CALL(response_cb,
              Call(1, false, matchPacket(std::move(interim_response))))
      .Times(1);
  auto changed_response =
      RegisterNotificationResponseBuilder::MakePlaybackStatusBuilder(
          false, PlayState::STOPPED);
  EXPECT_CALL(response_cb,
              Call(1, false, matchPacket(std::move(changed_response))))
      .Times(1);

  // Send the registration packet
  auto request = RegisterNotificationRequestBuilder::MakeBuilder(
      Event::PLAYBACK_STATUS_CHANGED, 0);
  auto pkt = TestAvrcpPacket::Make();
  request->Serialize(pkt);
  SendMessage(1, pkt);

  // Send a play status update, should be ignored since the interim response
  // hasn't been sent yet.
  test_device->HandlePlayStatusUpdate();

  // Send the interim response.
  PlayStatus status1 = {0x1234, 0x5678, PlayState::PLAYING};
  interim_cb.Run(status1);

  // Send the changed response, should succeed this time
  test_device->HandlePlayStatusUpdate();
  PlayStatus status2 = {0x1234, 0x5678, PlayState::STOPPED};
  changed_cb.Run(status2);
}

TEST_F(AvrcpDeviceTest, playPositionChangedBeforeInterimTest) {
  MockMediaInterface interface;
  NiceMock<MockA2dpInterface> a2dp_interface;

  // Pretend the device is active
  EXPECT_CALL(a2dp_interface, active_peer())
      .WillRepeatedly(Return(test_device->GetAddress()));

  test_device->RegisterInterfaces(&interface, &a2dp_interface, nullptr);

  MediaInterface::PlayStatusCallback interim_cb;
  MediaInterface::PlayStatusCallback changed_cb;

  EXPECT_CALL(interface, GetPlayStatus(_))
      .Times(2)
      .WillOnce(SaveArg<0>(&interim_cb))
      .WillOnce(SaveArg<0>(&changed_cb));

  // Test that the changed response doesn't get sent before the interim
  ::testing::InSequence s;
  auto interim_response =
      RegisterNotificationResponseBuilder::MakePlaybackPositionBuilder(true,
                                                                       0x1234);
  EXPECT_CALL(response_cb,
              Call(1, false, matchPacket(std::move(interim_response))))
      .Times(1);
  auto changed_response =
      RegisterNotificationResponseBuilder::MakePlaybackPositionBuilder(false,
                                                                       0x5678);
  EXPECT_CALL(response_cb,
              Call(1, false, matchPacket(std::move(changed_response))))
      .Times(1);

  // Send the registration packet
  auto request = RegisterNotificationRequestBuilder::MakeBuilder(
      Event::PLAYBACK_POS_CHANGED, 0);
  auto pkt = TestAvrcpPacket::Make();
  request->Serialize(pkt);
  SendMessage(1, pkt);

  // Send a play position update, should be ignored since the notification
  // isn't registered since no interim response has been sent.
  test_device->HandlePlayPosUpdate();

  // Run the interim callback for GetPlayStatus which should be pointing to the
  // GetPlayStatus call made by the update.
  PlayStatus status1 = {0x1234, 0x5678, PlayState::PAUSED};
  interim_cb.Run(status1);

  // Send a play position update, this one should succeed.
  test_device->HandlePlayPosUpdate();
  PlayStatus status2 = {0x5678, 0x9ABC, PlayState::STOPPED};
  changed_cb.Run(status2);
}

TEST_F(AvrcpDeviceTest, nowPlayingChangedBeforeInterim) {
  MockMediaInterface interface;
  NiceMock<MockA2dpInterface> a2dp_interface;

  test_device->RegisterInterfaces(&interface, &a2dp_interface, nullptr);

  SongInfo info = {"test_id",
                   {// The attribute map
                    AttributeEntry(Attribute::TITLE, "Test Song"),
                    AttributeEntry(Attribute::ARTIST_NAME, "Test Artist"),
                    AttributeEntry(Attribute::ALBUM_NAME, "Test Album"),
                    AttributeEntry(Attribute::TRACK_NUMBER, "1"),
                    AttributeEntry(Attribute::TOTAL_NUMBER_OF_TRACKS, "2"),
                    AttributeEntry(Attribute::GENRE, "Test Genre"),
                    AttributeEntry(Attribute::PLAYING_TIME, "1000")}};
  std::vector<SongInfo> list = {info};

  MediaInterface::NowPlayingCallback interim_cb;
  MediaInterface::NowPlayingCallback changed_cb;

  EXPECT_CALL(interface, GetNowPlayingList(_))
      .Times(2)
      .WillOnce(SaveArg<0>(&interim_cb))
      .WillOnce(SaveArg<0>(&changed_cb));

  // Test that the changed response doesn't get sent before the interim
  ::testing::InSequence s;
  auto interim_response =
      RegisterNotificationResponseBuilder::MakeNowPlayingBuilder(true);
  EXPECT_CALL(response_cb,
              Call(1, false, matchPacket(std::move(interim_response))))
      .Times(1);
  auto changed_response =
      RegisterNotificationResponseBuilder::MakeNowPlayingBuilder(false);
  EXPECT_CALL(response_cb,
              Call(1, false, matchPacket(std::move(changed_response))))
      .Times(1);

  // Send the registration packet
  auto request = RegisterNotificationRequestBuilder::MakeBuilder(
      Event::NOW_PLAYING_CONTENT_CHANGED, 0);
  auto pkt = TestAvrcpPacket::Make();
  request->Serialize(pkt);
  SendMessage(1, pkt);

  // Send now playing changed, should fail since the interim response hasn't
  // been sent
  test_device->HandleNowPlayingUpdate();

  // Send the data needed for the interim response
  interim_cb.Run("test_id", list);

  // Send now playing changed, should succeed
  test_device->HandleNowPlayingUpdate();
  changed_cb.Run("test_id", list);
}

TEST_F(AvrcpDeviceTest, addressPlayerChangedBeforeInterim) {
  MockMediaInterface interface;
  NiceMock<MockA2dpInterface> a2dp_interface;

  test_device->RegisterInterfaces(&interface, &a2dp_interface, nullptr);

  MediaInterface::MediaListCallback interim_cb;
  MediaInterface::MediaListCallback changed_cb;

  EXPECT_CALL(interface, GetMediaPlayerList(_))
      .Times(2)
      .WillOnce(SaveArg<0>(&interim_cb))
      .WillOnce(SaveArg<0>(&changed_cb));

  // Test that the changed response doesn't get sent before the interim
  ::testing::InSequence s;
  auto interim_response =
      RegisterNotificationResponseBuilder::MakeAddressedPlayerBuilder(true, 0,
                                                                      0);
  EXPECT_CALL(response_cb,
              Call(1, false, matchPacket(std::move(interim_response))))
      .Times(1);
  auto changed_response =
      RegisterNotificationResponseBuilder::MakeAddressedPlayerBuilder(false, 0,
                                                                      0);
  EXPECT_CALL(response_cb,
              Call(1, false, matchPacket(std::move(changed_response))))
      .Times(1);
  // TODO (apanicke): Remove this expectation once b/110957802 is fixed and
  // we don't try to reject notifications that aren't registered.
  auto rejected_response = RejectBuilder::MakeBuilder(
      CommandPdu::REGISTER_NOTIFICATION, Status::ADDRESSED_PLAYER_CHANGED);
  EXPECT_CALL(response_cb,
              Call(_, false, matchPacket(std::move(rejected_response))))
      .Times(4);

  // Send the registration packet
  auto request = RegisterNotificationRequestBuilder::MakeBuilder(
      Event::ADDRESSED_PLAYER_CHANGED, 0);
  auto pkt = TestAvrcpPacket::Make();
  request->Serialize(pkt);
  SendMessage(1, pkt);

  // Send addressed player update, should fail since the interim response
  // hasn't been sent
  test_device->HandleAddressedPlayerUpdate();

  // Send the data needed for the interim response
  MediaPlayerInfo info = {0, "Test Player", true};
  std::vector<MediaPlayerInfo> list = {info};
  interim_cb.Run(0, list);

  // Send addressed player update, should succeed
  test_device->HandleAddressedPlayerUpdate();
  changed_cb.Run(0, list);
}

TEST_F(AvrcpDeviceTest, nowPlayingTest) {
  MockMediaInterface interface;
  NiceMock<MockA2dpInterface> a2dp_interface;

  test_device->RegisterInterfaces(&interface, &a2dp_interface, nullptr);

  SongInfo info = {"test_id",
                   {// The attribute map
                    AttributeEntry(Attribute::TITLE, "Test Song"),
                    AttributeEntry(Attribute::ARTIST_NAME, "Test Artist"),
                    AttributeEntry(Attribute::ALBUM_NAME, "Test Album"),
                    AttributeEntry(Attribute::TRACK_NUMBER, "1"),
                    AttributeEntry(Attribute::TOTAL_NUMBER_OF_TRACKS, "2"),
                    AttributeEntry(Attribute::GENRE, "Test Genre"),
                    AttributeEntry(Attribute::PLAYING_TIME, "1000")}};
  std::vector<SongInfo> list = {info};
  EXPECT_CALL(interface, GetNowPlayingList(_))
      .Times(2)
      .WillRepeatedly(InvokeCb<0>("test_id", list));

  // Test the interim response for now playing list changed
  auto interim_response =
      RegisterNotificationResponseBuilder::MakeNowPlayingBuilder(true);
  EXPECT_CALL(response_cb,
              Call(1, false, matchPacket(std::move(interim_response))))
      .Times(1);

  auto request = RegisterNotificationRequestBuilder::MakeBuilder(
      Event::NOW_PLAYING_CONTENT_CHANGED, 0);
  auto pkt = TestAvrcpPacket::Make();
  request->Serialize(pkt);
  SendMessage(1, pkt);

  // Test the changed response for now playing list changed
  auto changed_response =
      RegisterNotificationResponseBuilder::MakeNowPlayingBuilder(false);
  EXPECT_CALL(response_cb,
              Call(1, false, matchPacket(std::move(changed_response))))
      .Times(1);
  test_device->HandleNowPlayingUpdate();
}

TEST_F(AvrcpDeviceTest, getPlayStatusTest) {
  MockMediaInterface interface;
  NiceMock<MockA2dpInterface> a2dp_interface;

  test_device->RegisterInterfaces(&interface, &a2dp_interface, nullptr);

  PlayStatus status = {0x1234, 0x5678, PlayState::PLAYING};

  EXPECT_CALL(interface, GetPlayStatus(_))
      .Times(1)
      .WillOnce(InvokeCb<0>(status));

  // Pretend the device is active
  EXPECT_CALL(a2dp_interface, active_peer())
      .WillRepeatedly(Return(test_device->GetAddress()));

  auto expected_response = GetPlayStatusResponseBuilder::MakeBuilder(
      0x5678, 0x1234, PlayState::PLAYING);
  EXPECT_CALL(response_cb,
              Call(1, false, matchPacket(std::move(expected_response))))
      .Times(1);

  auto request = TestAvrcpPacket::Make(get_play_status_request);
  SendMessage(1, request);
}

TEST_F(AvrcpDeviceTest, getElementAttributesTest) {
  MockMediaInterface interface;
  NiceMock<MockA2dpInterface> a2dp_interface;

  test_device->RegisterInterfaces(&interface, &a2dp_interface, nullptr);

  SongInfo info = {"test_id",
                   {// The attribute map
                    AttributeEntry(Attribute::TITLE, "Test Song"),
                    AttributeEntry(Attribute::ARTIST_NAME, "Test Artist"),
                    AttributeEntry(Attribute::ALBUM_NAME, "Test Album"),
                    AttributeEntry(Attribute::TRACK_NUMBER, "1"),
                    AttributeEntry(Attribute::TOTAL_NUMBER_OF_TRACKS, "2"),
                    AttributeEntry(Attribute::GENRE, "Test Genre"),
                    AttributeEntry(Attribute::PLAYING_TIME, "1000")}};

  EXPECT_CALL(interface, GetSongInfo(_)).WillRepeatedly(InvokeCb<0>(info));

  auto compare_to_partial =
      GetElementAttributesResponseBuilder::MakeBuilder(0xFFFF);
  compare_to_partial->AddAttributeEntry(Attribute::TITLE, "Test Song");
  EXPECT_CALL(response_cb,
              Call(2, false, matchPacket(std::move(compare_to_partial))))
      .Times(1);
  SendMessage(2, TestAvrcpPacket::Make(get_element_attributes_request_partial));

  auto compare_to_full =
      GetElementAttributesResponseBuilder::MakeBuilder(0xFFFF);
  compare_to_full->AddAttributeEntry(Attribute::TITLE, "Test Song");
  compare_to_full->AddAttributeEntry(Attribute::ARTIST_NAME, "Test Artist");
  compare_to_full->AddAttributeEntry(Attribute::ALBUM_NAME, "Test Album");
  compare_to_full->AddAttributeEntry(Attribute::TRACK_NUMBER, "1");
  compare_to_full->AddAttributeEntry(Attribute::TOTAL_NUMBER_OF_TRACKS, "2");
  compare_to_full->AddAttributeEntry(Attribute::GENRE, "Test Genre");
  compare_to_full->AddAttributeEntry(Attribute::PLAYING_TIME, "1000");
  EXPECT_CALL(response_cb,
              Call(3, false, matchPacket(std::move(compare_to_full))))
      .Times(1);
  SendMessage(3, TestAvrcpPacket::Make(get_element_attributes_request_full));
}

TEST_F(AvrcpDeviceTest, getElementAttributesMtuTest) {
  auto truncated_packet =
      GetElementAttributesResponseBuilder::MakeBuilder(0xFFFF);
  truncated_packet->AddAttributeEntry(Attribute::TITLE, "1234");

  MockMediaInterface interface;
  NiceMock<MockA2dpInterface> a2dp_interface;

  base::Callback<void(uint8_t, bool, AvrcpResponse)> cb =
      base::Bind([](MockFunction<void(uint8_t, bool, const AvrcpResponse&)>* a,
                    uint8_t b, bool c, AvrcpResponse d) { a->Call(b, c, d); },
                 &response_cb);
  Device device(RawAddress::kAny, true, cb, truncated_packet->size(), 0xFFFF);

  device.RegisterInterfaces(&interface, &a2dp_interface, nullptr);

  SongInfo info = {"test_id",
                   {AttributeEntry(Attribute::TITLE, "1234truncated")}};
  EXPECT_CALL(interface, GetSongInfo(_)).WillRepeatedly(InvokeCb<0>(info));

  EXPECT_CALL(response_cb,
              Call(1, false, matchPacket(std::move(truncated_packet))))
      .Times(1);

  device.MessageReceived(
      1, TestAvrcpPacket::Make(get_element_attributes_request_full));
}

TEST_F(AvrcpDeviceTest, getTotalNumberOfItemsMediaPlayersTest) {
  MockMediaInterface interface;
  NiceMock<MockA2dpInterface> a2dp_interface;

  test_device->RegisterInterfaces(&interface, &a2dp_interface, nullptr);

  std::vector<MediaPlayerInfo> player_list = {
      {0, "player1", true}, {1, "player2", true}, {2, "player3", true},
  };

  EXPECT_CALL(interface, GetMediaPlayerList(_))
      .Times(1)
      .WillOnce(InvokeCb<0>(0, player_list));

  auto expected_response = GetTotalNumberOfItemsResponseBuilder::MakeBuilder(
      Status::NO_ERROR, 0, player_list.size());
  EXPECT_CALL(response_cb,
              Call(1, true, matchPacket(std::move(expected_response))))
      .Times(1);

  SendBrowseMessage(1, TestBrowsePacket::Make(
                           get_total_number_of_items_request_media_players));
}

TEST_F(AvrcpDeviceTest, getTotalNumberOfItemsVFSTest) {
  MockMediaInterface interface;
  NiceMock<MockA2dpInterface> a2dp_interface;

  test_device->RegisterInterfaces(&interface, &a2dp_interface, nullptr);

  std::vector<ListItem> vfs_list = {
      {ListItem::FOLDER, {"id1", true, "folder1"}, SongInfo()},
      {ListItem::FOLDER, {"id2", true, "folder2"}, SongInfo()},
  };

  EXPECT_CALL(interface, GetFolderItems(_, "", _))
      .Times(1)
      .WillOnce(InvokeCb<2>(vfs_list));

  auto expected_response = GetTotalNumberOfItemsResponseBuilder::MakeBuilder(
      Status::NO_ERROR, 0, vfs_list.size());
  EXPECT_CALL(response_cb,
              Call(1, true, matchPacket(std::move(expected_response))))
      .Times(1);

  SendBrowseMessage(
      1, TestBrowsePacket::Make(get_total_number_of_items_request_vfs));
}

TEST_F(AvrcpDeviceTest, getTotalNumberOfItemsNowPlayingTest) {
  MockMediaInterface interface;
  NiceMock<MockA2dpInterface> a2dp_interface;

  test_device->RegisterInterfaces(&interface, &a2dp_interface, nullptr);

  std::vector<SongInfo> now_playing_list = {
      {"test_id1", {}}, {"test_id2", {}}, {"test_id3", {}},
      {"test_id4", {}}, {"test_id5", {}},
  };

  EXPECT_CALL(interface, GetNowPlayingList(_))
      .WillRepeatedly(InvokeCb<0>("test_id1", now_playing_list));

  auto expected_response = GetTotalNumberOfItemsResponseBuilder::MakeBuilder(
      Status::NO_ERROR, 0, now_playing_list.size());
  EXPECT_CALL(response_cb,
              Call(1, true, matchPacket(std::move(expected_response))))
      .Times(1);

  SendBrowseMessage(
      1, TestBrowsePacket::Make(get_total_number_of_items_request_now_playing));
}

TEST_F(AvrcpDeviceTest, getMediaPlayerListTest) {
  MockMediaInterface interface;
  NiceMock<MockA2dpInterface> a2dp_interface;

  test_device->RegisterInterfaces(&interface, &a2dp_interface, nullptr);

  MediaPlayerInfo info = {0, "Test Player", true};
  std::vector<MediaPlayerInfo> list = {info};

  EXPECT_CALL(interface, GetMediaPlayerList(_))
      .Times(1)
      .WillOnce(InvokeCb<0>(0, list));

  auto expected_response = GetFolderItemsResponseBuilder::MakePlayerListBuilder(
      Status::NO_ERROR, 0x0000, 0xFFFF);
  expected_response->AddMediaPlayer(MediaPlayerItem(0, "Test Player", true));
  EXPECT_CALL(response_cb,
              Call(1, true, matchPacket(std::move(expected_response))))
      .Times(1);

  auto request = TestBrowsePacket::Make(get_folder_items_request);
  SendBrowseMessage(1, request);
}

TEST_F(AvrcpDeviceTest, getNowPlayingListTest) {
  MockMediaInterface interface;
  NiceMock<MockA2dpInterface> a2dp_interface;

  test_device->RegisterInterfaces(&interface, &a2dp_interface, nullptr);

  SongInfo info = {"test_id",
                   {// The attribute map
                    AttributeEntry(Attribute::TITLE, "Test Song"),
                    AttributeEntry(Attribute::ARTIST_NAME, "Test Artist"),
                    AttributeEntry(Attribute::ALBUM_NAME, "Test Album"),
                    AttributeEntry(Attribute::TRACK_NUMBER, "1"),
                    AttributeEntry(Attribute::TOTAL_NUMBER_OF_TRACKS, "2"),
                    AttributeEntry(Attribute::GENRE, "Test Genre"),
                    AttributeEntry(Attribute::PLAYING_TIME, "1000")}};
  std::vector<SongInfo> list = {info};

  EXPECT_CALL(interface, GetNowPlayingList(_))
      .WillRepeatedly(InvokeCb<0>("test_id", list));

  auto expected_response = GetFolderItemsResponseBuilder::MakeNowPlayingBuilder(
      Status::NO_ERROR, 0x0000, 0xFFFF);
  expected_response->AddSong(MediaElementItem(1, "Test Song", info.attributes));
  EXPECT_CALL(response_cb,
              Call(1, true, matchPacket(std::move(expected_response))))
      .Times(1);

  auto request = TestBrowsePacket::Make(get_folder_items_request_now_playing);
  SendBrowseMessage(1, request);
}

TEST_F(AvrcpDeviceTest, getVFSFolderTest) {
  MockMediaInterface interface;
  NiceMock<MockA2dpInterface> a2dp_interface;

  test_device->RegisterInterfaces(&interface, &a2dp_interface, nullptr);

  FolderInfo info = {"test_id", true, "Test Folder"};
  ListItem item = {ListItem::FOLDER, info, SongInfo()};
  std::vector<ListItem> list = {item};

  EXPECT_CALL(interface, GetFolderItems(_, "", _))
      .Times(1)
      .WillOnce(InvokeCb<2>(list));

  auto expected_response = GetFolderItemsResponseBuilder::MakeVFSBuilder(
      Status::NO_ERROR, 0x0000, 0xFFFF);
  expected_response->AddFolder(FolderItem(1, 0, true, "Test Folder"));
  EXPECT_CALL(response_cb,
              Call(1, true, matchPacket(std::move(expected_response))))
      .Times(1);

  auto request = TestBrowsePacket::Make(get_folder_items_request_vfs);
  SendBrowseMessage(1, request);
}

TEST_F(AvrcpDeviceTest, getFolderItemsMtuTest) {
  auto truncated_packet = GetFolderItemsResponseBuilder::MakeVFSBuilder(
      Status::NO_ERROR, 0x0000, 0xFFFF);
  truncated_packet->AddFolder(FolderItem(1, 0, true, "Test Folder0"));
  truncated_packet->AddFolder(FolderItem(2, 0, true, "Test Folder1"));

  MockMediaInterface interface;
  NiceMock<MockA2dpInterface> a2dp_interface;
  base::Callback<void(uint8_t, bool, AvrcpResponse)> cb =
      base::Bind([](MockFunction<void(uint8_t, bool, const AvrcpResponse&)>* a,
                    uint8_t b, bool c, AvrcpResponse d) { a->Call(b, c, d); },
                 &response_cb);

  Device device(RawAddress::kAny, true, cb, 0xFFFF,
                truncated_packet->size() + FolderItem::kHeaderSize() + 5);
  device.RegisterInterfaces(&interface, &a2dp_interface, nullptr);

  FolderInfo info0 = {"test_id0", true, "Test Folder0"};
  FolderInfo info1 = {"test_id1", true, "Test Folder1"};
  FolderInfo info2 = {"test_id2", true, "Truncated folder"};
  // Used to ensure that adding an item that would fit in the MTU fails if
  // adding a large item failed.
  FolderInfo small_info = {"test_id2", true, "Small"};

  ListItem item0 = {ListItem::FOLDER, info0, SongInfo()};
  ListItem item1 = {ListItem::FOLDER, info1, SongInfo()};
  ListItem item2 = {ListItem::FOLDER, info2, SongInfo()};
  ListItem item3 = {ListItem::FOLDER, small_info, SongInfo()};

  std::vector<ListItem> list0 = {item0, item1, item2, item3};
  EXPECT_CALL(interface, GetFolderItems(_, "", _))
      .WillRepeatedly(InvokeCb<2>(list0));

  EXPECT_CALL(response_cb,
              Call(1, true, matchPacket(std::move(truncated_packet))))
      .Times(1);
  device.BrowseMessageReceived(
      1, TestBrowsePacket::Make(get_folder_items_request_vfs));
}

TEST_F(AvrcpDeviceTest, changePathTest) {
  MockMediaInterface interface;
  NiceMock<MockA2dpInterface> a2dp_interface;

  test_device->RegisterInterfaces(&interface, &a2dp_interface, nullptr);

  FolderInfo info0 = {"test_id0", true, "Test Folder0"};
  FolderInfo info1 = {"test_id1", true, "Test Folder1"};
  ListItem item0 = {ListItem::FOLDER, info0, SongInfo()};
  ListItem item1 = {ListItem::FOLDER, info1, SongInfo()};
  std::vector<ListItem> list0 = {item0, item1};
  EXPECT_CALL(interface, GetFolderItems(_, "", _))
      .Times(1)
      .WillRepeatedly(InvokeCb<2>(list0));

  FolderInfo info2 = {"test_id2", true, "Test Folder2"};
  FolderInfo info3 = {"test_id3", true, "Test Folder3"};
  FolderInfo info4 = {"test_id4", true, "Test Folder4"};
  ListItem item2 = {ListItem::FOLDER, info2, SongInfo()};
  ListItem item3 = {ListItem::FOLDER, info3, SongInfo()};
  ListItem item4 = {ListItem::FOLDER, info4, SongInfo()};
  std::vector<ListItem> list1 = {item2, item3, item4};
  EXPECT_CALL(interface, GetFolderItems(_, "test_id1", _))
      .Times(3)
      .WillRepeatedly(InvokeCb<2>(list1));

  std::vector<ListItem> list2 = {};
  EXPECT_CALL(interface, GetFolderItems(_, "test_id3", _))
      .Times(1)
      .WillOnce(InvokeCb<2>(list2));

  // Populate the VFS ID map
  auto folder_items_response = GetFolderItemsResponseBuilder::MakeVFSBuilder(
      Status::NO_ERROR, 0x0000, 0xFFFF);
  folder_items_response->AddFolder(FolderItem(1, 0, true, "Test Folder0"));
  folder_items_response->AddFolder(FolderItem(2, 0, true, "Test Folder1"));
  EXPECT_CALL(response_cb,
              Call(1, true, matchPacket(std::move(folder_items_response))))
      .Times(1);

  auto folder_request_builder =
      GetFolderItemsRequestBuilder::MakeBuilder(Scope::VFS, 0, 3, {});
  auto request = TestBrowsePacket::Make();
  folder_request_builder->Serialize(request);
  SendBrowseMessage(1, request);

  // Change path down into Test Folder1
  auto change_path_response =
      ChangePathResponseBuilder::MakeBuilder(Status::NO_ERROR, list1.size());
  EXPECT_CALL(response_cb,
              Call(2, true, matchPacket(std::move(change_path_response))));
  auto path_request_builder =
      ChangePathRequestBuilder::MakeBuilder(0, Direction::DOWN, 2);
  request = TestBrowsePacket::Make();
  path_request_builder->Serialize(request);
  SendBrowseMessage(2, request);

  // Populate the new VFS ID
  folder_items_response = GetFolderItemsResponseBuilder::MakeVFSBuilder(
      Status::NO_ERROR, 0x0000, 0xFFFF);
  folder_items_response->AddFolder(FolderItem(3, 0, true, "Test Folder2"));
  folder_items_response->AddFolder(FolderItem(4, 0, true, "Test Folder3"));
  folder_items_response->AddFolder(FolderItem(5, 0, true, "Test Folder4"));
  EXPECT_CALL(response_cb,
              Call(3, true, matchPacket(std::move(folder_items_response))))
      .Times(1);
  folder_request_builder =
      GetFolderItemsRequestBuilder::MakeBuilder(Scope::VFS, 0, 3, {});
  request = TestBrowsePacket::Make();
  folder_request_builder->Serialize(request);
  SendBrowseMessage(3, request);

  // Change path down into Test Folder3
  change_path_response =
      ChangePathResponseBuilder::MakeBuilder(Status::NO_ERROR, list2.size());
  EXPECT_CALL(response_cb,
              Call(4, true, matchPacket(std::move(change_path_response))));
  path_request_builder =
      ChangePathRequestBuilder::MakeBuilder(0, Direction::DOWN, 4);
  request = TestBrowsePacket::Make();
  path_request_builder->Serialize(request);
  SendBrowseMessage(4, request);

  // Change path up back into Test Folder1
  change_path_response =
      ChangePathResponseBuilder::MakeBuilder(Status::NO_ERROR, list1.size());
  EXPECT_CALL(response_cb,
              Call(5, true, matchPacket(std::move(change_path_response))));
  path_request_builder =
      ChangePathRequestBuilder::MakeBuilder(0, Direction::UP, 0);
  request = TestBrowsePacket::Make();
  path_request_builder->Serialize(request);
  SendBrowseMessage(5, request);
}

TEST_F(AvrcpDeviceTest, getItemAttributesNowPlayingTest) {
  MockMediaInterface interface;
  NiceMock<MockA2dpInterface> a2dp_interface;

  test_device->RegisterInterfaces(&interface, &a2dp_interface, nullptr);

  SongInfo info = {"test_id",
                   {// The attribute map
                    AttributeEntry(Attribute::TITLE, "Test Song"),
                    AttributeEntry(Attribute::ARTIST_NAME, "Test Artist"),
                    AttributeEntry(Attribute::ALBUM_NAME, "Test Album"),
                    AttributeEntry(Attribute::TRACK_NUMBER, "1"),
                    AttributeEntry(Attribute::TOTAL_NUMBER_OF_TRACKS, "2"),
                    AttributeEntry(Attribute::GENRE, "Test Genre"),
                    AttributeEntry(Attribute::PLAYING_TIME, "1000")}};
  std::vector<SongInfo> list = {info};

  EXPECT_CALL(interface, GetNowPlayingList(_))
      .WillRepeatedly(InvokeCb<0>("test_id", list));

  auto compare_to_full =
      GetItemAttributesResponseBuilder::MakeBuilder(Status::NO_ERROR, 0xFFFF);
  compare_to_full->AddAttributeEntry(Attribute::TITLE, "Test Song");
  compare_to_full->AddAttributeEntry(Attribute::ARTIST_NAME, "Test Artist");
  compare_to_full->AddAttributeEntry(Attribute::ALBUM_NAME, "Test Album");
  compare_to_full->AddAttributeEntry(Attribute::TRACK_NUMBER, "1");
  compare_to_full->AddAttributeEntry(Attribute::TOTAL_NUMBER_OF_TRACKS, "2");
  compare_to_full->AddAttributeEntry(Attribute::GENRE, "Test Genre");
  compare_to_full->AddAttributeEntry(Attribute::PLAYING_TIME, "1000");
  EXPECT_CALL(response_cb,
              Call(1, true, matchPacket(std::move(compare_to_full))))
      .Times(1);

  auto request =
      TestBrowsePacket::Make(get_item_attributes_request_all_attributes);
  SendBrowseMessage(1, request);
}

TEST_F(AvrcpDeviceTest, geItemAttributesMtuTest) {
  auto truncated_packet =
      GetItemAttributesResponseBuilder::MakeBuilder(Status::NO_ERROR, 0xFFFF);
  truncated_packet->AddAttributeEntry(Attribute::TITLE, "1234");

  MockMediaInterface interface;
  NiceMock<MockA2dpInterface> a2dp_interface;
  base::Callback<void(uint8_t, bool, AvrcpResponse)> cb =
      base::Bind([](MockFunction<void(uint8_t, bool, const AvrcpResponse&)>* a,
                    uint8_t b, bool c, AvrcpResponse d) { a->Call(b, c, d); },
                 &response_cb);
  Device device(RawAddress::kAny, true, cb, 0xFFFF, truncated_packet->size());
  device.RegisterInterfaces(&interface, &a2dp_interface, nullptr);

  SongInfo info = {"test_id",
                   {AttributeEntry(Attribute::TITLE, "1234truncated")}};
  std::vector<SongInfo> list = {info};
  EXPECT_CALL(interface, GetNowPlayingList(_))
      .WillRepeatedly(InvokeCb<0>("test_id", list));

  EXPECT_CALL(response_cb,
              Call(1, true, matchPacket(std::move(truncated_packet))))
      .Times(1);
  device.BrowseMessageReceived(
      1, TestBrowsePacket::Make(get_item_attributes_request_all_attributes));
}

TEST_F(AvrcpDeviceTest, setAddressedPlayerTest) {
  MockMediaInterface interface;
  NiceMock<MockA2dpInterface> a2dp_interface;

  test_device->RegisterInterfaces(&interface, &a2dp_interface, nullptr);

  MediaPlayerInfo info = {0, "Test Player", true};
  std::vector<MediaPlayerInfo> list = {info};

  EXPECT_CALL(interface, GetMediaPlayerList(_))
      .WillRepeatedly(InvokeCb<0>(0, list));

  auto set_addr_player_rej_rsp = RejectBuilder::MakeBuilder(
      CommandPdu::SET_ADDRESSED_PLAYER, Status::INVALID_PLAYER_ID);

  EXPECT_CALL(response_cb,
              Call(1, false, matchPacket(std::move(set_addr_player_rej_rsp))))
      .Times(1);

  auto player_id_1_request =
      TestAvrcpPacket::Make(set_addressed_player_id_1_request);
  SendMessage(1, player_id_1_request);

  auto set_addr_player_rsp =
      SetAddressedPlayerResponseBuilder::MakeBuilder(Status::NO_ERROR);

  EXPECT_CALL(response_cb,
              Call(1, false, matchPacket(std::move(set_addr_player_rsp))))
      .Times(1);

  auto request = TestAvrcpPacket::Make(set_addressed_player_request);
  SendMessage(1, request);
}

TEST_F(AvrcpDeviceTest, volumeChangedTest) {
  MockMediaInterface interface;
  NiceMock<MockA2dpInterface> a2dp_interface;
  MockVolumeInterface vol_interface;

  test_device->RegisterInterfaces(&interface, &a2dp_interface, &vol_interface);

  // Pretend the device is active
  EXPECT_CALL(a2dp_interface, active_peer())
      .WillRepeatedly(Return(test_device->GetAddress()));

  auto reg_notif =
      RegisterNotificationRequestBuilder::MakeBuilder(Event::VOLUME_CHANGED, 0);
  EXPECT_CALL(response_cb, Call(_, false, matchPacket(std::move(reg_notif))))
      .Times(1);
  test_device->RegisterVolumeChanged();

  EXPECT_CALL(vol_interface, DeviceConnected(test_device->GetAddress(), _))
      .Times(1)
      .WillOnce(InvokeCb<1>(0x30));
  auto set_vol = SetAbsoluteVolumeRequestBuilder::MakeBuilder(0x30);
  EXPECT_CALL(response_cb, Call(_, false, matchPacket(std::move(set_vol))))
      .Times(1);

  auto response = TestAvrcpPacket::Make(interim_volume_changed_notification);
  SendMessage(1, response);

  EXPECT_CALL(vol_interface, SetVolume(0x47)).Times(1);
  auto reg_notif2 =
      RegisterNotificationRequestBuilder::MakeBuilder(Event::VOLUME_CHANGED, 0);
  EXPECT_CALL(response_cb, Call(_, false, matchPacket(std::move(reg_notif2))))
      .Times(1);
  response = TestAvrcpPacket::Make(changed_volume_changed_notification);
  SendMessage(1, response);
  response = TestAvrcpPacket::Make(interim_volume_changed_notification);
  SendMessage(1, response);
}

TEST_F(AvrcpDeviceTest, volumeChangedNonActiveTest) {
  MockMediaInterface interface;
  NiceMock<MockA2dpInterface> a2dp_interface;
  MockVolumeInterface vol_interface;

  test_device->RegisterInterfaces(&interface, &a2dp_interface, &vol_interface);

  // Pretend the device isn't active
  EXPECT_CALL(a2dp_interface, active_peer())
      .WillRepeatedly(Return(RawAddress::kEmpty));

  auto reg_notif =
      RegisterNotificationRequestBuilder::MakeBuilder(Event::VOLUME_CHANGED, 0);
  EXPECT_CALL(response_cb, Call(_, false, matchPacket(std::move(reg_notif))))
      .Times(1);
  test_device->RegisterVolumeChanged();

  EXPECT_CALL(vol_interface, DeviceConnected(test_device->GetAddress(), _))
      .Times(1)
      .WillOnce(InvokeCb<1>(0x30));
  auto set_vol = SetAbsoluteVolumeRequestBuilder::MakeBuilder(0x30);
  EXPECT_CALL(response_cb, Call(_, false, matchPacket(std::move(set_vol))))
      .Times(1);

  auto response = TestAvrcpPacket::Make(interim_volume_changed_notification);
  SendMessage(1, response);

  // Ensure that SetVolume is never called
  EXPECT_CALL(vol_interface, SetVolume(0x47)).Times(0);

  auto reg_notif2 =
      RegisterNotificationRequestBuilder::MakeBuilder(Event::VOLUME_CHANGED, 0);
  EXPECT_CALL(response_cb, Call(_, false, matchPacket(std::move(reg_notif2))))
      .Times(1);
  response = TestAvrcpPacket::Make(changed_volume_changed_notification);
  SendMessage(1, response);
  response = TestAvrcpPacket::Make(interim_volume_changed_notification);
  SendMessage(1, response);
}

TEST_F(AvrcpDeviceTest, volumeRejectedTest) {
  MockMediaInterface interface;
  NiceMock<MockA2dpInterface> a2dp_interface;
  MockVolumeInterface vol_interface;

  test_device->RegisterInterfaces(&interface, &a2dp_interface, &vol_interface);

  auto reg_notif =
      RegisterNotificationRequestBuilder::MakeBuilder(Event::VOLUME_CHANGED, 0);
  EXPECT_CALL(response_cb, Call(_, false, matchPacket(std::move(reg_notif))))
      .Times(1);
  test_device->RegisterVolumeChanged();

  auto response = TestAvrcpPacket::Make(rejected_volume_changed_notification);
  SendMessage(1, response);

  EXPECT_CALL(response_cb, Call(_, _, _)).Times(0);
}

TEST_F(AvrcpDeviceTest, playPushedActiveDeviceTest) {
  MockMediaInterface interface;
  NiceMock<MockA2dpInterface> a2dp_interface;
  MockVolumeInterface vol_interface;

  test_device->RegisterInterfaces(&interface, &a2dp_interface, &vol_interface);

  // Pretend the device is active
  EXPECT_CALL(a2dp_interface, active_peer())
      .WillRepeatedly(Return(test_device->GetAddress()));

  auto play_pushed = PassThroughPacketBuilder::MakeBuilder(false, true, 0x44);
  auto play_pushed_response =
      PassThroughPacketBuilder::MakeBuilder(true, true, 0x44);
  EXPECT_CALL(response_cb,
              Call(_, false, matchPacket(std::move(play_pushed_response))))
      .Times(1);

  PlayStatus status = {0x1234, 0x5678, PlayState::PLAYING};
  EXPECT_CALL(interface, GetPlayStatus(_))
      .Times(1)
      .WillOnce(InvokeCb<0>(status));

  EXPECT_CALL(interface, SendKeyEvent(0x44, KeyState::PUSHED)).Times(1);

  auto play_pushed_pkt = TestAvrcpPacket::Make();
  play_pushed->Serialize(play_pushed_pkt);

  SendMessage(1, play_pushed_pkt);
}

TEST_F(AvrcpDeviceTest, playPushedInactiveDeviceTest) {
  MockMediaInterface interface;
  NiceMock<MockA2dpInterface> a2dp_interface;
  MockVolumeInterface vol_interface;

  test_device->RegisterInterfaces(&interface, &a2dp_interface, &vol_interface);

  // Pretend the device is not active
  EXPECT_CALL(a2dp_interface, active_peer())
      .WillRepeatedly(Return(RawAddress::kEmpty));

  auto play_pushed = PassThroughPacketBuilder::MakeBuilder(false, true, 0x44);
  auto play_pushed_response =
      PassThroughPacketBuilder::MakeBuilder(true, true, 0x44);
  EXPECT_CALL(response_cb,
              Call(_, false, matchPacket(std::move(play_pushed_response))))
      .Times(1);

  // Expect that the device will try to set itself as active
  EXPECT_CALL(interface, SetActiveDevice(test_device->GetAddress())).Times(1);

  // No play command should be sent since the music is already playing
  PlayStatus status = {0x1234, 0x5678, PlayState::PLAYING};
  EXPECT_CALL(interface, GetPlayStatus(_))
      .Times(1)
      .WillOnce(InvokeCb<0>(status));
  EXPECT_CALL(interface, SendKeyEvent(0x44, KeyState::PUSHED)).Times(0);

  auto play_pushed_pkt = TestAvrcpPacket::Make();
  play_pushed->Serialize(play_pushed_pkt);

  SendMessage(1, play_pushed_pkt);
}

TEST_F(AvrcpDeviceTest, mediaKeyActiveDeviceTest) {
  MockMediaInterface interface;
  NiceMock<MockA2dpInterface> a2dp_interface;
  MockVolumeInterface vol_interface;

  test_device->RegisterInterfaces(&interface, &a2dp_interface, &vol_interface);

  // Pretend the device is active
  EXPECT_CALL(a2dp_interface, active_peer())
      .WillRepeatedly(Return(test_device->GetAddress()));

  auto play_released =
      PassThroughPacketBuilder::MakeBuilder(false, false, 0x44);
  auto play_released_response =
      PassThroughPacketBuilder::MakeBuilder(true, false, 0x44);
  EXPECT_CALL(response_cb,
              Call(_, false, matchPacket(std::move(play_released_response))))
      .Times(1);

  EXPECT_CALL(interface, GetPlayStatus(_)).Times(0);

  EXPECT_CALL(interface, SendKeyEvent(0x44, KeyState::RELEASED)).Times(1);

  auto play_released_pkt = TestAvrcpPacket::Make();
  play_released->Serialize(play_released_pkt);

  SendMessage(1, play_released_pkt);
}

TEST_F(AvrcpDeviceTest, mediaKeyInactiveDeviceTest) {
  MockMediaInterface interface;
  NiceMock<MockA2dpInterface> a2dp_interface;
  MockVolumeInterface vol_interface;

  test_device->RegisterInterfaces(&interface, &a2dp_interface, &vol_interface);

  // Pretend the device is not active
  EXPECT_CALL(a2dp_interface, active_peer())
      .WillRepeatedly(Return(RawAddress::kEmpty));

  auto play_released =
      PassThroughPacketBuilder::MakeBuilder(false, false, 0x44);
  auto play_released_response =
      PassThroughPacketBuilder::MakeBuilder(true, false, 0x44);
  EXPECT_CALL(response_cb,
              Call(_, false, matchPacket(std::move(play_released_response))))
      .Times(1);

  EXPECT_CALL(interface, GetPlayStatus(_)).Times(0);

  // Expect that the key event wont be sent to the media interface
  EXPECT_CALL(interface, SendKeyEvent(0x44, KeyState::RELEASED)).Times(0);

  auto play_released_pkt = TestAvrcpPacket::Make();
  play_released->Serialize(play_released_pkt);

  SendMessage(1, play_released_pkt);
}

TEST_F(AvrcpDeviceTest, getCapabilitiesTest) {
  MockMediaInterface interface;
  NiceMock<MockA2dpInterface> a2dp_interface;

  test_device->RegisterInterfaces(&interface, &a2dp_interface, nullptr);

  // GetCapabilities with CapabilityID COMPANY_ID
  auto request_company_id_response =
      GetCapabilitiesResponseBuilder::MakeCompanyIdBuilder(0x001958);
  request_company_id_response->AddCompanyId(0x002345);
  EXPECT_CALL(
      response_cb,
      Call(1, false, matchPacket(std::move(request_company_id_response))))
      .Times(1);

  auto request_company_id =
      TestAvrcpPacket::Make(get_capabilities_request_company_id);
  SendMessage(1, request_company_id);

  // GetCapabilities with CapabilityID EVENTS_SUPPORTED
  auto request_events_supported_response =
      GetCapabilitiesResponseBuilder::MakeEventsSupportedBuilder(
          Event::PLAYBACK_STATUS_CHANGED);
  request_events_supported_response->AddEvent(Event::TRACK_CHANGED);
  request_events_supported_response->AddEvent(Event::PLAYBACK_POS_CHANGED);

  EXPECT_CALL(
      response_cb,
      Call(2, false, matchPacket(std::move(request_events_supported_response))))
      .Times(1);

  auto request_events_supported =
      TestAvrcpPacket::Make(get_capabilities_request);
  SendMessage(2, request_events_supported);

  // GetCapabilities with CapabilityID UNKNOWN
  auto request_unknown_response = RejectBuilder::MakeBuilder(
      CommandPdu::GET_CAPABILITIES, Status::INVALID_PARAMETER);

  EXPECT_CALL(response_cb,
              Call(3, false, matchPacket(std::move(request_unknown_response))))
      .Times(1);

  auto request_unknown =
      TestAvrcpPacket::Make(get_capabilities_request_unknown);
  SendMessage(3, request_unknown);
}

TEST_F(AvrcpDeviceTest, getInvalidItemAttributesTest) {
  MockMediaInterface interface;
  NiceMock<MockA2dpInterface> a2dp_interface;

  test_device->RegisterInterfaces(&interface, &a2dp_interface, nullptr);

  SongInfo info = {"test_id",
                   {// The attribute map
                    AttributeEntry(Attribute::TITLE, "Test Song"),
                    AttributeEntry(Attribute::ARTIST_NAME, "Test Artist"),
                    AttributeEntry(Attribute::ALBUM_NAME, "Test Album"),
                    AttributeEntry(Attribute::TRACK_NUMBER, "1"),
                    AttributeEntry(Attribute::TOTAL_NUMBER_OF_TRACKS, "2"),
                    AttributeEntry(Attribute::GENRE, "Test Genre"),
                    AttributeEntry(Attribute::PLAYING_TIME, "1000")}};
  std::vector<SongInfo> list = {info};

  EXPECT_CALL(interface, GetNowPlayingList(_))
      .WillRepeatedly(InvokeCb<0>("test_id", list));

  auto compare_to_full = GetItemAttributesResponseBuilder::MakeBuilder(
      Status::UIDS_CHANGED, 0xFFFF);
  compare_to_full->AddAttributeEntry(Attribute::TITLE, "Test Song");
  compare_to_full->AddAttributeEntry(Attribute::ARTIST_NAME, "Test Artist");
  compare_to_full->AddAttributeEntry(Attribute::ALBUM_NAME, "Test Album");
  compare_to_full->AddAttributeEntry(Attribute::TRACK_NUMBER, "1");
  compare_to_full->AddAttributeEntry(Attribute::TOTAL_NUMBER_OF_TRACKS, "2");
  compare_to_full->AddAttributeEntry(Attribute::GENRE, "Test Genre");
  compare_to_full->AddAttributeEntry(Attribute::PLAYING_TIME, "1000");
  EXPECT_CALL(response_cb,
              Call(1, true, matchPacket(std::move(compare_to_full))))
      .Times(1);

  auto request = TestBrowsePacket::Make(
      get_item_attributes_request_all_attributes_invalid);
  SendBrowseMessage(1, request);
}

TEST_F(AvrcpDeviceTest, invalidRegisterNotificationTest) {
  MockMediaInterface interface;
  NiceMock<MockA2dpInterface> a2dp_interface;

  test_device->RegisterInterfaces(&interface, &a2dp_interface, nullptr);

  auto reg_notif_rej_rsp = RejectBuilder::MakeBuilder(
      CommandPdu::REGISTER_NOTIFICATION, Status::INVALID_PARAMETER);
  EXPECT_CALL(response_cb,
              Call(1, false, matchPacket(std::move(reg_notif_rej_rsp))))
      .Times(1);

  auto reg_notif_request = TestAvrcpPacket::Make(register_notification_invalid);
  SendMessage(1, reg_notif_request);
}

TEST_F(AvrcpDeviceTest, invalidVendorPacketTest) {
  MockMediaInterface interface;
  NiceMock<MockA2dpInterface> a2dp_interface;

  test_device->RegisterInterfaces(&interface, &a2dp_interface, nullptr);

  auto rsp = RejectBuilder::MakeBuilder(static_cast<CommandPdu>(0), Status::INVALID_COMMAND);
  EXPECT_CALL(response_cb, Call(1, false, matchPacket(std::move(rsp)))).Times(1);
  auto short_packet = TestAvrcpPacket::Make(short_vendor_packet);
  SendMessage(1, short_packet);
}

TEST_F(AvrcpDeviceTest, invalidCapabilitiesPacketTest) {
  MockMediaInterface interface;
  NiceMock<MockA2dpInterface> a2dp_interface;

  test_device->RegisterInterfaces(&interface, &a2dp_interface, nullptr);

  auto rsp = RejectBuilder::MakeBuilder(CommandPdu::GET_CAPABILITIES, Status::INVALID_PARAMETER);
  EXPECT_CALL(response_cb, Call(1, false, matchPacket(std::move(rsp)))).Times(1);
  auto short_packet = TestAvrcpPacket::Make(short_get_capabilities_request);
  SendMessage(1, short_packet);
}

TEST_F(AvrcpDeviceTest, invalidGetElementAttributesPacketTest) {
  MockMediaInterface interface;
  NiceMock<MockA2dpInterface> a2dp_interface;

  test_device->RegisterInterfaces(&interface, &a2dp_interface, nullptr);

  auto rsp = RejectBuilder::MakeBuilder(CommandPdu::GET_ELEMENT_ATTRIBUTES, Status::INVALID_PARAMETER);
  EXPECT_CALL(response_cb, Call(1, false, matchPacket(std::move(rsp)))).Times(1);
  auto short_packet = TestAvrcpPacket::Make(short_get_element_attributes_request);
  SendMessage(1, short_packet);
}

TEST_F(AvrcpDeviceTest, invalidPlayItemPacketTest) {
  MockMediaInterface interface;
  NiceMock<MockA2dpInterface> a2dp_interface;

  test_device->RegisterInterfaces(&interface, &a2dp_interface, nullptr);

  auto rsp = RejectBuilder::MakeBuilder(CommandPdu::PLAY_ITEM, Status::INVALID_PARAMETER);
  EXPECT_CALL(response_cb, Call(1, false, matchPacket(std::move(rsp)))).Times(1);
  auto short_packet = TestAvrcpPacket::Make(short_play_item_request);
  SendMessage(1, short_packet);
}

TEST_F(AvrcpDeviceTest, invalidSetAddressedPlayerPacketTest) {
  MockMediaInterface interface;
  NiceMock<MockA2dpInterface> a2dp_interface;

  test_device->RegisterInterfaces(&interface, &a2dp_interface, nullptr);

  auto rsp = RejectBuilder::MakeBuilder(CommandPdu::SET_ADDRESSED_PLAYER, Status::INVALID_PARAMETER);
  EXPECT_CALL(response_cb, Call(1, false, matchPacket(std::move(rsp)))).Times(1);
  auto short_packet = TestAvrcpPacket::Make(short_set_addressed_player_request);
  SendMessage(1, short_packet);
}

TEST_F(AvrcpDeviceTest, invalidBrowsePacketTest) {
  MockMediaInterface interface;
  NiceMock<MockA2dpInterface> a2dp_interface;

  test_device->RegisterInterfaces(&interface, &a2dp_interface, nullptr);

  auto rsp = GeneralRejectBuilder::MakeBuilder(Status::INVALID_COMMAND);
  EXPECT_CALL(response_cb, Call(1, false, matchPacket(std::move(rsp)))).Times(1);
  auto short_packet = TestBrowsePacket::Make(short_browse_packet);
  SendBrowseMessage(1, short_packet);
}

TEST_F(AvrcpDeviceTest, invalidGetFolderItemsPacketTest) {
  MockMediaInterface interface;
  NiceMock<MockA2dpInterface> a2dp_interface;

  test_device->RegisterInterfaces(&interface, &a2dp_interface, nullptr);

  auto rsp = GetFolderItemsResponseBuilder::MakePlayerListBuilder(Status::INVALID_PARAMETER, 0x0000, 0xFFFF);
  EXPECT_CALL(response_cb, Call(1, true, matchPacket(std::move(rsp)))).Times(1);
  auto short_packet = TestBrowsePacket::Make(short_get_folder_items_request);
  SendBrowseMessage(1, short_packet);
}

TEST_F(AvrcpDeviceTest, invalidGetTotalNumberOfItemsPacketTest) {
  MockMediaInterface interface;
  NiceMock<MockA2dpInterface> a2dp_interface;

  test_device->RegisterInterfaces(&interface, &a2dp_interface, nullptr);

  auto rsp = GetTotalNumberOfItemsResponseBuilder::MakeBuilder(Status::INVALID_PARAMETER, 0x0000, 0xFFFF);
  EXPECT_CALL(response_cb, Call(1, true, matchPacket(std::move(rsp)))).Times(1);
  auto short_packet = TestBrowsePacket::Make(short_get_total_number_of_items_request);
  SendBrowseMessage(1, short_packet);
}

TEST_F(AvrcpDeviceTest, invalidChangePathPacketTest) {
  MockMediaInterface interface;
  NiceMock<MockA2dpInterface> a2dp_interface;

  test_device->RegisterInterfaces(&interface, &a2dp_interface, nullptr);

  auto rsp = ChangePathResponseBuilder::MakeBuilder(Status::INVALID_PARAMETER, 0);
  EXPECT_CALL(response_cb, Call(1, true, matchPacket(std::move(rsp)))).Times(1);
  auto short_packet = TestBrowsePacket::Make(short_change_path_request);
  SendBrowseMessage(1, short_packet);
}

TEST_F(AvrcpDeviceTest, invalidGetItemAttributesPacketTest) {
  MockMediaInterface interface;
  NiceMock<MockA2dpInterface> a2dp_interface;

  test_device->RegisterInterfaces(&interface, &a2dp_interface, nullptr);

  auto rsp = GetItemAttributesResponseBuilder::MakeBuilder(Status::INVALID_PARAMETER, 0xFFFF);
  EXPECT_CALL(response_cb, Call(1, true, matchPacket(std::move(rsp)))).Times(1);
  auto short_packet = TestBrowsePacket::Make(short_get_item_attributes_request);
  SendBrowseMessage(1, short_packet);
}

}  // namespace avrcp
}  // namespace bluetooth

const stack_config_t* stack_config_get_interface(void) {
  return &bluetooth::avrcp::interface;
}
