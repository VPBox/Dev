/******************************************************************************
 *
 *  Copyright 2017 Google, Inc.
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

#include "hci_packet.h"
#include "l2cap_packet.h"
#include "l2cap_test_packets.h"

#include <gtest/gtest.h>
#include <memory>

using std::shared_ptr;
using std::vector;

namespace test_vendor_lib {

class TestPacket : public HciPacket {
 public:
  TestPacket(const std::vector<uint8_t>& packet) {
    complete_packet_ = packet;
  }
  TestPacket() = default;

 private:
  std::vector<uint8_t> complete_packet_;
  size_t get_length() {
    return complete_packet_.size();
  }
  uint8_t& get_at_index(size_t index) {
    return complete_packet_[index];
  }
};

class L2capTest : public ::testing::Test {
 public:
  std::shared_ptr<L2capSdu> update_fcs(vector<uint8_t> sdu) {
    sdu.resize(sdu.size() - 2);

    return L2capSdu::L2capSduBuilder(sdu);
  }

  void compare_packets(shared_ptr<HciPacket> expected, shared_ptr<HciPacket> received) {
    Iterator expected_begin = expected->get_begin();
    Iterator expected_end = expected->get_end();

    Iterator received_begin = received->get_begin();
    Iterator received_end = received->get_end();

    ASSERT_EQ(expected_end - expected_begin, received_end - received_begin);

    while (expected_begin < expected_end) {
      ASSERT_EQ(*expected_begin, *received_begin);
      expected_begin++;
      received_begin++;
    }
  }
};

TEST_F(L2capTest, assembleGoodPackets) {
  vector<std::shared_ptr<L2capSdu> > test_packet;

  // Test 1: Pass correct packets.
  test_packet.push_back(L2capSdu::L2capSduConstructor(good_sdu[0]));
  test_packet.push_back(L2capSdu::L2capSduConstructor(good_sdu[1]));
  test_packet.push_back(L2capSdu::L2capSduConstructor(good_sdu[2]));

  shared_ptr<L2capPacket> test_1 = L2capPacket::assemble(test_packet);
  ASSERT_NE(test_1, nullptr);

  shared_ptr<TestPacket> expected(new TestPacket(good_l2cap_packet));

  compare_packets(expected, test_1);

  test_packet.clear();

  test_packet.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_1));
  test_packet.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_2));
  test_packet.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_3));
  test_packet.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_4));
  test_packet.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_5));
  test_packet.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_6));
  test_packet.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_7));
  test_packet.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_8));
  test_packet.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_9));

  test_1 = L2capPacket::assemble(test_packet);
  ASSERT_NE(test_1, nullptr);

  expected.reset(new TestPacket(complete_l2cap_packet));
  compare_packets(expected, test_1);

  test_packet.clear();
}

TEST_F(L2capTest, assembleOutofOrderPackets) {
  vector<std::shared_ptr<L2capSdu> > test_packet;

  // Test 2: Pass out of order packets.
  test_packet.push_back(L2capSdu::L2capSduConstructor(good_sdu[1]));
  test_packet.push_back(L2capSdu::L2capSduConstructor(good_sdu[0]));
  test_packet.push_back(L2capSdu::L2capSduConstructor(good_sdu[2]));

  shared_ptr<L2capPacket> test_2 = L2capPacket::assemble(test_packet);
  EXPECT_EQ(test_2, nullptr);

  test_packet.clear();

  test_packet.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_1));
  test_packet.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_3));
  test_packet.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_2));
  test_packet.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_6));
  test_packet.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_5));
  test_packet.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_4));
  test_packet.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_8));
  test_packet.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_7));
  test_packet.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_9));

  test_2 = L2capPacket::assemble(test_packet);
  EXPECT_EQ(test_2, nullptr);

  test_packet.clear();
}

TEST_F(L2capTest, assembleBadControlBytes) {
  vector<std::shared_ptr<L2capSdu> > test_packet;

  // Test 3: Pass packets missing the finished control bytes.
  test_packet.push_back(L2capSdu::L2capSduConstructor(good_sdu[0]));
  test_packet.push_back(L2capSdu::L2capSduConstructor(good_sdu[1]));

  shared_ptr<L2capPacket> test_3 = L2capPacket::assemble(test_packet);
  EXPECT_EQ(test_3, nullptr);

  test_packet.clear();

  test_packet.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_1));
  test_packet.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_2));
  test_packet.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_3));
  test_packet.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_4));
  test_packet.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_5));
  test_packet.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_6));
  test_packet.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_7));
  test_packet.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_8));

  test_3 = L2capPacket::assemble(test_packet);
  EXPECT_EQ(test_3, nullptr);

  test_packet.clear();
}

TEST_F(L2capTest, assembleBadFCS) {
  vector<std::shared_ptr<L2capSdu> > test_packet;

  // Test 4: Pass packets with incorrect frame check sequences.
  test_packet.push_back(L2capSdu::L2capSduConstructor(good_sdu[0]));
  good_sdu[1][good_sdu[1].size() - 1]++;
  test_packet.push_back(L2capSdu::L2capSduConstructor(good_sdu[1]));
  good_sdu[1][good_sdu[1].size() - 1]--;
  test_packet.push_back(L2capSdu::L2capSduConstructor(good_sdu[2]));

  shared_ptr<L2capPacket> test_4 = L2capPacket::assemble(test_packet);
  EXPECT_EQ(test_4, nullptr);

  test_packet.clear();

  test_packet.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_1));
  test_packet.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_2));
  test_packet.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_3));
  test_packet.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_4));
  l2cap_test_packet_5[l2cap_test_packet_5.size() - 1]++;
  test_packet.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_5));
  l2cap_test_packet_5[l2cap_test_packet_5.size() - 1]--;
  test_packet.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_6));
  test_packet.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_7));
  test_packet.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_8));
  test_packet.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_9));

  test_4 = L2capPacket::assemble(test_packet);
  EXPECT_EQ(test_4, nullptr);

  test_packet.clear();
}

TEST_F(L2capTest, assembleEmptyPayload) {
  vector<std::shared_ptr<L2capSdu> > test_packet;

  // Test 5: Pass a packet with an empty payload.
  test_packet.push_back(L2capSdu::L2capSduConstructor(empty_sdu_payload[0]));
  test_packet.push_back(L2capSdu::L2capSduConstructor(empty_sdu_payload[1]));

  shared_ptr<L2capPacket> test_5 = L2capPacket::assemble(test_packet);
  ASSERT_NE(test_5, nullptr);

  shared_ptr<TestPacket> expected(new TestPacket(empty_l2cap_payload));
  compare_packets(expected, test_5);

  test_packet.clear();
}

TEST_F(L2capTest, assembleAllStartingControlError) {
  vector<std::shared_ptr<L2capSdu> > test_packet;

  // Test 6: Pass a SDU with all the control bytes set to as the starting bytes.
  test_packet.push_back(L2capSdu::L2capSduConstructor(all_first_packet[0]));
  test_packet.push_back(L2capSdu::L2capSduConstructor(all_first_packet[1]));
  test_packet.push_back(L2capSdu::L2capSduConstructor(all_first_packet[2]));

  shared_ptr<L2capPacket> test_6 = L2capPacket::assemble(test_packet);
  EXPECT_EQ(test_6, nullptr);

  test_packet.clear();
}

TEST_F(L2capTest, assembleBadCID) {
  vector<std::shared_ptr<L2capSdu> > test_packet;

  // Test 7: Pass SDUs with mixed channel ids.
  test_packet.push_back(L2capSdu::L2capSduConstructor(good_sdu[0]));
  good_sdu[1][2]++;
  test_packet.push_back(update_fcs(good_sdu[1]));
  good_sdu[1][2]--;
  test_packet.push_back(L2capSdu::L2capSduConstructor(good_sdu[2]));

  shared_ptr<L2capPacket> test_7 = L2capPacket::assemble(test_packet);
  EXPECT_EQ(test_7, nullptr);

  test_packet.clear();

  test_packet.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_1));
  l2cap_test_packet_2[2]++;
  test_packet.push_back((update_fcs(l2cap_test_packet_2)));
  l2cap_test_packet_2[2]--;
  test_packet.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_3));
  test_packet.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_4));
  l2cap_test_packet_5[2]++;
  test_packet.push_back((update_fcs(l2cap_test_packet_5)));
  l2cap_test_packet_5[2]--;
  test_packet.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_6));
  test_packet.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_7));
  l2cap_test_packet_8[2]--;
  test_packet.push_back((update_fcs(l2cap_test_packet_8)));
  l2cap_test_packet_8[2]++;
  test_packet.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_9));

  test_7 = L2capPacket::assemble(test_packet);
  EXPECT_EQ(test_7, nullptr);

  test_packet.clear();
}

TEST_F(L2capTest, assembleUnsegmentedSDU) {
  vector<std::shared_ptr<L2capSdu> > test_packet;

  // Test 8: Pass a complete l2cap packet.
  test_packet.push_back(L2capSdu::L2capSduConstructor(one_sdu[0]));

  shared_ptr<L2capPacket> test_8 = L2capPacket::assemble(test_packet);
  EXPECT_NE(test_8, nullptr);

  test_packet.clear();
}

TEST_F(L2capTest, assembleBadTxSeq) {
  vector<std::shared_ptr<L2capSdu> > test_packet;

  // Test 9: Pass SDUs with incorrect TxSeq.
  good_sdu[0][4] += 4;
  test_packet.push_back((update_fcs(good_sdu[0])));
  good_sdu[0][4] -= 4;
  test_packet.push_back(L2capSdu::L2capSduConstructor(good_sdu[1]));
  test_packet.push_back(L2capSdu::L2capSduConstructor(good_sdu[2]));

  shared_ptr<L2capPacket> test_9 = L2capPacket::assemble(test_packet);
  EXPECT_EQ(test_9, nullptr);

  test_packet.clear();
}

TEST_F(L2capTest, assembleBadTotalSDULength) {
  vector<std::shared_ptr<L2capSdu> > test_packet;

  // Test 10: Pass SDUs with an incorrect total SDU length
  good_sdu[0][7]++;
  test_packet.push_back((update_fcs(good_sdu[0])));
  good_sdu[0][7]--;
  test_packet.push_back(L2capSdu::L2capSduConstructor(good_sdu[1]));
  test_packet.push_back(L2capSdu::L2capSduConstructor(good_sdu[2]));

  shared_ptr<L2capPacket> test_10 = L2capPacket::assemble(test_packet);
  EXPECT_EQ(test_10, nullptr);

  test_packet.clear();

  l2cap_test_packet_1[6]++;
  test_packet.push_back((update_fcs(l2cap_test_packet_1)));
  l2cap_test_packet_1[6]--;
  test_packet.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_2));
  test_packet.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_3));
  test_packet.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_4));
  test_packet.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_5));
  test_packet.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_6));
  test_packet.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_7));
  test_packet.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_8));
  test_packet.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_9));

  test_10 = L2capPacket::assemble(test_packet);

  EXPECT_EQ(test_10, nullptr);

  test_packet.clear();
}

// Begin Fragment Test1
TEST_F(L2capTest, fragmentSmallSegmentTest) {
  std::vector<std::shared_ptr<L2capSdu> > sdu;
  std::shared_ptr<L2capPacket> l2cap_expected;
  std::shared_ptr<L2capPacket> l2cap_received;

  sdu.push_back(L2capSdu::L2capSduConstructor(good_sdu[0]));
  sdu.push_back(L2capSdu::L2capSduConstructor(good_sdu[1]));
  sdu.push_back(L2capSdu::L2capSduConstructor(good_sdu[2]));

  l2cap_expected = L2capPacket::assemble(sdu);

  sdu.clear();

  // Test1: Small segments
  sdu = l2cap_expected->fragment(16, 0x02, 0x41);

  l2cap_received = L2capPacket::assemble(sdu);
  ASSERT_NE(l2cap_received, nullptr) << "packet reassembly failed after fragment" << std::endl
                                     << "Test1: Small Segment request" << std::endl
                                     << "sdu used: good_sdu" << std::endl
                                     << "function call: fragment(16, 0x02, 0x41)" << std::endl;

  compare_packets(l2cap_expected, l2cap_received);

  sdu.clear();
  l2cap_expected.reset();
  l2cap_received.reset();
}  // End Fragment Test1

// Begin Fragment Test2
TEST_F(L2capTest, fragmentLargeSegmentTest) {
  std::vector<std::shared_ptr<L2capSdu> > sdu;
  std::shared_ptr<L2capPacket> l2cap_expected;
  std::shared_ptr<L2capPacket> l2cap_received;

  sdu.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_1));
  sdu.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_2));
  sdu.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_3));
  sdu.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_4));
  sdu.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_5));
  sdu.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_6));
  sdu.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_7));
  sdu.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_8));
  sdu.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_9));

  l2cap_expected = L2capPacket::assemble(sdu);

  sdu.clear();

  // Test2: Large Segments
  sdu = l2cap_expected->fragment(1024, 0x02, 0x41);

  l2cap_received = L2capPacket::assemble(sdu);
  ASSERT_NE(l2cap_received, nullptr) << "packet reassembly failed after fragment" << std::endl
                                     << "Test2: Large Segment request" << std::endl
                                     << "sdu used: l2cap_test_packet[1-9]" << std::endl
                                     << "function call: fragment(1024, 0x02, 0x41)" << std::endl;

  compare_packets(l2cap_expected, l2cap_received);

  sdu.clear();
  l2cap_expected.reset();
  l2cap_received.reset();
}  // End Fragment Test2

// Begin Fragment Test3
TEST_F(L2capTest, fragmentTxSeqTest) {
  std::vector<std::shared_ptr<L2capSdu> > sdu;
  std::shared_ptr<L2capPacket> l2cap_expected;
  std::shared_ptr<L2capPacket> l2cap_received;

  sdu.push_back(L2capSdu::L2capSduConstructor(good_sdu[0]));
  sdu.push_back(L2capSdu::L2capSduConstructor(good_sdu[1]));
  sdu.push_back(L2capSdu::L2capSduConstructor(good_sdu[2]));

  l2cap_expected = L2capPacket::assemble(sdu);

  sdu.clear();

  // Test3: Non-zero starting TxSeq
  sdu = l2cap_expected->fragment(24, 0x08, 0x41);

  l2cap_received = L2capPacket::assemble(sdu);
  ASSERT_NE(l2cap_received, nullptr) << "packet reassembly failed after fragment" << std::endl
                                     << "Test3: Non-zero starting TxSeq" << std::endl
                                     << "sdu used: good_sdu" << std::endl
                                     << "function call: fragment(24, 0x08, 0x41)" << std::endl;

  compare_packets(l2cap_expected, l2cap_received);

  sdu.clear();
  l2cap_expected.reset();
  l2cap_received.reset();
}  // End Fragment Test3

// Begin Fragment Test4
TEST_F(L2capTest, fragmentPayloadTest) {
  std::vector<std::shared_ptr<L2capSdu> > sdu;
  std::shared_ptr<L2capPacket> l2cap_expected;
  std::shared_ptr<L2capPacket> l2cap_received;

  sdu.push_back(L2capSdu::L2capSduConstructor(empty_sdu_payload[0]));
  sdu.push_back(L2capSdu::L2capSduConstructor(empty_sdu_payload[1]));

  l2cap_expected = L2capPacket::assemble(sdu);

  sdu.clear();

  // Test4: Packet with no payload
  sdu = l2cap_expected->fragment(16, 0x02, 0x41);

  l2cap_received = L2capPacket::assemble(sdu);
  ASSERT_NE(l2cap_received, nullptr) << "packet reassembly failed after fragment" << std::endl
                                     << "Test4: Packet with no payload" << std::endl
                                     << "sdu used: empty_sdu_payload" << std::endl
                                     << "function call: fragment(16, 0x02, 0x41)" << std::endl;

  compare_packets(l2cap_expected, l2cap_received);

  sdu.clear();
  l2cap_expected.reset();
  l2cap_received.reset();
}  // End Fragment Test4

// Begin Fragment Test5
TEST_F(L2capTest, fragmentSegmentSizeTest) {
  std::vector<std::shared_ptr<L2capSdu> > sdu;
  std::shared_ptr<L2capPacket> l2cap_expected;
  std::shared_ptr<L2capPacket> l2cap_received;

  sdu.push_back(L2capSdu::L2capSduConstructor(good_sdu[0]));
  sdu.push_back(L2capSdu::L2capSduConstructor(good_sdu[1]));
  sdu.push_back(L2capSdu::L2capSduConstructor(good_sdu[2]));

  l2cap_expected = L2capPacket::assemble(sdu);

  sdu.clear();

  // Test5: Larger segment size than packet size
  sdu = l2cap_expected->fragment(256, 0x02, 0x41);

  l2cap_received = L2capPacket::assemble(sdu);
  ASSERT_NE(l2cap_received, nullptr) << "packet reassembly failed after fragment" << std::endl
                                     << "Test5: Segment size > Payload" << std::endl
                                     << "sdu used: good_sdu" << std::endl
                                     << "function call: fragment(256, 0x02, 0x41)" << std::endl;

  compare_packets(l2cap_expected, l2cap_received);

  sdu.clear();
  l2cap_expected.reset();
  l2cap_received.reset();
}  // End Fragment Test5

// Begin Fragment Test6
TEST_F(L2capTest, fragmentSegmentSizeTest2) {
  std::vector<std::shared_ptr<L2capSdu> > sdu;
  std::shared_ptr<L2capPacket> l2cap_expected;
  std::shared_ptr<L2capPacket> l2cap_received;

  sdu.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_1));
  sdu.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_2));
  sdu.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_3));
  sdu.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_4));
  sdu.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_5));
  sdu.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_6));
  sdu.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_7));
  sdu.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_8));
  sdu.push_back(L2capSdu::L2capSduConstructor(l2cap_test_packet_9));

  l2cap_expected = L2capPacket::assemble(sdu);
  sdu.clear();

  // Test6: Small segment size on large packet.
  sdu = l2cap_expected->fragment(512, 0x02, 0x41);

  l2cap_received = L2capPacket::assemble(sdu);
  ASSERT_NE(l2cap_received, nullptr) << "packet reassembly failed after fragment" << std::endl
                                     << "Test6: Small Segment request on large packet" << std::endl
                                     << "sdu used: l2cap_test_packet_[1-9]" << std::endl
                                     << "function call: fragment(64, 0x02, 0x41)" << std::endl;

  compare_packets(l2cap_expected, l2cap_received);

  sdu.clear();
  l2cap_expected.reset();
  l2cap_received.reset();
}  // End Fragment Test6

}  // namespace test_vendor_lib
