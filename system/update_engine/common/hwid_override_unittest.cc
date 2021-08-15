//
// Copyright (C) 2012 The Android Open Source Project
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

#include "update_engine/common/hwid_override.h"

#include <string>

#include <base/files/file_path.h>
#include <base/files/file_util.h>
#include <base/files/scoped_temp_dir.h>
#include <gtest/gtest.h>

namespace chromeos_update_engine {

class HwidOverrideTest : public ::testing::Test {
 public:
  HwidOverrideTest() {}
  ~HwidOverrideTest() override = default;

  void SetUp() override {
    ASSERT_TRUE(tempdir_.CreateUniqueTempDir());
    ASSERT_TRUE(base::CreateDirectory(tempdir_.GetPath().Append("etc")));
  }

 protected:
  base::ScopedTempDir tempdir_;

 private:
  DISALLOW_COPY_AND_ASSIGN(HwidOverrideTest);
};

TEST_F(HwidOverrideTest, ReadGood) {
  std::string expected_hwid("expected");
  std::string keyval(HwidOverride::kHwidOverrideKey);
  keyval += ("=" + expected_hwid);
  ASSERT_EQ(base::WriteFile(tempdir_.GetPath().Append("etc/lsb-release"),
                            keyval.c_str(),
                            keyval.length()),
            static_cast<int>(keyval.length()));
  EXPECT_EQ(expected_hwid, HwidOverride::Read(tempdir_.GetPath()));
}

TEST_F(HwidOverrideTest, ReadNothing) {
  std::string keyval("SOMETHING_ELSE=UNINTERESTING");
  ASSERT_EQ(base::WriteFile(tempdir_.GetPath().Append("etc/lsb-release"),
                            keyval.c_str(),
                            keyval.length()),
            static_cast<int>(keyval.length()));
  EXPECT_EQ(std::string(), HwidOverride::Read(tempdir_.GetPath()));
}

TEST_F(HwidOverrideTest, ReadFailure) {
  EXPECT_EQ(std::string(), HwidOverride::Read(tempdir_.GetPath()));
}

}  // namespace chromeos_update_engine
