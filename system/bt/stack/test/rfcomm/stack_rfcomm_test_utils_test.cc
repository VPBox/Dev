/******************************************************************************
 *
 *  Copyright 2018 The Android Open Source Project
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

#include <string>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "rfcdefs.h"
#include "stack_rfcomm_test_utils.h"
#include "stack_test_packet_utils.h"

namespace {

using testing::ElementsAreArray;

using bluetooth::CreateL2capDataPacket;
using bluetooth::CreateAclPacket;
using bluetooth::AllocateWrappedIncomingL2capAclPacket;

using bluetooth::rfcomm::GetDlci;
using bluetooth::rfcomm::GetAddressField;
using bluetooth::rfcomm::GetControlField;
using bluetooth::rfcomm::CreateMccPnFrame;
using bluetooth::rfcomm::CreateMccMscFrame;
using bluetooth::rfcomm::CreateMultiplexerControlFrame;
using bluetooth::rfcomm::CreateRfcommPacket;
using bluetooth::rfcomm::CreateQuickDataPacket;
using bluetooth::rfcomm::CreateQuickPnPacket;
using bluetooth::rfcomm::CreateQuickSabmPacket;
using bluetooth::rfcomm::CreateQuickUaPacket;
using bluetooth::rfcomm::CreateQuickMscPacket;

const uint8_t kIncomingSabmChannel0[] = {0x08, 0x20, 0x08, 0x00, 0x04, 0x00,
                                         0x5c, 0x00, 0x03, 0x3f, 0x01, 0x1c};

TEST(RfcommTestPacketGeneratorTest, TestGenerateSabmChannel0Packet) {
  uint8_t dlci = GetDlci(false, 0);
  EXPECT_EQ(dlci, 0);
  uint8_t address_field = GetAddressField(true, true, dlci);
  EXPECT_EQ(address_field, 0x03);
  uint8_t control_field = GetControlField(true, RFCOMM_SABME);
  EXPECT_EQ(control_field, 0x3F);
  std::vector<uint8_t> rfcomm_packet =
      CreateRfcommPacket(address_field, control_field, -1, {});
  std::vector<uint8_t> l2cap_packet =
      CreateL2capDataPacket(0x005c, rfcomm_packet);
  std::vector<uint8_t> acl_packet =
      CreateAclPacket(0x0008, 0b10, 0b00, l2cap_packet);
  EXPECT_THAT(acl_packet, ElementsAreArray(kIncomingSabmChannel0));
}

TEST(RfcommTestPacketGeneratorTest, TestQuickGenerateSabmChannel0Packet) {
  EXPECT_THAT(CreateQuickSabmPacket(RFCOMM_MX_DLCI, 0x005c, 0x0008),
              ElementsAreArray(kIncomingSabmChannel0));
}

const uint8_t kOutgoingUaChannel0[] = {0x08, 0x20, 0x08, 0x00, 0x04, 0x00,
                                       0x00, 0x17, 0x03, 0x73, 0x01, 0xd7};

TEST(RfcommTestPacketGeneratorTest, TestGenerateUaPacket) {
  uint8_t dlci = GetDlci(false, 0);
  EXPECT_EQ(dlci, 0);
  uint8_t address_field = GetAddressField(true, true, dlci);
  EXPECT_EQ(address_field, 0x03);
  uint8_t control_field = GetControlField(true, RFCOMM_UA);
  EXPECT_EQ(control_field, 0x73);
  std::vector<uint8_t> rfcomm_packet =
      CreateRfcommPacket(address_field, control_field, -1, {});
  std::vector<uint8_t> l2cap_packet =
      CreateL2capDataPacket(0x1700, rfcomm_packet);
  std::vector<uint8_t> acl_packet =
      CreateAclPacket(0x0008, 0b10, 0b00, l2cap_packet);
  EXPECT_THAT(acl_packet, ElementsAreArray(kOutgoingUaChannel0));
}

TEST(RfcommTestPacketGeneratorTest, TestQuickGenerateUaPacket) {
  EXPECT_THAT(CreateQuickUaPacket(RFCOMM_MX_DLCI, 0x1700, 0x0008),
              ElementsAreArray(kOutgoingUaChannel0));
}

const uint8_t kIncomingUihPnSetChannelTo3[] = {
    0x08, 0x20, 0x12, 0x00, 0x0e, 0x00, 0x5c, 0x00, 0x03, 0xef, 0x15,
    0x83, 0x11, 0x06, 0xf0, 0x00, 0x00, 0x74, 0x03, 0x00, 0x01, 0x70};

TEST(RfcommTestPacketGeneratorTest, TestGenerateUihPnSetChannel3Packet) {
  uint8_t dlci = GetDlci(false, 0);
  EXPECT_EQ(dlci, 0);
  uint8_t address_field = GetAddressField(true, true, dlci);
  EXPECT_EQ(address_field, 0x03);
  uint8_t control_field = GetControlField(false, RFCOMM_UIH);
  EXPECT_EQ(control_field, 0xEF);
  uint8_t new_dlci = GetDlci(false, 3);
  EXPECT_EQ(new_dlci, 6);
  std::vector<uint8_t> mcc_pn_data =
      CreateMccPnFrame(new_dlci, 0x0, 0xF, 0, 0, 884, 0, 1);
  std::vector<uint8_t> mcc_payload =
      CreateMultiplexerControlFrame(0x20, true, mcc_pn_data);
  std::vector<uint8_t> rfcomm_packet =
      CreateRfcommPacket(address_field, control_field, -1, mcc_payload);
  std::vector<uint8_t> l2cap_packet =
      CreateL2capDataPacket(0x005c, rfcomm_packet);
  std::vector<uint8_t> acl_packet =
      CreateAclPacket(0x0008, 0b10, 0b00, l2cap_packet);
  EXPECT_THAT(acl_packet, ElementsAreArray(kIncomingUihPnSetChannelTo3));
}

TEST(RfcommTestPacketGeneratorTest, TestQuickGenerateUihPnSetChannel3Packet) {
  EXPECT_THAT(CreateQuickPnPacket(true, GetDlci(false, 3), true, 884, 0xF, 0, 1,
                                  0x005c, 0x0008),
              ElementsAreArray(kIncomingUihPnSetChannelTo3));
}

const uint8_t kIncomingSabmChannel3[] = {0x08, 0x20, 0x08, 0x00, 0x04, 0x00,
                                         0x5c, 0x00, 0x1b, 0x3f, 0x01, 0xd3};

TEST(RfcommTestPacketGeneratorTest, TestGenerateSabmChannel3Packet) {
  uint8_t dlci = GetDlci(false, 3);
  EXPECT_EQ(dlci, 6);
  uint8_t address_field = GetAddressField(true, true, dlci);
  EXPECT_EQ(address_field, 0x1b);
  uint8_t control_field = GetControlField(true, RFCOMM_SABME);
  EXPECT_EQ(control_field, 0x3F);
  std::vector<uint8_t> rfcomm_packet =
      CreateRfcommPacket(address_field, control_field, -1, {});
  std::vector<uint8_t> l2cap_packet =
      CreateL2capDataPacket(0x005c, rfcomm_packet);
  std::vector<uint8_t> acl_packet =
      CreateAclPacket(0x0008, 0b10, 0b00, l2cap_packet);
  EXPECT_THAT(acl_packet, ElementsAreArray(kIncomingSabmChannel3));
}

TEST(RfcommTestPacketGeneratorTest, TestGenerateQuickSabmChannel3Packet) {
  EXPECT_THAT(CreateQuickSabmPacket(GetDlci(false, 3), 0x005c, 0x0008),
              ElementsAreArray(kIncomingSabmChannel3));
}

const uint8_t kIncomingUihMscCmdFrame[] = {0x08, 0x20, 0x0c, 0x00, 0x08, 0x00,
                                           0x5c, 0x00, 0x03, 0xef, 0x09, 0xe3,
                                           0x05, 0x1b, 0x8d, 0x70};

TEST(RfcommTestPacketGeneratorTest, TestGenerateUihMscCmdPacket) {
  uint8_t dlci = GetDlci(false, 0);
  EXPECT_EQ(dlci, 0);
  uint8_t address_field = GetAddressField(true, true, dlci);
  EXPECT_EQ(address_field, 0x03);
  uint8_t control_field = GetControlField(false, RFCOMM_UIH);
  EXPECT_EQ(control_field, 0xEF);
  uint8_t new_dlci = GetDlci(false, 3);
  EXPECT_EQ(new_dlci, 6);
  std::vector<uint8_t> mcc_msc_data =
      CreateMccMscFrame(new_dlci, false, true, true, false, true);
  std::vector<uint8_t> mcc_payload =
      CreateMultiplexerControlFrame(0x38, true, mcc_msc_data);
  std::vector<uint8_t> rfcomm_packet =
      CreateRfcommPacket(address_field, control_field, -1, mcc_payload);
  std::vector<uint8_t> l2cap_packet =
      CreateL2capDataPacket(0x005c, rfcomm_packet);
  std::vector<uint8_t> acl_packet =
      CreateAclPacket(0x0008, 0b10, 0b00, l2cap_packet);
  EXPECT_THAT(acl_packet, ElementsAreArray(kIncomingUihMscCmdFrame));
}

TEST(RfcommTestPacketGeneratorTest, TestQuickGenerateUihMscCmdPacket) {
  EXPECT_THAT(CreateQuickMscPacket(true, GetDlci(false, 3), 0x005c, 0x0008,
                                   true, false, true, true, false, true),
              ElementsAreArray(kIncomingUihMscCmdFrame));
}

const uint8_t kIncomingUihMscResponseFrame[] = {
    0x08, 0x20, 0x0c, 0x00, 0x08, 0x00, 0x5c, 0x00,
    0x03, 0xef, 0x09, 0xe1, 0x05, 0x1b, 0x8d, 0x70};

TEST(RfcommTestPacketGeneratorTest, TestGenerateUihMscResponsePacket) {
  uint8_t dlci = GetDlci(false, 0);
  EXPECT_EQ(dlci, 0);
  uint8_t address_field = GetAddressField(true, true, dlci);
  EXPECT_EQ(address_field, 0x03);
  uint8_t control_field = GetControlField(false, RFCOMM_UIH);
  EXPECT_EQ(control_field, 0xEF);
  uint8_t new_dlci = GetDlci(false, 3);
  EXPECT_EQ(new_dlci, 6);
  std::vector<uint8_t> mcc_msc_data =
      CreateMccMscFrame(new_dlci, false, true, true, false, true);
  std::vector<uint8_t> mcc_payload =
      CreateMultiplexerControlFrame(0x38, false, mcc_msc_data);
  std::vector<uint8_t> rfcomm_packet =
      CreateRfcommPacket(address_field, control_field, -1, mcc_payload);
  std::vector<uint8_t> l2cap_packet =
      CreateL2capDataPacket(0x005c, rfcomm_packet);
  std::vector<uint8_t> acl_packet =
      CreateAclPacket(0x0008, 0b10, 0b00, l2cap_packet);
  EXPECT_THAT(acl_packet, ElementsAreArray(kIncomingUihMscResponseFrame));
}

TEST(RfcommTestPacketGeneratorTest, TestQuickGenerateUihMscResponsePacket) {
  EXPECT_THAT(CreateQuickMscPacket(true, GetDlci(false, 3), 0x005c, 0x0008,
                                   false, false, true, true, false, true),
              ElementsAreArray(kIncomingUihMscResponseFrame));
}

const uint8_t kIncomingBrsfFrame[] = {0x08, 0x20, 0x15, 0x00, 0x11, 0x00, 0x5c,
                                      0x00, 0x1b, 0xff, 0x19, 0x06, 0x41, 0x54,
                                      0x2b, 0x42, 0x52, 0x53, 0x46, 0x3d, 0x39,
                                      0x35, 0x39, 0x0d, 0x93};

TEST(RfcommTestPacketGeneratorTest, TestGenerateDataPacket) {
  uint8_t dlci = GetDlci(false, 3);
  EXPECT_EQ(dlci, 6);
  uint8_t address_field = GetAddressField(true, true, dlci);
  EXPECT_EQ(address_field, 0x1B);
  uint8_t control_field = GetControlField(true, RFCOMM_UIH);
  EXPECT_EQ(control_field, 0xFF);
  const std::string data_str = "AT+BRSF=959\r";
  const std::vector<uint8_t> data(data_str.begin(), data_str.end());
  std::vector<uint8_t> rfcomm_packet =
      CreateRfcommPacket(address_field, control_field, 6, data);
  std::vector<uint8_t> l2cap_packet =
      CreateL2capDataPacket(0x005c, rfcomm_packet);
  std::vector<uint8_t> acl_packet =
      CreateAclPacket(0x0008, 0b10, 0b00, l2cap_packet);
  EXPECT_THAT(acl_packet, ElementsAreArray(kIncomingBrsfFrame));
}

TEST(RfcommTestPacketGeneratorTest, TestQuickGenerateDataPacket) {
  EXPECT_THAT(CreateQuickDataPacket(GetDlci(false, 3), true, 0x005c, 0x0008, 6,
                                    "AT+BRSF=959\r"),
              ElementsAreArray(kIncomingBrsfFrame));
}

}  // namespace