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
 * limitations under the License.
 */

#include <unistd.h>
#include <string>

#include <android-base/test_utils.h>
#include <gtest/gtest.h>

#include "Common.h"

using namespace std::string_literals;

TEST(SyspropTest, CreateNestedDirectoryTest) {
  TemporaryDir temp_dir;

  ASSERT_FALSE(IsDirectory(temp_dir.path + "/test_dir/test_dir2/test_dir3"s));
  ASSERT_TRUE(
      CreateDirectories(temp_dir.path + "/test_dir/test_dir2/test_dir3"s));
  ASSERT_TRUE(IsDirectory(temp_dir.path + "/test_dir/test_dir2/test_dir3"s));

  rmdir((temp_dir.path + "/test_dir/test_dir2/test_dir3"s).c_str());
  rmdir((temp_dir.path + "/test_dir/test_dir2"s).c_str());
  rmdir((temp_dir.path + "/test_dir"s).c_str());
}
