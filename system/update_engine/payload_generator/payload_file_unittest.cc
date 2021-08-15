//
// Copyright (C) 2015 The Android Open Source Project
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

#include "update_engine/payload_generator/payload_file.h"

#include <string>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

#include "update_engine/common/test_utils.h"
#include "update_engine/payload_generator/extent_ranges.h"

using std::string;
using std::vector;

namespace chromeos_update_engine {

class PayloadFileTest : public ::testing::Test {
 protected:
  PayloadFile payload_;
};

TEST_F(PayloadFileTest, ReorderBlobsTest) {
  test_utils::ScopedTempFile orig_blobs("ReorderBlobsTest.orig.XXXXXX");

  // The operations have three blob and one gap (the whitespace):
  // Rootfs operation 1: [8, 3] bcd
  // Rootfs operation 2: [7, 1] a
  // Kernel operation 1: [0, 6] kernel
  string orig_data = "kernel abcd";
  EXPECT_TRUE(test_utils::WriteFileString(orig_blobs.path(), orig_data));

  test_utils::ScopedTempFile new_blobs("ReorderBlobsTest.new.XXXXXX");

  payload_.part_vec_.resize(2);

  vector<AnnotatedOperation> aops;
  AnnotatedOperation aop;
  aop.op.set_data_offset(8);
  aop.op.set_data_length(3);
  aops.push_back(aop);

  aop.op.set_data_offset(7);
  aop.op.set_data_length(1);
  aops.push_back(aop);
  payload_.part_vec_[0].aops = aops;

  aop.op.set_data_offset(0);
  aop.op.set_data_length(6);
  payload_.part_vec_[1].aops = {aop};

  EXPECT_TRUE(payload_.ReorderDataBlobs(orig_blobs.path(), new_blobs.path()));

  const vector<AnnotatedOperation>& part0_aops = payload_.part_vec_[0].aops;
  const vector<AnnotatedOperation>& part1_aops = payload_.part_vec_[1].aops;
  string new_data;
  EXPECT_TRUE(utils::ReadFile(new_blobs.path(), &new_data));
  // Kernel blobs should appear at the end.
  EXPECT_EQ("bcdakernel", new_data);

  EXPECT_EQ(2U, part0_aops.size());
  EXPECT_EQ(0U, part0_aops[0].op.data_offset());
  EXPECT_EQ(3U, part0_aops[0].op.data_length());
  EXPECT_EQ(3U, part0_aops[1].op.data_offset());
  EXPECT_EQ(1U, part0_aops[1].op.data_length());

  EXPECT_EQ(1U, part1_aops.size());
  EXPECT_EQ(4U, part1_aops[0].op.data_offset());
  EXPECT_EQ(6U, part1_aops[0].op.data_length());
}

}  // namespace chromeos_update_engine
