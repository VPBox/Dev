/*
 * Copyright 2017 The Android Open Source Project
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

#include "l2cap_sdu.h"

#include <gtest/gtest.h>
#include <memory>

#include "l2cap_test_packets.h"

using std::vector;

namespace test_vendor_lib {

std::shared_ptr<L2capSdu> packet_1 = L2capSdu::L2capSduConstructor(l2cap_test_packet_1);
std::shared_ptr<L2capSdu> packet_2 = L2capSdu::L2capSduConstructor(l2cap_test_packet_2);
std::shared_ptr<L2capSdu> packet_3 = L2capSdu::L2capSduConstructor(l2cap_test_packet_3);
std::shared_ptr<L2capSdu> packet_4 = L2capSdu::L2capSduConstructor(l2cap_test_packet_4);
std::shared_ptr<L2capSdu> packet_5 = L2capSdu::L2capSduConstructor(l2cap_test_packet_5);
std::shared_ptr<L2capSdu> packet_6 = L2capSdu::L2capSduConstructor(l2cap_test_packet_6);
std::shared_ptr<L2capSdu> packet_7 = L2capSdu::L2capSduConstructor(l2cap_test_packet_7);
std::shared_ptr<L2capSdu> packet_8 = L2capSdu::L2capSduConstructor(l2cap_test_packet_8);
std::shared_ptr<L2capSdu> packet_9 = L2capSdu::L2capSduConstructor(l2cap_test_packet_9);

class L2capSduTest : public ::testing::Test {
 public:
  L2capSduTest(){};

  ~L2capSduTest() = default;

};  // L2capSduTest

TEST_F(L2capSduTest, getFcsTest) {
  EXPECT_EQ(0x72aa, packet_1->get_fcs());
  EXPECT_EQ(0x5b57, packet_2->get_fcs());
  EXPECT_EQ(0xe644, packet_3->get_fcs());
  EXPECT_EQ(0x21b0, packet_4->get_fcs());
  EXPECT_EQ(0xae96, packet_5->get_fcs());
  EXPECT_EQ(0x9254, packet_6->get_fcs());
  EXPECT_EQ(0xf6fa, packet_7->get_fcs());
  EXPECT_EQ(0x1da4, packet_8->get_fcs());
  EXPECT_EQ(0x781a, packet_9->get_fcs());
}

TEST_F(L2capSduTest, getPayloadLengthTest) {
  EXPECT_EQ(l2cap_test_packet_1.size() - 4, packet_1->get_payload_length());
  EXPECT_EQ(l2cap_test_packet_2.size() - 4, packet_2->get_payload_length());
  EXPECT_EQ(l2cap_test_packet_3.size() - 4, packet_3->get_payload_length());
  EXPECT_EQ(l2cap_test_packet_4.size() - 4, packet_4->get_payload_length());
  EXPECT_EQ(l2cap_test_packet_5.size() - 4, packet_5->get_payload_length());
  EXPECT_EQ(l2cap_test_packet_6.size() - 4, packet_6->get_payload_length());
  EXPECT_EQ(l2cap_test_packet_7.size() - 4, packet_7->get_payload_length());
  EXPECT_EQ(l2cap_test_packet_8.size() - 4, packet_8->get_payload_length());
  EXPECT_EQ(l2cap_test_packet_9.size() - 4, packet_9->get_payload_length());
}

TEST_F(L2capSduTest, calculateFcsTest) {
  EXPECT_EQ(0x72aa, packet_1->calculate_fcs());
  EXPECT_EQ(0x5b57, packet_2->calculate_fcs());
  EXPECT_EQ(0xe644, packet_3->calculate_fcs());
  EXPECT_EQ(0x21b0, packet_4->calculate_fcs());
  EXPECT_EQ(0xae96, packet_5->calculate_fcs());
  EXPECT_EQ(0x9254, packet_6->calculate_fcs());
  EXPECT_EQ(0xf6fa, packet_7->calculate_fcs());
  EXPECT_EQ(0x1da4, packet_8->calculate_fcs());
  EXPECT_EQ(0x781a, packet_9->calculate_fcs());
}

TEST_F(L2capSduTest, getControlsTest) {
  EXPECT_EQ(0x4102, packet_1->get_controls());
  EXPECT_EQ(0xc104, packet_2->get_controls());
  EXPECT_EQ(0xc106, packet_3->get_controls());
  EXPECT_EQ(0xc108, packet_4->get_controls());
  EXPECT_EQ(0xc10a, packet_5->get_controls());
  EXPECT_EQ(0xc10c, packet_6->get_controls());
  EXPECT_EQ(0xc10e, packet_7->get_controls());
  EXPECT_EQ(0xc110, packet_8->get_controls());
  EXPECT_EQ(0x8112, packet_9->get_controls());
}

TEST_F(L2capSduTest, getTotalLengthTest) {
  EXPECT_EQ(0x1f95, packet_1->get_total_l2cap_length());
}

TEST_F(L2capSduTest, getVectorSizeTest) {
  EXPECT_EQ(l2cap_test_packet_1.size(), packet_1->get_vector_size());
  EXPECT_EQ(l2cap_test_packet_2.size(), packet_2->get_vector_size());
  EXPECT_EQ(l2cap_test_packet_3.size(), packet_3->get_vector_size());
  EXPECT_EQ(l2cap_test_packet_4.size(), packet_4->get_vector_size());
  EXPECT_EQ(l2cap_test_packet_5.size(), packet_5->get_vector_size());
  EXPECT_EQ(l2cap_test_packet_6.size(), packet_6->get_vector_size());
  EXPECT_EQ(l2cap_test_packet_7.size(), packet_7->get_vector_size());
  EXPECT_EQ(l2cap_test_packet_8.size(), packet_8->get_vector_size());
  EXPECT_EQ(l2cap_test_packet_9.size(), packet_9->get_vector_size());
}

TEST_F(L2capSduTest, getCidTest) {
  EXPECT_EQ(0x0047, packet_1->get_channel_id());
  EXPECT_EQ(0x0047, packet_2->get_channel_id());
  EXPECT_EQ(0x0047, packet_3->get_channel_id());
  EXPECT_EQ(0x0047, packet_4->get_channel_id());
  EXPECT_EQ(0x0047, packet_5->get_channel_id());
  EXPECT_EQ(0x0047, packet_6->get_channel_id());
  EXPECT_EQ(0x0047, packet_7->get_channel_id());
  EXPECT_EQ(0x0047, packet_8->get_channel_id());
  EXPECT_EQ(0x0047, packet_9->get_channel_id());
}

}  // namespace test_vendor_lib
