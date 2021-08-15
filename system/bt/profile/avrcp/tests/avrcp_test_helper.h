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

#include <gmock/gmock.h>
#include <type_traits>

#include "avrcp.h"
#include "avrcp_internal.h"
#include "avrcp_packet.h"
#include "tests/packet_test_helper.h"

namespace bluetooth {
namespace avrcp {

using AvrcpResponse = std::unique_ptr<::bluetooth::PacketBuilder>;
using TestAvrcpPacket = TestPacketType<Packet>;
using TestBrowsePacket = TestPacketType<BrowsePacket>;

class MockMediaInterface : public MediaInterface {
 public:
  MOCK_METHOD2(SendKeyEvent, void(uint8_t, KeyState));
  MOCK_METHOD1(GetSongInfo, void(MediaInterface::SongInfoCallback));
  MOCK_METHOD1(GetPlayStatus, void(MediaInterface::PlayStatusCallback));
  MOCK_METHOD1(GetNowPlayingList, void(MediaInterface::NowPlayingCallback));
  MOCK_METHOD1(GetMediaPlayerList, void(MediaInterface::MediaListCallback));
  MOCK_METHOD3(GetFolderItems, void(uint16_t, std::string,
                                    MediaInterface::FolderItemsCallback));
  MOCK_METHOD2(SetBrowsedPlayer,
               void(uint16_t, MediaInterface::SetBrowsedPlayerCallback));
  MOCK_METHOD3(PlayItem, void(uint16_t, bool, std::string));
  MOCK_METHOD1(SetActiveDevice, void(const RawAddress&));
  MOCK_METHOD1(RegisterUpdateCallback, void(MediaCallbacks*));
  MOCK_METHOD1(UnregisterUpdateCallback, void(MediaCallbacks*));
};

class MockVolumeInterface : public VolumeInterface {
 public:
  MOCK_METHOD1(DeviceConnected, void(const RawAddress&));
  MOCK_METHOD2(DeviceConnected, void(const RawAddress&, VolumeChangedCb));
  MOCK_METHOD1(DeviceDisconnected, void(const RawAddress&));
  MOCK_METHOD1(SetVolume, void(int8_t));
};

class MockAvrcpInterface : public AvrcpInterface {
 public:
  MOCK_METHOD7(AddRecord, uint16_t(uint16_t, const char*, const char*, uint16_t,
                                   uint32_t, bool, uint16_t));
  MOCK_METHOD4(FindService, uint16_t(uint16_t, const RawAddress&,
                                     tAVRC_SDP_DB_PARAMS*, tAVRC_FIND_CBACK));
  MOCK_METHOD3(Open, uint16_t(uint8_t*, tAVRC_CONN_CB*, const RawAddress&));
  MOCK_METHOD2(OpenBrowse, uint16_t(uint8_t, uint8_t));
  MOCK_METHOD1(GetPeerMtu, uint16_t(uint8_t));
  MOCK_METHOD1(GetBrowseMtu, uint16_t(uint8_t));
  MOCK_METHOD1(Close, uint16_t(uint8_t));
  MOCK_METHOD1(CloseBrowse, uint16_t(uint8_t));
  MOCK_METHOD4(MsgReq, uint16_t(uint8_t, uint8_t, uint8_t, BT_HDR*));
};

class MockA2dpInterface : public A2dpInterface {
 public:
  MOCK_METHOD1(event_open, void(const RawAddress&));
  MOCK_METHOD1(event_close, void(const RawAddress&));
  MOCK_METHOD0(active_peer, RawAddress());
  MOCK_METHOD1(is_peer_in_silence_mode, bool(const RawAddress&));
};

class MockSdpInterface : public SdpInterface {
 public:
  MOCK_METHOD6(InitDiscoveryDb,
               bool(tSDP_DISCOVERY_DB*, uint32_t, uint16_t,
                    const bluetooth::Uuid*, uint16_t, uint16_t*));
  MOCK_METHOD3(ServiceSearchAttributeRequest,
               bool(const RawAddress&, tSDP_DISCOVERY_DB*, tSDP_DISC_CMPL_CB*));
  MOCK_METHOD3(FindServiceInDb,
               tSDP_DISC_REC*(tSDP_DISCOVERY_DB*, uint16_t, t_sdp_disc_rec*));
  MOCK_METHOD2(FindAttributeInRec, tSDP_DISC_ATTR*(t_sdp_disc_rec*, uint16_t));
  MOCK_METHOD3(FindProfileVersionInRec,
               bool(t_sdp_disc_rec*, uint16_t, uint16_t*));
};

ACTION_TEMPLATE(InvokeCb, HAS_1_TEMPLATE_PARAMS(int, k),
                AND_1_VALUE_PARAMS(input)) {
  ::testing::get<k>(args).Run(input);
}

ACTION_TEMPLATE(InvokeCb, HAS_1_TEMPLATE_PARAMS(int, k),
                AND_2_VALUE_PARAMS(a, b)) {
  ::testing::get<k>(args).Run(a, b);
}

ACTION_TEMPLATE(InvokeCb, HAS_1_TEMPLATE_PARAMS(int, k),
                AND_3_VALUE_PARAMS(a, b, c)) {
  ::testing::get<k>(args).Run(a, b, c);
}

template <class PacketType>
class PacketMatcher : public ::testing::MatcherInterface<const AvrcpResponse&> {
 public:
  AvrcpResponse pkt_to_compare_to_;

  PacketMatcher(AvrcpResponse&& pkt) { pkt_to_compare_to_ = std::move(pkt); }

  bool MatchAndExplain(
      const AvrcpResponse& r,
      ::testing::MatchResultListener* listener) const override {
    auto packet1 = TestPacketType<PacketType>::Make();
    r->Serialize(packet1);

    auto packet2 = TestPacketType<PacketType>::Make();
    pkt_to_compare_to_->Serialize(packet2);

    if (packet1->GetData() != packet2->GetData()) {
      *listener << "\nPacket to compare to: \n";
      *listener << packet2->ToString();
      *listener << "\nActual packet: \n";
      *listener << packet1->ToString();
    }

    return packet1->GetData() == packet2->GetData();
  }

  void DescribeTo(::std::ostream* os) const override { *os << "Packets match"; }
};

inline ::testing::Matcher<const AvrcpResponse&> matchPacket(
    std::unique_ptr<PacketBuilder>&& arg) {
  return MakeMatcher(new PacketMatcher<Packet>(std::move(arg)));
}

inline ::testing::Matcher<const AvrcpResponse&> matchPacket(
    std::unique_ptr<BrowsePacketBuilder>&& arg) {
  return MakeMatcher(new PacketMatcher<BrowsePacket>(std::move(arg)));
}

}  // namespace avrcp
}  // namespace bluetooth
