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

#include "update_engine/image_properties.h"

#include <string>

#include <base/files/file_util.h>
#include <base/files/scoped_temp_dir.h>
#include <gtest/gtest.h>

#include "update_engine/common/constants.h"
#include "update_engine/common/fake_prefs.h"
#include "update_engine/common/test_utils.h"
#include "update_engine/fake_system_state.h"

using chromeos_update_engine::test_utils::WriteFileString;
using std::string;

namespace chromeos_update_engine {

class ImagePropertiesTest : public ::testing::Test {
 protected:
  void SetUp() override {
    // Create a uniquely named test directory.
    ASSERT_TRUE(tempdir_.CreateUniqueTempDir());
    osrelease_dir_ = tempdir_.GetPath().Append("etc/os-release.d");
    EXPECT_TRUE(base::CreateDirectory(osrelease_dir_));
    test::SetImagePropertiesRootPrefix(tempdir_.GetPath().value().c_str());
  }

  void WriteOsRelease(const string& key, const string& value) {
    ASSERT_TRUE(WriteFileString(osrelease_dir_.Append(key).value(), value));
  }

  void WriteChannel(const string& channel) {
    string misc(2080, '\0');
    misc += channel;
    misc.resize(4096);
    ASSERT_TRUE(
        WriteFileString(tempdir_.GetPath().Append("misc").value(), misc));
  }

  FakeSystemState fake_system_state_;

  base::ScopedTempDir tempdir_;
  base::FilePath osrelease_dir_;
};

TEST_F(ImagePropertiesTest, SimpleTest) {
  WriteOsRelease("product_id", "abc");
  WriteOsRelease("system_id", "def");
  WriteOsRelease("product_version", "1.2.3.4");
  WriteOsRelease("system_version", "5.6.7.8");
  ImageProperties props = LoadImageProperties(&fake_system_state_);
  EXPECT_EQ("abc", props.product_id);
  EXPECT_EQ("def", props.system_id);
  EXPECT_EQ("1.2.3.4", props.version);
  EXPECT_EQ("5.6.7.8", props.system_version);
  EXPECT_EQ("stable-channel", props.current_channel);
  EXPECT_EQ(constants::kOmahaDefaultProductionURL, props.omaha_url);
}

TEST_F(ImagePropertiesTest, IDPrefixTest) {
  WriteOsRelease("product_id", "abc:def");
  WriteOsRelease("system_id", "foo:bar");
  ImageProperties props = LoadImageProperties(&fake_system_state_);
  EXPECT_EQ("abc:def", props.product_id);
  EXPECT_EQ("abc:bar", props.system_id);
}

TEST_F(ImagePropertiesTest, IDInvalidPrefixTest) {
  WriteOsRelease("product_id", "def");
  WriteOsRelease("system_id", "foo:bar");
  ImageProperties props = LoadImageProperties(&fake_system_state_);
  EXPECT_EQ("def", props.product_id);
  EXPECT_EQ("foo:bar", props.system_id);

  WriteOsRelease("product_id", "abc:def");
  WriteOsRelease("system_id", "bar");
  props = LoadImageProperties(&fake_system_state_);
  EXPECT_EQ("abc:def", props.product_id);
  EXPECT_EQ("bar", props.system_id);
}

TEST_F(ImagePropertiesTest, LoadChannelTest) {
  WriteChannel("unittest-channel");
  ImageProperties props = LoadImageProperties(&fake_system_state_);
  EXPECT_EQ("unittest-channel", props.current_channel);
}

TEST_F(ImagePropertiesTest, DefaultStableChannelTest) {
  WriteChannel("");
  ImageProperties props = LoadImageProperties(&fake_system_state_);
  EXPECT_EQ("stable-channel", props.current_channel);
}

TEST_F(ImagePropertiesTest, StoreLoadMutableChannelTest) {
  FakePrefs prefs;
  fake_system_state_.set_prefs(&prefs);
  WriteChannel("previous-channel");
  MutableImageProperties props;
  props.target_channel = "new-channel";
  EXPECT_TRUE(StoreMutableImageProperties(&fake_system_state_, props));
  MutableImageProperties loaded_props =
      LoadMutableImageProperties(&fake_system_state_);
  EXPECT_EQ(props.target_channel, loaded_props.target_channel);
}

}  // namespace chromeos_update_engine
