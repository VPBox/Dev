/*
 * Copyright (C) 2018 The Android Open Source Project
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
 * limitations under the License
 */

#include "server_configurable_flags/disaster_recovery.h"
#include "server_configurable_flags/get_flags.h"

#include <gtest/gtest.h>

#include <algorithm>
#include <string>
#include <vector>

#include "android-base/file.h"
#include "android-base/properties.h"
#include "android-base/strings.h"

using namespace server_configurable_flags;
using namespace android::base;

static bool contains(std::vector<std::string>& vec, std::string& str) {
  return std::find(vec.begin(), vec.end(), str) != vec.end();
}

TEST(server_configurable_flags, empty_flag_returns_default) {
  std::string result =
      server_configurable_flags::GetServerConfigurableFlag("category", "flag", "default");
  ASSERT_EQ("default", result);
}

TEST(server_configurable_flags, set_flag_returns_value) {
  android::base::SetProperty("persist.device_config.category.flag", "hello");
  std::string result =
      server_configurable_flags::GetServerConfigurableFlag("category", "flag", "default");
  ASSERT_EQ("hello", result);

  // clean up
  android::base::SetProperty("persist.device_config.category.flag", "");
}

TEST(server_configurable_flags, invalid_flag_returns_default) {
  std::string result =
      server_configurable_flags::GetServerConfigurableFlag("category.", "flag", "default");
  ASSERT_EQ("default", result);

  result = server_configurable_flags::GetServerConfigurableFlag("category", "!flag", "default");
  ASSERT_EQ("default", result);

  result = server_configurable_flags::GetServerConfigurableFlag("category", ".flag", "default");
  ASSERT_EQ("default", result);
}

TEST(server_configurable_flags, flags_reset_skip_under_threshold) {
#if defined(__BIONIC__)
  android::base::SetProperty("persist.device_config.attempted_boot_count", "1");
  android::base::SetProperty("persist.device_config.category1.prop1", "val1");
  android::base::SetProperty("persist.device_config.category1.prop2", "val2");
  android::base::SetProperty("persist.device_config.category2.prop3", "val3");
  android::base::SetProperty("sys.category3.test", "val4");
  android::base::SetProperty("device_config.reset_performed", "");

  server_configurable_flags::ServerConfigurableFlagsReset(server_configurable_flags::BOOT_FAILURE);

  ASSERT_EQ("2", android::base::GetProperty("persist.device_config.attempted_boot_count", ""));
  ASSERT_EQ("val1", android::base::GetProperty("persist.device_config.category1.prop1", ""));
  ASSERT_EQ("val2", android::base::GetProperty("persist.device_config.category1.prop2", ""));
  ASSERT_EQ("val3", android::base::GetProperty("persist.device_config.category2.prop3", ""));
  ASSERT_EQ("val4", android::base::GetProperty("sys.category3.test", ""));
  ASSERT_EQ("", android::base::GetProperty("device_config.reset_performed", ""));
#else   // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif  // __BIONIC__
}

TEST(server_configurable_flags, flags_reset_performed_over_threshold) {
#if defined(__BIONIC__)
  android::base::SetProperty("persist.device_config.attempted_boot_count", "5");
  android::base::SetProperty("persist.device_config.category1.prop1", "val1");
  android::base::SetProperty("persist.device_config.category1.prop2", "val2");
  android::base::SetProperty("persist.device_config.category2.prop3", "val3");
  android::base::SetProperty("sys.category3.test", "val4");

  server_configurable_flags::ServerConfigurableFlagsReset(server_configurable_flags::BOOT_FAILURE);

  ASSERT_EQ("true", android::base::GetProperty("device_config.reset_performed", ""));
  ASSERT_EQ("5", android::base::GetProperty("persist.device_config.attempted_boot_count", ""));
  ASSERT_EQ("", android::base::GetProperty("persist.device_config.category1.prop1", ""));
  ASSERT_EQ("", android::base::GetProperty("persist.device_config.category1.prop2", ""));
  ASSERT_EQ("", android::base::GetProperty("persist.device_config.category2.prop3", ""));
  ASSERT_EQ("val4", android::base::GetProperty("sys.category3.test", ""));

  std::string content;
  ASSERT_EQ(true, ReadFileToString("/data/server_configurable_flags/reset_flags", &content));
  std::vector<std::string> properties = Split(content, ";");
  ASSERT_EQ((unsigned long)3, properties.size());
  std::string prop1("persist.device_config.category1.prop1"),
      prop2("persist.device_config.category1.prop2"),
      prop3("persist.device_config.category2.prop3");
  ASSERT_EQ(true, contains(properties, prop1));
  ASSERT_EQ(true, contains(properties, prop2));
  ASSERT_EQ(true, contains(properties, prop3));
#else   // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif  // __BIONIC__
}

TEST(server_configurable_flags, flags_reset_performed_on_updatable_crashing) {
#if defined(__BIONIC__)
  android::base::SetProperty("persist.device_config.attempted_boot_count", "1");
  android::base::SetProperty("persist.device_config.category1.prop1", "val1");
  android::base::SetProperty("persist.device_config.category1.prop2", "val2");
  android::base::SetProperty("persist.device_config.category2.prop3", "val3");
  android::base::SetProperty("sys.category3.test", "val4");

  server_configurable_flags::ServerConfigurableFlagsReset(
      server_configurable_flags::UPDATABLE_CRASHING);

  ASSERT_EQ("true", android::base::GetProperty("device_config.reset_performed", ""));
  ASSERT_EQ("1", android::base::GetProperty("persist.device_config.attempted_boot_count", ""));
  ASSERT_EQ("", android::base::GetProperty("persist.device_config.category1.prop1", ""));
  ASSERT_EQ("", android::base::GetProperty("persist.device_config.category1.prop2", ""));
  ASSERT_EQ("", android::base::GetProperty("persist.device_config.category2.prop3", ""));
  ASSERT_EQ("val4", android::base::GetProperty("sys.category3.test", ""));

  std::string content;
  ASSERT_EQ(true, ReadFileToString("/data/server_configurable_flags/reset_flags", &content));
  std::vector<std::string> properties = Split(content, ";");
  ASSERT_EQ((unsigned long)3, properties.size());
  std::string prop1("persist.device_config.category1.prop1"),
      prop2("persist.device_config.category1.prop2"),
      prop3("persist.device_config.category2.prop3");
  ASSERT_EQ(true, contains(properties, prop1));
  ASSERT_EQ(true, contains(properties, prop2));
  ASSERT_EQ(true, contains(properties, prop3));
#else   // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif  // __BIONIC__
}
