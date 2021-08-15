//
// Copyright (C) 2010 The Android Open Source Project
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

#include "update_engine/payload_generator/extent_ranges.h"

#include <vector>

#include <gtest/gtest.h>

#include "update_engine/common/test_utils.h"
#include "update_engine/payload_consumer/payload_constants.h"
#include "update_engine/payload_generator/extent_utils.h"

using std::vector;

namespace chromeos_update_engine {

class ExtentRangesTest : public ::testing::Test {};

namespace {
void ExpectRangeEq(const ExtentRanges& ranges,
                   const uint64_t* expected,
                   size_t sz,
                   int line) {
  uint64_t blocks = 0;
  for (size_t i = 1; i < sz; i += 2) {
    blocks += expected[i];
  }
  EXPECT_EQ(blocks, ranges.blocks()) << "line: " << line;

  const ExtentRanges::ExtentSet& result = ranges.extent_set();
  ExtentRanges::ExtentSet::const_iterator it = result.begin();
  for (size_t i = 0; i < sz; i += 2) {
    EXPECT_FALSE(it == result.end()) << "line: " << line;
    EXPECT_EQ(expected[i], it->start_block()) << "line: " << line;
    EXPECT_EQ(expected[i + 1], it->num_blocks()) << "line: " << line;
    ++it;
  }
}

#define EXPECT_RANGE_EQ(ranges, var)                      \
  do {                                                    \
    ExpectRangeEq(ranges, var, arraysize(var), __LINE__); \
  } while (0)

void ExpectRangesOverlapOrTouch(uint64_t a_start,
                                uint64_t a_num,
                                uint64_t b_start,
                                uint64_t b_num) {
  EXPECT_TRUE(ExtentRanges::ExtentsOverlapOrTouch(
      ExtentForRange(a_start, a_num), ExtentForRange(b_start, b_num)));
  EXPECT_TRUE(ExtentRanges::ExtentsOverlapOrTouch(
      ExtentForRange(b_start, b_num), ExtentForRange(a_start, a_num)));
}

void ExpectFalseRangesOverlapOrTouch(uint64_t a_start,
                                     uint64_t a_num,
                                     uint64_t b_start,
                                     uint64_t b_num) {
  EXPECT_FALSE(ExtentRanges::ExtentsOverlapOrTouch(
      ExtentForRange(a_start, a_num), ExtentForRange(b_start, b_num)));
  EXPECT_FALSE(ExtentRanges::ExtentsOverlapOrTouch(
      ExtentForRange(b_start, b_num), ExtentForRange(a_start, a_num)));
  EXPECT_FALSE(ExtentRanges::ExtentsOverlap(ExtentForRange(a_start, a_num),
                                            ExtentForRange(b_start, b_num)));
  EXPECT_FALSE(ExtentRanges::ExtentsOverlap(ExtentForRange(b_start, b_num),
                                            ExtentForRange(a_start, a_num)));
}

void ExpectRangesOverlap(uint64_t a_start,
                         uint64_t a_num,
                         uint64_t b_start,
                         uint64_t b_num) {
  EXPECT_TRUE(ExtentRanges::ExtentsOverlap(ExtentForRange(a_start, a_num),
                                           ExtentForRange(b_start, b_num)));
  EXPECT_TRUE(ExtentRanges::ExtentsOverlap(ExtentForRange(b_start, b_num),
                                           ExtentForRange(a_start, a_num)));
  EXPECT_TRUE(ExtentRanges::ExtentsOverlapOrTouch(
      ExtentForRange(a_start, a_num), ExtentForRange(b_start, b_num)));
  EXPECT_TRUE(ExtentRanges::ExtentsOverlapOrTouch(
      ExtentForRange(b_start, b_num), ExtentForRange(a_start, a_num)));
}

void ExpectFalseRangesOverlap(uint64_t a_start,
                              uint64_t a_num,
                              uint64_t b_start,
                              uint64_t b_num) {
  EXPECT_FALSE(ExtentRanges::ExtentsOverlap(ExtentForRange(a_start, a_num),
                                            ExtentForRange(b_start, b_num)));
  EXPECT_FALSE(ExtentRanges::ExtentsOverlap(ExtentForRange(b_start, b_num),
                                            ExtentForRange(a_start, a_num)));
}

}  // namespace

TEST(ExtentRangesTest, ExtentsOverlapTest) {
  ExpectRangesOverlapOrTouch(10, 20, 30, 10);
  ExpectRangesOverlap(10, 20, 25, 10);
  ExpectFalseRangesOverlapOrTouch(10, 20, 35, 10);
  ExpectFalseRangesOverlap(10, 20, 30, 10);
  ExpectRangesOverlap(12, 4, 12, 3);

  ExpectRangesOverlapOrTouch(kSparseHole, 2, kSparseHole, 3);
  ExpectRangesOverlap(kSparseHole, 2, kSparseHole, 3);
  ExpectFalseRangesOverlapOrTouch(kSparseHole, 2, 10, 3);
  ExpectFalseRangesOverlapOrTouch(10, 2, kSparseHole, 3);
  ExpectFalseRangesOverlap(kSparseHole, 2, 10, 3);
  ExpectFalseRangesOverlap(10, 2, kSparseHole, 3);
}

TEST(ExtentRangesTest, SimpleTest) {
  ExtentRanges ranges;
  {
    static const uint64_t expected[] = {};
    // Can't use arraysize() on 0-length arrays:
    ExpectRangeEq(ranges, expected, 0, __LINE__);
  }
  ranges.SubtractBlock(2);
  {
    static const uint64_t expected[] = {};
    // Can't use arraysize() on 0-length arrays:
    ExpectRangeEq(ranges, expected, 0, __LINE__);
  }

  ranges.AddBlock(0);
  ranges.Dump();
  ranges.AddBlock(1);
  ranges.AddBlock(3);

  {
    static const uint64_t expected[] = {0, 2, 3, 1};
    EXPECT_RANGE_EQ(ranges, expected);
  }
  ranges.AddBlock(2);
  {
    static const uint64_t expected[] = {0, 4};
    EXPECT_RANGE_EQ(ranges, expected);
    ranges.AddBlock(kSparseHole);
    EXPECT_RANGE_EQ(ranges, expected);
    ranges.SubtractBlock(kSparseHole);
    EXPECT_RANGE_EQ(ranges, expected);
  }
  ranges.SubtractBlock(2);
  {
    static const uint64_t expected[] = {0, 2, 3, 1};
    EXPECT_RANGE_EQ(ranges, expected);
  }

  for (uint64_t i = 100; i < 1000; i += 100) {
    ranges.AddExtent(ExtentForRange(i, 50));
  }
  {
    static const uint64_t expected[] = {0,   2,  3,   1,  100, 50, 200, 50,
                                        300, 50, 400, 50, 500, 50, 600, 50,
                                        700, 50, 800, 50, 900, 50};
    EXPECT_RANGE_EQ(ranges, expected);
  }

  ranges.SubtractExtent(ExtentForRange(210, 410 - 210));
  {
    static const uint64_t expected[] = {0,   2,   3,   1,   100, 50,  200,
                                        10,  410, 40,  500, 50,  600, 50,
                                        700, 50,  800, 50,  900, 50};
    EXPECT_RANGE_EQ(ranges, expected);
  }
  ranges.AddExtent(ExtentForRange(100000, 0));
  {
    static const uint64_t expected[] = {0,   2,   3,   1,   100, 50,  200,
                                        10,  410, 40,  500, 50,  600, 50,
                                        700, 50,  800, 50,  900, 50};
    EXPECT_RANGE_EQ(ranges, expected);
  }
  ranges.SubtractExtent(ExtentForRange(3, 0));
  {
    static const uint64_t expected[] = {0,   2,   3,   1,   100, 50,  200,
                                        10,  410, 40,  500, 50,  600, 50,
                                        700, 50,  800, 50,  900, 50};
    EXPECT_RANGE_EQ(ranges, expected);
  }
}

TEST(ExtentRangesTest, MultipleRanges) {
  ExtentRanges ranges_a, ranges_b;
  ranges_a.AddBlock(0);
  ranges_b.AddBlock(4);
  ranges_b.AddBlock(3);
  {
    uint64_t expected[] = {3, 2};
    EXPECT_RANGE_EQ(ranges_b, expected);
  }
  ranges_a.AddRanges(ranges_b);
  {
    uint64_t expected[] = {0, 1, 3, 2};
    EXPECT_RANGE_EQ(ranges_a, expected);
  }
  ranges_a.SubtractRanges(ranges_b);
  {
    uint64_t expected[] = {0, 1};
    EXPECT_RANGE_EQ(ranges_a, expected);
  }
  {
    uint64_t expected[] = {3, 2};
    EXPECT_RANGE_EQ(ranges_b, expected);
  }
}

TEST(ExtentRangesTest, GetExtentsForBlockCountTest) {
  ExtentRanges ranges;
  ranges.AddExtents(vector<Extent>(1, ExtentForRange(10, 30)));
  {
    vector<Extent> zero_extents = ranges.GetExtentsForBlockCount(0);
    EXPECT_TRUE(zero_extents.empty());
  }
  ::google::protobuf::RepeatedPtrField<Extent> rep_field;
  *rep_field.Add() = ExtentForRange(30, 40);
  ranges.AddRepeatedExtents(rep_field);
  ranges.SubtractExtents(vector<Extent>(1, ExtentForRange(20, 10)));
  *rep_field.Mutable(0) = ExtentForRange(50, 10);
  ranges.SubtractRepeatedExtents(rep_field);
  EXPECT_EQ(40U, ranges.blocks());

  for (int i = 0; i < 2; i++) {
    vector<Extent> expected(2);
    expected[0] = ExtentForRange(10, 10);
    expected[1] = ExtentForRange(30, i == 0 ? 10 : 20);
    vector<Extent> actual =
        ranges.GetExtentsForBlockCount(10 + expected[1].num_blocks());
    EXPECT_EQ(expected.size(), actual.size());
    for (vector<Extent>::size_type j = 0, e = expected.size(); j != e; ++j) {
      EXPECT_EQ(expected[j].start_block(), actual[j].start_block())
          << "j = " << j;
      EXPECT_EQ(expected[j].num_blocks(), actual[j].num_blocks())
          << "j = " << j;
    }
  }
}

TEST(ExtentRangesTest, ContainsBlockTest) {
  ExtentRanges ranges;
  EXPECT_FALSE(ranges.ContainsBlock(123));

  ranges.AddExtent(ExtentForRange(10, 10));
  ranges.AddExtent(ExtentForRange(100, 1));

  EXPECT_FALSE(ranges.ContainsBlock(9));
  EXPECT_TRUE(ranges.ContainsBlock(10));
  EXPECT_TRUE(ranges.ContainsBlock(15));
  EXPECT_TRUE(ranges.ContainsBlock(19));
  EXPECT_FALSE(ranges.ContainsBlock(20));

  // Test for an extent with just the block we are requesting.
  EXPECT_FALSE(ranges.ContainsBlock(99));
  EXPECT_TRUE(ranges.ContainsBlock(100));
  EXPECT_FALSE(ranges.ContainsBlock(101));
}

TEST(ExtentRangesTest, FilterExtentRangesEmptyRanges) {
  ExtentRanges ranges;
  EXPECT_EQ(vector<Extent>(), FilterExtentRanges(vector<Extent>(), ranges));
  EXPECT_EQ(vector<Extent>{ExtentForRange(50, 10)},
            FilterExtentRanges(vector<Extent>{ExtentForRange(50, 10)}, ranges));
  // Check that the empty Extents are ignored.
  EXPECT_EQ((vector<Extent>{ExtentForRange(10, 10), ExtentForRange(20, 10)}),
            FilterExtentRanges(vector<Extent>{ExtentForRange(10, 10),
                                              ExtentForRange(3, 0),
                                              ExtentForRange(20, 10)},
                               ranges));
}

TEST(ExtentRangesTest, FilterExtentRangesMultipleRanges) {
  // Two overlapping extents, with three ranges to remove.
  vector<Extent> extents{ExtentForRange(10, 100), ExtentForRange(30, 100)};
  ExtentRanges ranges;
  // This overlaps the beginning of the second extent.
  ranges.AddExtent(ExtentForRange(28, 3));
  ranges.AddExtent(ExtentForRange(50, 10));
  ranges.AddExtent(ExtentForRange(70, 10));
  // This overlaps the end of the second extent.
  ranges.AddExtent(ExtentForRange(108, 6));
  EXPECT_EQ((vector<Extent>{// For the first extent:
                            ExtentForRange(10, 18),
                            ExtentForRange(31, 19),
                            ExtentForRange(60, 10),
                            ExtentForRange(80, 28),
                            // For the second extent:
                            ExtentForRange(31, 19),
                            ExtentForRange(60, 10),
                            ExtentForRange(80, 28),
                            ExtentForRange(114, 16)}),
            FilterExtentRanges(extents, ranges));
}

TEST(ExtentRangesTest, FilterExtentRangesOvelapping) {
  ExtentRanges ranges;
  ranges.AddExtent(ExtentForRange(10, 3));
  ranges.AddExtent(ExtentForRange(20, 5));
  // Requested extent overlaps with one of the ranges.
  EXPECT_EQ(vector<Extent>(),
            FilterExtentRanges(
                vector<Extent>{ExtentForRange(10, 1), ExtentForRange(22, 1)},
                ranges));
}

}  // namespace chromeos_update_engine
