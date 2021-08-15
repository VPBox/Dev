//
// Copyright (C) 2017 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include "update_engine/payload_generator/deflate_utils.h"

#include <unistd.h>

#include <algorithm>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

#include "update_engine/common/test_utils.h"
#include "update_engine/common/utils.h"
#include "update_engine/payload_generator/extent_ranges.h"
#include "update_engine/payload_generator/extent_utils.h"

using puffin::BitExtent;
using puffin::ByteExtent;
using std::vector;

namespace chromeos_update_engine {
namespace deflate_utils {

// This creates a sudo-random BitExtents from ByteExtents for simpler testing.
vector<BitExtent> ByteToBitExtent(const vector<ByteExtent>& byte_extents) {
  vector<BitExtent> bit_extents;
  for (auto& byte_extent : byte_extents) {
    bit_extents.emplace_back(byte_extent.offset * 8 + (byte_extent.offset & 7),
                             byte_extent.length * 8 - (byte_extent.length & 7));
  }
  return bit_extents;
}

TEST(DeflateUtilsTest, ExtentsShiftTest) {
  vector<Extent> base_extents = {ExtentForRange(10, 10),
                                 ExtentForRange(70, 10),
                                 ExtentForRange(50, 10),
                                 ExtentForRange(30, 10),
                                 ExtentForRange(90, 10)};
  vector<Extent> over_extents = {ExtentForRange(2, 2),
                                 ExtentForRange(5, 2),
                                 ExtentForRange(7, 3),
                                 ExtentForRange(13, 10),
                                 ExtentForRange(25, 20),
                                 ExtentForRange(47, 3)};
  vector<Extent> out_over_extents = {ExtentForRange(12, 2),
                                     ExtentForRange(15, 2),
                                     ExtentForRange(17, 3),
                                     ExtentForRange(73, 7),
                                     ExtentForRange(50, 3),
                                     ExtentForRange(55, 5),
                                     ExtentForRange(30, 10),
                                     ExtentForRange(90, 5),
                                     ExtentForRange(97, 3)};
  EXPECT_TRUE(ShiftExtentsOverExtents(base_extents, &over_extents));
  EXPECT_EQ(over_extents, out_over_extents);

  // Failure case
  base_extents = {ExtentForRange(10, 10)};
  over_extents = {ExtentForRange(2, 12)};
  EXPECT_FALSE(ShiftExtentsOverExtents(base_extents, &over_extents));
}

TEST(DeflateUtilsTest, ShiftBitExtentsOverExtentsTest) {
  vector<Extent> base_extents = {ExtentForRange(3, 1),
                                 ExtentForRange(1, 1),
                                 ExtentForRange(5, 1),
                                 ExtentForRange(7, 1),
                                 ExtentForRange(9, 1)};
  vector<BitExtent> over_extents =
      ByteToBitExtent({{0, 0}, {100, 2000}, {4096, 0}, {5000, 5000}});
  vector<BitExtent> out_over_extents =
      ByteToBitExtent({{12288, 0}, {12388, 2000}, {4096, 0}});
  ASSERT_TRUE(ShiftBitExtentsOverExtents(base_extents, &over_extents));
  EXPECT_EQ(over_extents, out_over_extents);
}

TEST(DeflateUtilsTest, ShiftBitExtentsOverExtentsBoundaryTest) {
  vector<Extent> base_extents = {ExtentForRange(1, 1)};
  vector<BitExtent> over_extents = ByteToBitExtent({{2, 4096}});
  vector<BitExtent> out_over_extents = {};
  EXPECT_FALSE(ShiftBitExtentsOverExtents(base_extents, &over_extents));

  base_extents = {ExtentForRange(1, 1)};
  over_extents = {};
  out_over_extents = {};
  EXPECT_TRUE(ShiftBitExtentsOverExtents(base_extents, &over_extents));
  EXPECT_EQ(over_extents, out_over_extents);

  base_extents = {};
  over_extents = {};
  out_over_extents = {};
  EXPECT_TRUE(ShiftBitExtentsOverExtents(base_extents, &over_extents));
  EXPECT_EQ(over_extents, out_over_extents);

  base_extents = {};
  over_extents = ByteToBitExtent({{0, 1}});
  out_over_extents = ByteToBitExtent({{0, 1}});
  EXPECT_FALSE(ShiftBitExtentsOverExtents(base_extents, &over_extents));
  EXPECT_EQ(over_extents, out_over_extents);

  base_extents = {ExtentForRange(1, 2)};
  over_extents = ByteToBitExtent({{0, 3 * 4096}, {4 * 4096, 4096}});
  out_over_extents = ByteToBitExtent({{0, 3 * 4096}, {4 * 4096, 4096}});
  EXPECT_FALSE(ShiftBitExtentsOverExtents(base_extents, &over_extents));
  EXPECT_EQ(over_extents, out_over_extents);
}

TEST(DeflateUtilsTest, FindDeflatesTest) {
  vector<Extent> extents = {
      ExtentForRange(1, 1), ExtentForRange(3, 1), ExtentForRange(5, 1)};
  vector<BitExtent> in_deflates = ByteToBitExtent({{0, 0},
                                                   {10, 400},
                                                   {4096, 0},
                                                   {3000, 2000},
                                                   {4096, 100},
                                                   {4097, 100},
                                                   {8100, 92},
                                                   {8100, 93},
                                                   {8100, 6000},
                                                   {25000, 1}});
  vector<BitExtent> expected_out_deflates =
      ByteToBitExtent({{4096, 0}, {4096, 100}, {4097, 100}, {8100, 92}});
  vector<BitExtent> out_deflates;
  out_deflates = FindDeflates(extents, in_deflates);
  EXPECT_EQ(out_deflates, expected_out_deflates);
}

TEST(DeflateUtilsTest, FindDeflatesBoundaryTest) {
  vector<Extent> extents = {};
  vector<BitExtent> in_deflates = ByteToBitExtent({{0, 0}, {8100, 93}});
  vector<BitExtent> expected_out_deflates = {};
  vector<BitExtent> out_deflates;
  out_deflates = FindDeflates(extents, in_deflates);
  EXPECT_EQ(out_deflates, expected_out_deflates);

  extents = {};
  in_deflates = {};
  out_deflates = FindDeflates(extents, in_deflates);
  EXPECT_EQ(out_deflates, expected_out_deflates);
}

TEST(DeflateUtilsTest, CompactTest) {
  vector<Extent> extents = {
      ExtentForRange(1, 1), ExtentForRange(5, 1), ExtentForRange(3, 1)};
  vector<BitExtent> in_deflates =
      ByteToBitExtent({{4096, 0}, {12288, 4096}, {4096, 100}, {20480, 100}});
  vector<BitExtent> expected_out_deflates =
      ByteToBitExtent({{0, 0}, {0, 100}, {4096, 100}, {8192, 4096}});
  vector<BitExtent> out_deflates;
  ASSERT_TRUE(CompactDeflates(extents, in_deflates, &out_deflates));
  EXPECT_EQ(out_deflates, expected_out_deflates);
}

TEST(DeflateUtilsTest, CompactBoundaryTest) {
  vector<Extent> extents = {};
  vector<BitExtent> in_deflates = ByteToBitExtent({{4096, 0}});
  vector<BitExtent> expected_out_deflates = {};
  vector<BitExtent> out_deflates;
  EXPECT_FALSE(CompactDeflates(extents, in_deflates, &out_deflates));
  EXPECT_EQ(out_deflates, expected_out_deflates);

  extents = {};
  in_deflates = {};
  ASSERT_TRUE(CompactDeflates(extents, in_deflates, &out_deflates));
  EXPECT_EQ(out_deflates, expected_out_deflates);

  extents = {ExtentForRange(1, 1)};
  in_deflates = {};
  ASSERT_TRUE(CompactDeflates(extents, in_deflates, &out_deflates));
  EXPECT_EQ(out_deflates, expected_out_deflates);
}

}  // namespace deflate_utils
}  // namespace chromeos_update_engine
