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

#include "../cryptfs.h"

namespace android {

class CryptfsTest : public testing::Test {
  protected:
    virtual void SetUp() {}

    virtual void TearDown() {}
};

TEST_F(CryptfsTest, MatchMultiEntryTest) {
    ASSERT_NE(0, match_multi_entry("foo", "foo", 0));
    ASSERT_NE(0, match_multi_entry("foo_0", "foo", 0));
    ASSERT_NE(0, match_multi_entry("foo_1", "foo", 0));
    ASSERT_NE(0, match_multi_entry("foo_2", "foo", 0));

    ASSERT_EQ(0, match_multi_entry("foo", "foo", 1));
    ASSERT_EQ(0, match_multi_entry("foo_0", "foo", 1));
    ASSERT_NE(0, match_multi_entry("foo_1", "foo", 1));
    ASSERT_NE(0, match_multi_entry("foo_2", "foo", 1));

    ASSERT_EQ(0, match_multi_entry("foo", "foo", 2));
    ASSERT_EQ(0, match_multi_entry("foo_0", "foo", 2));
    ASSERT_EQ(0, match_multi_entry("foo_1", "foo", 2));
    ASSERT_NE(0, match_multi_entry("foo_2", "foo", 2));

    ASSERT_EQ(0, match_multi_entry("food", "foo", 0));
    ASSERT_EQ(0, match_multi_entry("foo", "food", 0));
    ASSERT_EQ(0, match_multi_entry("foo", "bar", 0));
    ASSERT_EQ(0, match_multi_entry("foo_2", "bar", 0));
}

}  // namespace android
