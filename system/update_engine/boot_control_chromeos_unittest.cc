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

#include "update_engine/boot_control_chromeos.h"

#include <gtest/gtest.h>

namespace chromeos_update_engine {

class BootControlChromeOSTest : public ::testing::Test {
 protected:
  void SetUp() override {
    // We don't run Init() for bootctl_, we set its internal values instead.
    bootctl_.num_slots_ = 2;
    bootctl_.current_slot_ = 0;
    bootctl_.boot_disk_name_ = "/dev/null";
  }

  BootControlChromeOS bootctl_;  // BootControlChromeOS under test.
};

TEST_F(BootControlChromeOSTest, SysfsBlockDeviceTest) {
  EXPECT_EQ("/sys/block/sda", bootctl_.SysfsBlockDevice("/dev/sda"));
  EXPECT_EQ("", bootctl_.SysfsBlockDevice("/foo/sda"));
  EXPECT_EQ("", bootctl_.SysfsBlockDevice("/dev/foo/bar"));
  EXPECT_EQ("", bootctl_.SysfsBlockDevice("/"));
  EXPECT_EQ("", bootctl_.SysfsBlockDevice("./"));
  EXPECT_EQ("", bootctl_.SysfsBlockDevice(""));
}

TEST_F(BootControlChromeOSTest, GetPartitionNumberTest) {
  // The partition name should not be case-sensitive.
  EXPECT_EQ(2, bootctl_.GetPartitionNumber("kernel", 0));
  EXPECT_EQ(2, bootctl_.GetPartitionNumber("boot", 0));
  EXPECT_EQ(2, bootctl_.GetPartitionNumber("KERNEL", 0));
  EXPECT_EQ(2, bootctl_.GetPartitionNumber("BOOT", 0));

  EXPECT_EQ(3, bootctl_.GetPartitionNumber("ROOT", 0));
  EXPECT_EQ(3, bootctl_.GetPartitionNumber("system", 0));

  EXPECT_EQ(3, bootctl_.GetPartitionNumber("ROOT", 0));
  EXPECT_EQ(3, bootctl_.GetPartitionNumber("system", 0));

  // Slot B.
  EXPECT_EQ(4, bootctl_.GetPartitionNumber("KERNEL", 1));
  EXPECT_EQ(5, bootctl_.GetPartitionNumber("ROOT", 1));

  // Slot C doesn't exists.
  EXPECT_EQ(-1, bootctl_.GetPartitionNumber("KERNEL", 2));
  EXPECT_EQ(-1, bootctl_.GetPartitionNumber("ROOT", 2));

  // Non A/B partitions are ignored.
  EXPECT_EQ(-1, bootctl_.GetPartitionNumber("OEM", 0));
  EXPECT_EQ(-1, bootctl_.GetPartitionNumber("A little panda", 0));
}

}  // namespace chromeos_update_engine
