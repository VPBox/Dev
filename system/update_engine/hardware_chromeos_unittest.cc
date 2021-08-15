//
// Copyright (C) 2016 The Android Open Source Project
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

#include "update_engine/hardware_chromeos.h"

#include <memory>

#include <base/files/file_util.h>
#include <base/files/scoped_temp_dir.h>
#include <gtest/gtest.h>

#include "update_engine/common/constants.h"
#include "update_engine/common/fake_hardware.h"
#include "update_engine/common/test_utils.h"
#include "update_engine/update_manager/umtest_utils.h"

using chromeos_update_engine::test_utils::WriteFileString;
using std::string;

namespace chromeos_update_engine {

class HardwareChromeOSTest : public ::testing::Test {
 protected:
  void SetUp() override { ASSERT_TRUE(root_dir_.CreateUniqueTempDir()); }

  void WriteStatefulConfig(const string& config) {
    base::FilePath kFile(root_dir_.GetPath().value() + kStatefulPartition +
                         "/etc/update_manager.conf");
    ASSERT_TRUE(base::CreateDirectory(kFile.DirName()));
    ASSERT_TRUE(WriteFileString(kFile.value(), config));
  }

  void WriteRootfsConfig(const string& config) {
    base::FilePath kFile(root_dir_.GetPath().value() +
                         "/etc/update_manager.conf");
    ASSERT_TRUE(base::CreateDirectory(kFile.DirName()));
    ASSERT_TRUE(WriteFileString(kFile.value(), config));
  }

  // Helper method to call HardwareChromeOS::LoadConfig with the test directory.
  void CallLoadConfig(bool normal_mode) {
    hardware_.LoadConfig(root_dir_.GetPath().value(), normal_mode);
  }

  HardwareChromeOS hardware_;
  base::ScopedTempDir root_dir_;
};

TEST_F(HardwareChromeOSTest, NoFileFoundReturnsDefault) {
  CallLoadConfig(true /* normal_mode */);
  EXPECT_TRUE(hardware_.IsOOBEEnabled());
}

TEST_F(HardwareChromeOSTest, DontReadStatefulInNormalMode) {
  WriteStatefulConfig("is_oobe_enabled=false");

  CallLoadConfig(true /* normal_mode */);
  EXPECT_TRUE(hardware_.IsOOBEEnabled());
}

TEST_F(HardwareChromeOSTest, ReadStatefulInDevMode) {
  WriteRootfsConfig("is_oobe_enabled=true");
  // Since the stateful is present, we should read that one.
  WriteStatefulConfig("is_oobe_enabled=false");

  CallLoadConfig(false /* normal_mode */);
  EXPECT_FALSE(hardware_.IsOOBEEnabled());
}

TEST_F(HardwareChromeOSTest, ReadRootfsIfStatefulNotFound) {
  WriteRootfsConfig("is_oobe_enabled=false");

  CallLoadConfig(false /* normal_mode */);
  EXPECT_FALSE(hardware_.IsOOBEEnabled());
}

}  // namespace chromeos_update_engine
