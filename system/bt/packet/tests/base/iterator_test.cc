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
#include <memory>

#include "packet.h"
#include "packet_test_common.h"
#include "test_packets.h"

namespace bluetooth {

using pair = std::pair<size_t, size_t>;

class IteratorTest
    : public ::testing::TestWithParam<std::pair<size_t, size_t>> {
 public:
  std::shared_ptr<TestPacket> GetTestPacket() {
    auto bounds = GetParam();
    auto lower_bound = bounds.first;
    auto upper_bound = bounds.second;

    return TestPacket::Make(test_l2cap_data, lower_bound, upper_bound);
  }

  size_t GetTestPacketLength() { return GetParam().second - GetParam().first; }

  size_t GetLowerBound() { return GetParam().first; }

  size_t GetUpperBound() { return GetParam().second; }
};

INSTANTIATE_TEST_CASE_P(IteratorParameterTest, IteratorTest,
                        ::testing::Values(pair(0, test_l2cap_data.size()),
                                          pair(3, test_l2cap_data.size() - 2)));

TEST_F(IteratorTest, iteratorCreateDeathTest) {
  auto packet =
      TestPacket::Make(test_l2cap_data, 3, test_l2cap_data.size() - 2);
  ASSERT_DEATH(Iterator(packet, 0), "index_ >= packet->packet_start_index_");
  ASSERT_DEATH(Iterator(packet, test_l2cap_data.size()),
               "index_ <= packet->packet_end_index_");
}

TEST_F(IteratorTest, extractTest) {
  auto packet = TestPacket::Make(test_l2cap_data);
  Iterator general_case = packet->begin();

  ASSERT_EQ(0x02u, general_case.extract<uint8_t>());
  ASSERT_EQ(0x2edcu, general_case.extract<uint16_t>());
  ASSERT_EQ(0x00620066u, general_case.extract<uint32_t>());
  ASSERT_EQ(0x00010000000a0013u, general_case.extract<uint64_t>());
}

TEST_F(IteratorTest, extractBETest) {
  auto packet = TestPacket::Make(test_l2cap_data);
  Iterator general_case = packet->begin();

  ASSERT_EQ(0x02u, general_case.extractBE<uint8_t>());
  ASSERT_EQ(0xdc2eu, general_case.extractBE<uint16_t>());
  ASSERT_EQ(0x66006200u, general_case.extractBE<uint32_t>());
  ASSERT_EQ(0x13000a0000000100u, general_case.extractBE<uint64_t>());
}

TEST_P(IteratorTest, payloadBoundsTest) {
  auto packet = GetTestPacket();
  ASSERT_EQ(static_cast<size_t>(packet->end() - packet->begin()),
            GetTestPacketLength());

  auto it = packet->begin();
  for (size_t i = 0; i < GetTestPacketLength(); i++) {
    ASSERT_EQ(test_l2cap_data[i + GetLowerBound()], *it++);
  }
}

TEST_P(IteratorTest, extractBoundsDeathTest) {
  auto packet = GetTestPacket();
  Iterator bounds_test = packet->end();
  ASSERT_DEATH(bounds_test.extract<uint8_t>(),
               "index_ != packet_->packet_end_index_");
  ASSERT_DEATH(bounds_test.extract<uint16_t>(),
               "index_ != packet_->packet_end_index_");
  ASSERT_DEATH(bounds_test.extract<uint32_t>(),
               "index_ != packet_->packet_end_index_");
  ASSERT_DEATH(bounds_test.extract<uint64_t>(),
               "index_ != packet_->packet_end_index_");
}

TEST_P(IteratorTest, extractBEBoundsDeathTest) {
  auto packet = GetTestPacket();
  Iterator bounds_test = packet->end();
  ASSERT_DEATH(bounds_test.extractBE<uint8_t>(),
               "index_ != packet_->packet_end_index_");
  ASSERT_DEATH(bounds_test.extractBE<uint16_t>(),
               "index_ != packet_->packet_end_index_");
  ASSERT_DEATH(bounds_test.extractBE<uint32_t>(),
               "index_ != packet_->packet_end_index_");
  ASSERT_DEATH(bounds_test.extractBE<uint64_t>(),
               "index_ != packet_->packet_end_index_");
}

TEST_P(IteratorTest, dereferenceDeathTest) {
  auto packet = GetTestPacket();
  Iterator dereference_test = packet->end();

  ASSERT_EQ((*packet)[GetTestPacketLength() - 1],
            *(dereference_test - static_cast<size_t>(1)));
  ASSERT_DEATH(*dereference_test, "index_ != packet_->packet_end_index_");
}

TEST_P(IteratorTest, plusEqTest) {
  auto packet = GetTestPacket();
  Iterator plus_eq = packet->begin();
  for (size_t i = 0; i < GetTestPacketLength(); i += 2) {
    ASSERT_EQ(test_l2cap_data[i + GetLowerBound()], *plus_eq)
        << "+= test: Dereferenced iterator does not equal expected at index "
        << i;
    plus_eq += 2;
  }

  ASSERT_EQ(plus_eq, packet->end());
}

TEST_P(IteratorTest, preIncrementTest) {
  auto packet = GetTestPacket();
  Iterator plus_plus = packet->begin();
  for (size_t i = 0; i < GetTestPacketLength() - 1; i++) {
    ASSERT_EQ(test_l2cap_data[i + GetLowerBound() + 1], *(++plus_plus))
        << "Pre-increment test: Dereferenced iterator does not equal expected "
        << "at index " << i;
  }
}

TEST_P(IteratorTest, postIncrementTest) {
  auto packet = GetTestPacket();
  Iterator plus_plus = packet->begin();
  for (size_t i = 0; i < GetTestPacketLength(); i++) {
    ASSERT_EQ(test_l2cap_data[i + GetLowerBound()], *(plus_plus++))
        << "Post-increment test: Dereferenced iterator does not equal expected "
        << "at index " << i;
  }
}

TEST_P(IteratorTest, additionTest) {
  auto packet = GetTestPacket();
  Iterator plus = packet->begin();
  for (size_t i = 0; i < GetTestPacketLength(); i++) {
    ASSERT_EQ(test_l2cap_data[i + GetLowerBound()], *plus)
        << "+ test: Dereferenced iterator does not equal expected at index "
        << i;
    plus = plus + static_cast<size_t>(1);
  }
}

TEST_P(IteratorTest, minusEqTest) {
  auto packet = GetTestPacket();
  Iterator minus_eq = packet->end();
  minus_eq -= 1;
  for (int i = GetTestPacketLength() - 1; i > 0; i -= 2) {
    ASSERT_EQ(test_l2cap_data[static_cast<size_t>(i) + GetLowerBound()],
              *minus_eq)
        << "-= test: Dereferenced iterator does not equal expected at index "
        << i;
    minus_eq -= 2;
  }
}

TEST_P(IteratorTest, preDecrementTest) {
  auto packet = GetTestPacket();
  Iterator minus_minus = packet->end();
  for (int i = GetTestPacketLength(); i > 0; i--) {
    ASSERT_EQ(test_l2cap_data[static_cast<size_t>(i) + GetLowerBound() - 1],
              *(--minus_minus))
        << "Pre-decrement test: Dereferenced iterator does not equal expected "
        << "at index " << i;
  }
}

TEST_P(IteratorTest, postDecrementTest) {
  auto packet = GetTestPacket();
  Iterator minus_minus = packet->end();
  minus_minus--;
  for (int i = GetTestPacketLength() - 1; i > 0; i--) {
    ASSERT_EQ(test_l2cap_data[static_cast<size_t>(i) + GetLowerBound()],
              *(minus_minus--))
        << "Post-decrement test: Dereferenced iterator does not equal expected "
        << "at index " << i;
  }
}

TEST_P(IteratorTest, subtractionTest) {
  auto packet = GetTestPacket();
  Iterator minus = packet->end();
  minus = minus - static_cast<size_t>(1);
  for (int i = GetTestPacketLength() - 1; i > 0; i--) {
    ASSERT_EQ(test_l2cap_data[static_cast<size_t>(i) + GetLowerBound()], *minus)
        << "- test: Dereferenced iterator does not equal expected at index "
        << i;
    minus = minus - static_cast<size_t>(1);
  }
}

TEST_P(IteratorTest, plusEqBoundsTest) {
  auto packet = GetTestPacket();
  Iterator plus_eq = packet->end();
  for (size_t i = 0; i < 100; i++) {
    plus_eq += i;
    ASSERT_EQ(packet->end(), plus_eq)
        << "+= test: Iterator exceeded the upper bound set by get_length()";
  }
}

TEST_P(IteratorTest, preIncrementBoundsTest) {
  auto packet = GetTestPacket();
  Iterator plus_plus = packet->end();
  plus_plus--;
  for (size_t i = 0; i < 100; i++) {
    ASSERT_EQ(packet->end(), ++plus_plus)
        << "Pre-increment test: Iterator exceeded the upper bound set "
           "by get_length()";
  }
}

TEST_P(IteratorTest, postIncrementBoundsTest) {
  auto packet = GetTestPacket();
  Iterator plus_plus = packet->end();
  for (size_t i = 0; i < 100; i++) {
    ASSERT_EQ(packet->end(), plus_plus++)
        << "Post-increment test: Iterator exceeded the upper bound set "
           "by get_length()";
  }
}

TEST_P(IteratorTest, additionBoundsTest) {
  auto packet = GetTestPacket();
  Iterator plus = packet->end();
  for (size_t i = 0; i < 100; i++) {
    plus = plus + static_cast<size_t>(i);
    ASSERT_EQ(packet->end(), plus)
        << "+ test: Iterator exceeded the upper bound set by get_length()";
  }
}

TEST_P(IteratorTest, minusEqBoundsTest) {
  auto packet = GetTestPacket();
  Iterator minus_eq = packet->begin();
  for (size_t i = 0; i < 100; i++) {
    minus_eq -= i;
    ASSERT_EQ(test_l2cap_data[GetLowerBound()], *minus_eq)
        << "-= test: Iterator is less than the lower bound set by "
           "packet->begin()";
  }
}

TEST_P(IteratorTest, preDecrementBoundsTest) {
  auto packet = GetTestPacket();
  Iterator minus_minus = packet->begin();
  for (size_t i = 0; i < 100; i++) {
    ASSERT_EQ(test_l2cap_data[GetLowerBound()], *(--minus_minus))
        << "Pre-decrement test: Iterator is less than the lower bound set by "
           "packet->begin()";
  }
}

TEST_P(IteratorTest, postDecrementBoundsTest) {
  auto packet = GetTestPacket();
  Iterator minus_minus = packet->begin();
  for (size_t i = 0; i < 100; i++) {
    ASSERT_EQ(test_l2cap_data[GetLowerBound()], *(minus_minus--))
        << "Post-decrement test: Iterator is less than the lower bound set by "
           "packet->begin()";
  }
}

TEST_P(IteratorTest, subtractionBoundsTest) {
  auto packet = GetTestPacket();
  Iterator minus = packet->begin();
  for (size_t i = 0; i < 100; i++) {
    minus = minus - static_cast<size_t>(i);
    ASSERT_EQ(test_l2cap_data[GetLowerBound()], *minus)
        << "- test: Iterator is less than the lower bound set "
           "by packet->begin()";
  }
}

}  // namespace bluetooth
