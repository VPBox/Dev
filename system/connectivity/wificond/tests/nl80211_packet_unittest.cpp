/*
 * Copyright (C) 2016, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <memory>

#include <gtest/gtest.h>

#include "wificond/net/kernel-header-latest/nl80211.h"
#include "wificond/net/nl80211_attribute.h"
#include "wificond/net/nl80211_packet.h"

using std::string;

namespace android {
namespace wificond {

namespace {

const uint8_t kU8Value1 = 150;
const uint16_t kU16Value1 = 5000;
const uint32_t kU32Value1 = 500000;
const uint32_t kU32Value2 = 800000;
const uint32_t kPortId = 123;

const uint8_t kGenNLCommand = 102;
const uint16_t kNLMsgType = 4000;
const uint32_t kNLMsgSequenceNumber = 70000;

// NL80211 family id is dynamically allocated by kernel.
const uint16_t kNL80211FamilyId = 0x13;
const uint32_t kWiPhy = 0;
const uint32_t kExpectedIfIndex = 4;
const uint8_t kMacAddressBytes[] = {
    0xc0, 0x3f, 0x0e, 0x77, 0xe8, 0x7f
};

const unsigned char kNL80211_CMD_ASSOCIATE[] = {
    0x58, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x26, 0x01, 0x00, 0x00, 0x08, 0x00, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x03, 0x00,
    0x04, 0x00, 0x00, 0x00, 0x32, 0x00, 0x33, 0x00,
    0x10, 0x00, 0x3a, 0x01, 0x48, 0x5d, 0x60, 0x77,
    0x2d, 0xcf, 0xc0, 0x3f, 0x0e, 0x77, 0xe8, 0x7f,
    0xc0, 0x3f, 0x0e, 0x77, 0xe8, 0x7f, 0x40, 0x07,
    0x01, 0x04, 0x00, 0x00, 0x01, 0xc0, 0x01, 0x08,
    0x82, 0x84, 0x8b, 0x96, 0x0c, 0x12, 0x18, 0x24,
    0x32, 0x04, 0x30, 0x48, 0x60, 0x6c, 0x00, 0x00,
};

const uint32_t kExpectedCqmNotAcked = 50;

const unsigned char kNL80211_CMD_NOTIFY_CQM[] = {
    0x3c, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x40, 0x01, 0x00, 0x00, 0x08, 0x00, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x03, 0x00,
    0x04, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x06, 0x00,
    0xc0, 0x3f, 0x0e, 0x77, 0xe8, 0x7f, 0x00, 0x00,
    0x0c, 0x00, 0x5e, 0x00, 0x08, 0x00, 0x04, 0x00,
    0x32, 0x00, 0x00, 0x00,
};

const uint32_t kNewStationExpectedGeneration = 275;

const unsigned char kNL80211_CMD_NEW_STATION[] = {
    0x34, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x13, 0x01, 0x00, 0x00, 0x08, 0x00, 0x03, 0x00,
    0x04, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x06, 0x00,
    0xc0, 0x3f, 0x0e, 0x77, 0xe8, 0x7f, 0x00, 0x00,
    0x08, 0x00, 0x2e, 0x00, 0x13, 0x01, 0x00, 0x00,
    0x04, 0x00, 0x15, 0x00,
};

}  // namespace

TEST(NL80211PacketTest, CanConstructValidNL80211Packet) {
  NL80211Packet netlink_packet(kNLMsgType,
                               kGenNLCommand,
                               kNLMsgSequenceNumber,
                               kPortId);
  EXPECT_TRUE(netlink_packet.IsValid());
}

TEST(NL80211PacketTest, SetAndGetNL80211PacketHeaderFields) {
  NL80211Packet netlink_packet(kNLMsgType,
                               kGenNLCommand,
                               kNLMsgSequenceNumber,
                               kPortId);
  netlink_packet.SetFlags(NLM_F_MULTI);
  EXPECT_EQ(kGenNLCommand, netlink_packet.GetCommand());
  EXPECT_EQ(kNLMsgType, netlink_packet.GetMessageType());
  EXPECT_EQ(kNLMsgSequenceNumber, netlink_packet.GetMessageSequence());
  EXPECT_TRUE(netlink_packet.IsMulti());
  EXPECT_EQ(kPortId, netlink_packet.GetPortId());
}

TEST(NL80211PacketTest, AddAttributeToNL80211Packet) {
  NL80211Packet netlink_packet(kNLMsgType,
                               kGenNLCommand,
                               kNLMsgSequenceNumber,
                               kPortId);
  NL80211Attr<uint8_t> u8_attr(1, kU8Value1);
  netlink_packet.AddAttribute(u8_attr);
  EXPECT_TRUE(netlink_packet.IsValid());
  EXPECT_TRUE(netlink_packet.HasAttribute(1));
  uint8_t attr_value;
  EXPECT_TRUE(netlink_packet.GetAttributeValue(1, &attr_value));
  EXPECT_EQ(attr_value, kU8Value1);
}

TEST(NL80211PacketTest, AddMultipleAttributesToNL80211Packet) {
  NL80211Packet netlink_packet(kNLMsgType,
                               kGenNLCommand,
                               kNLMsgSequenceNumber,
                               kPortId);
  NL80211Attr<uint8_t> u8_attr(1, kU8Value1);
  NL80211Attr<uint32_t> u32_attr(2, kU32Value1);
  NL80211Attr<uint16_t> u16_attr(3, kU16Value1);
  netlink_packet.AddAttribute(u8_attr);
  netlink_packet.AddAttribute(u32_attr);
  netlink_packet.AddAttribute(u16_attr);
  EXPECT_TRUE(netlink_packet.IsValid());
  EXPECT_TRUE(netlink_packet.HasAttribute(1));
  EXPECT_TRUE(netlink_packet.HasAttribute(2));
  EXPECT_TRUE(netlink_packet.HasAttribute(3));
  uint8_t u8_attr_value;
  uint32_t u32_attr_value;
  uint16_t u16_attr_value;
  EXPECT_TRUE(netlink_packet.GetAttributeValue(1, &u8_attr_value));
  EXPECT_TRUE(netlink_packet.GetAttributeValue(2, &u32_attr_value));
  EXPECT_TRUE(netlink_packet.GetAttributeValue(3, &u16_attr_value));
  EXPECT_EQ(u8_attr_value, kU8Value1);
  EXPECT_EQ(u32_attr_value, kU32Value1);
  EXPECT_EQ(u16_attr_value, kU16Value1);
}

TEST(NL80211PacketTest, AddNestedAttributesToNL80211Packet) {
  NL80211Packet netlink_packet(kNLMsgType,
                               kGenNLCommand,
                               kNLMsgSequenceNumber,
                               kPortId);
  NL80211NestedAttr nested_attr(1);
  NL80211Attr<uint16_t> u16_attr(2, kU16Value1);
  NL80211Attr<uint32_t> u32_attr_1(3, kU32Value1);
  NL80211Attr<uint32_t> u32_attr_2(4, kU32Value2);
  nested_attr.AddAttribute(u16_attr);
  nested_attr.AddAttribute(u32_attr_1);
  netlink_packet.AddAttribute(nested_attr);
  netlink_packet.AddAttribute(u32_attr_2);
  EXPECT_TRUE(netlink_packet.HasAttribute(1));
  EXPECT_TRUE(netlink_packet.HasAttribute(4));
  // Attribute 2 and 3 are deeper nested.
  // They should not be found from packet level.
  EXPECT_FALSE(netlink_packet.HasAttribute(2));
  EXPECT_FALSE(netlink_packet.HasAttribute(3));
}

TEST(NL80211PacketTest, CannotGetMissingAttributeFromNL80211Packet) {
  NL80211Packet netlink_packet(kNLMsgType,
                               kGenNLCommand,
                               kNLMsgSequenceNumber,
                               kPortId);
  NL80211Attr<uint8_t> u8_attr(1, kU8Value1);
  netlink_packet.AddAttribute(u8_attr);
  EXPECT_TRUE(netlink_packet.IsValid());
  EXPECT_FALSE(netlink_packet.HasAttribute(2));
  uint8_t attr_value;
  EXPECT_FALSE(netlink_packet.GetAttributeValue(2, &attr_value));
}

TEST(NL80211PacketTest, CanGetAllOfAttributeFromNL80211Packet) {
  NL80211Packet netlink_packet(kNLMsgType,
                               kGenNLCommand,
                               kNLMsgSequenceNumber,
                               kPortId);
  NL80211Attr<uint8_t> u8_attr(1, kU8Value1);
  NL80211Attr<uint32_t> u32_attr_1(2, kU32Value1);
  NL80211Attr<uint32_t> u32_attr_2(4, kU32Value2);
  netlink_packet.AddAttribute(u8_attr);
  netlink_packet.AddAttribute(u32_attr_1);
  netlink_packet.AddAttribute(u32_attr_2);
  EXPECT_TRUE(netlink_packet.IsValid());
  std::vector<BaseNL80211Attr> attributes;
  EXPECT_TRUE(netlink_packet.GetAllAttributes(&attributes));

  EXPECT_TRUE(attributes.size() == 3);

  NL80211Attr<uint8_t>* u8_attr_retrieved =
      static_cast<NL80211Attr<uint8_t>*>(&attributes[0]);
  EXPECT_TRUE(u8_attr_retrieved->GetValue() == kU8Value1);

  NL80211Attr<uint32_t>* u32_attr_1_retrieved =
      static_cast<NL80211Attr<uint32_t>*>(&attributes[1]);
  EXPECT_TRUE(u32_attr_1_retrieved->GetValue() == kU32Value1);

  NL80211Attr<uint32_t>* u32_attr_2_retrieved =
      static_cast<NL80211Attr<uint32_t>*>(&attributes[2]);
  EXPECT_TRUE(u32_attr_2_retrieved->GetValue() == kU32Value2);
}

TEST(NL80211PacketTest, ParseCMDAssociateTest) {
  NL80211Packet netlink_packet(std::vector<uint8_t>(
      kNL80211_CMD_ASSOCIATE,
      kNL80211_CMD_ASSOCIATE + sizeof(kNL80211_CMD_ASSOCIATE)));
  EXPECT_TRUE(netlink_packet.IsValid());
  EXPECT_EQ(kNL80211FamilyId, netlink_packet.GetMessageType());
  EXPECT_EQ(NL80211_CMD_ASSOCIATE, netlink_packet.GetCommand());
  uint32_t value;
  EXPECT_TRUE(netlink_packet.GetAttributeValue(NL80211_ATTR_WIPHY, &value));
  EXPECT_EQ(kWiPhy, value);
  EXPECT_TRUE(netlink_packet.GetAttributeValue(NL80211_ATTR_IFINDEX, &value));
  EXPECT_EQ(kExpectedIfIndex, value);
  std::vector<uint8_t> rawdata;
  EXPECT_TRUE(netlink_packet.GetAttributeValue(NL80211_ATTR_FRAME, &rawdata));
  EXPECT_FALSE(rawdata.empty());

}

TEST(NL80211PacketTest, ParseCMDNotifyCQMTest) {
  NL80211Packet netlink_packet(std::vector<uint8_t>(
      kNL80211_CMD_NOTIFY_CQM,
      kNL80211_CMD_NOTIFY_CQM + sizeof(kNL80211_CMD_NOTIFY_CQM)));
  EXPECT_TRUE(netlink_packet.IsValid());
  EXPECT_EQ(kNL80211FamilyId, netlink_packet.GetMessageType());
  EXPECT_EQ(NL80211_CMD_NOTIFY_CQM, netlink_packet.GetCommand());
  uint32_t value;
  EXPECT_TRUE(netlink_packet.GetAttributeValue(NL80211_ATTR_WIPHY, &value));
  EXPECT_EQ(kWiPhy, value);
  EXPECT_TRUE(netlink_packet.GetAttributeValue(NL80211_ATTR_IFINDEX, &value));
  EXPECT_EQ(kExpectedIfIndex, value);
  std::vector<uint8_t> mac;
  EXPECT_TRUE(netlink_packet.GetAttributeValue(NL80211_ATTR_MAC, &mac));
  std::vector<uint8_t> expected_mac(
      kMacAddressBytes,
      kMacAddressBytes + sizeof(kMacAddressBytes));
  EXPECT_EQ(expected_mac, mac);
  NL80211NestedAttr nested(0);
  EXPECT_TRUE(netlink_packet.GetAttribute(NL80211_ATTR_CQM, &nested));
  EXPECT_TRUE(nested.GetAttributeValue(NL80211_ATTR_CQM_PKT_LOSS_EVENT, &value));
  EXPECT_EQ(kExpectedCqmNotAcked, value);
}

TEST(NL80211PacketTest, ParseCMDNewStation) {
  NL80211Packet netlink_packet(std::vector<uint8_t>(
      kNL80211_CMD_NEW_STATION,
      kNL80211_CMD_NEW_STATION + sizeof(kNL80211_CMD_NEW_STATION)));
  EXPECT_TRUE(netlink_packet.IsValid());
  EXPECT_EQ(kNL80211FamilyId, netlink_packet.GetMessageType());
  EXPECT_EQ(NL80211_CMD_NEW_STATION, netlink_packet.GetCommand());
  uint32_t value;
  EXPECT_TRUE(netlink_packet.GetAttributeValue(NL80211_ATTR_IFINDEX, &value));
  EXPECT_EQ(kExpectedIfIndex, value);
  std::vector<uint8_t> mac;
  EXPECT_TRUE(netlink_packet.GetAttributeValue(NL80211_ATTR_MAC, &mac));
  std::vector<uint8_t> expected_mac(
      kMacAddressBytes,
      kMacAddressBytes + sizeof(kMacAddressBytes));
  EXPECT_EQ(expected_mac, mac);
  NL80211NestedAttr nested(0);
  EXPECT_TRUE(netlink_packet.GetAttribute(NL80211_ATTR_STA_INFO, &nested));
  EXPECT_TRUE(netlink_packet.GetAttributeValue(NL80211_ATTR_GENERATION,
                                               &value));
  EXPECT_EQ(kNewStationExpectedGeneration, value);
}

}  // namespace wificond
}  // namespace android
