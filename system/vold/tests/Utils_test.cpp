/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include <gtest/gtest.h>

#include "../Utils.h"

namespace android {
namespace vold {

class UtilsTest : public testing::Test {};

TEST_F(UtilsTest, FindValueTest) {
    std::string tmp;

    ASSERT_FALSE(FindValue("", "KEY", &tmp));
    ASSERT_FALSE(FindValue("NOTMATCH=\"VALUE\"", "KEY", &tmp));
    ASSERT_FALSE(FindValue("BADKEY=\"VALUE\"", "KEY", &tmp));

    ASSERT_TRUE(FindValue("KEY=\"VALUE\"", "KEY", &tmp));
    ASSERT_EQ("VALUE", tmp);

    ASSERT_TRUE(FindValue("FOO=\"BAR\" KEY=\"VALUE VALUE\" BAR=\"BAZ\"", "KEY", &tmp));
    ASSERT_EQ("VALUE VALUE", tmp);

    ASSERT_TRUE(FindValue("BADKEY=\"VALUE\" KEY=\"BAZ\"", "KEY", &tmp));
    ASSERT_EQ("BAZ", tmp);

    ASSERT_TRUE(FindValue("BADKEY=\"VALUE\" NOTKEY=\"OTHER\" KEY=\"QUUX\"", "KEY", &tmp));
    ASSERT_EQ("QUUX", tmp);
}

}  // namespace vold
}  // namespace android
