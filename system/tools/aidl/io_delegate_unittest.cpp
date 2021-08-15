/*
 * Copyright (C) 2016, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <string>

#include <gtest/gtest.h>

#include "io_delegate.h"

using std::string;

namespace android {
namespace aidl {

TEST(IoDelegateTest, CannotGetAbsolutePathFromEmptyString) {
  string absolute_path;
  EXPECT_FALSE(IoDelegate::GetAbsolutePath("", &absolute_path));
  EXPECT_TRUE(absolute_path.empty());
}

TEST(IoDelegateTest, CurrentlyInfersLinuxAbsolutePath) {
  string absolute_path;
  EXPECT_TRUE(IoDelegate::GetAbsolutePath("foo", &absolute_path));
  ASSERT_FALSE(absolute_path.empty());
  // Should find our desired file at the end of |absolute_path|
  // But we don't know the prefix, since it's the current working directory
  EXPECT_TRUE(absolute_path.rfind("/foo") == absolute_path.length() - 4);
  // Whatever our current working directory, the path is absolute.
  EXPECT_EQ(absolute_path[0], '/');
}

}  // namespace android
}  // namespace aidl
