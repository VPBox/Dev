/*
 * Copyright (C) 2019 The Android Open Source Project
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

#include <android-base/properties.h>
#include <gtest/gtest.h>
#include <chrono>

#include "linkerconfig/variables.h"

using android::linkerconfig::modules::Variables;

TEST(linkerconfig_variables, load_from_map) {
  Variables::AddValue("TEST_KEY", "TEST_VALUE");
  auto value = Variables::GetValue("TEST_KEY");
  ASSERT_TRUE(value.has_value());
  ASSERT_EQ(value.value(), "TEST_VALUE");
}

TEST(linkerconfig_variables, load_from_property) {
#if defined(__BIONIC__)
  android::base::SetProperty("debug.linkerconfig.test_prop_key",
                             "TEST_PROP_VALUE");
  ASSERT_TRUE(android::base::WaitForProperty("debug.linkerconfig.test_prop_key",
                                             "TEST_PROP_VALUE",
                                             std::chrono::seconds(1)));
  auto value = Variables::GetValue("debug.linkerconfig.test_prop_key");
  ASSERT_TRUE(value.has_value());
  ASSERT_EQ(value.value(), "TEST_PROP_VALUE");
#endif
}

TEST(linkerconfig_variables, fallback_value) {
  auto value = Variables::GetValue("INVALID_KEY");
  ASSERT_FALSE(value.has_value());
}