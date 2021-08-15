/*
 * Copyright (C) 2016 The Android Open Source Project
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

#define LOG_TAG "libhidl-gen-utils"

#include <hidl-util/FqInstance.h>

#include <gtest/gtest.h>
#include <vector>

using ::android::FqInstance;
using ::android::FQName;

class LibHidlGenUtilsTest : public ::testing::Test {};

TEST_F(LibHidlGenUtilsTest, FqInstance1) {
    FqInstance e;
    ASSERT_TRUE(e.setTo("android.hardware.foo@1.0::IFoo/instance"));
    EXPECT_EQ("android.hardware.foo@1.0::IFoo/instance", e.string());
    ASSERT_TRUE(e.hasPackage());
    EXPECT_EQ("android.hardware.foo", e.getPackage());
    ASSERT_TRUE(e.hasVersion());
    EXPECT_EQ(1u, e.getMajorVersion());
    EXPECT_EQ(0u, e.getMinorVersion());
    EXPECT_EQ((std::make_pair<size_t, size_t>(1u, 0u)), e.getVersion());
    ASSERT_TRUE(e.hasInterface());
    EXPECT_EQ("IFoo", e.getInterface());
    ASSERT_TRUE(e.hasInstance());
    EXPECT_EQ("instance", e.getInstance());
}

TEST_F(LibHidlGenUtilsTest, FqInstance2) {
    FqInstance e;
    ASSERT_TRUE(e.setTo("@1.0::IFoo/instance"));
    EXPECT_EQ("@1.0::IFoo/instance", e.string());
    ASSERT_FALSE(e.hasPackage());
    ASSERT_TRUE(e.hasVersion());
    EXPECT_EQ((std::make_pair<size_t, size_t>(1u, 0u)), e.getVersion());
    ASSERT_TRUE(e.hasInterface());
    EXPECT_EQ("IFoo", e.getInterface());
    ASSERT_TRUE(e.hasInstance());
    EXPECT_EQ("instance", e.getInstance());
}

TEST_F(LibHidlGenUtilsTest, FqInstance3) {
    FqInstance e;
    ASSERT_TRUE(e.setTo("IFoo/instance"));
    EXPECT_EQ("IFoo/instance", e.string());
    ASSERT_FALSE(e.hasPackage());
    ASSERT_FALSE(e.hasVersion());
    ASSERT_TRUE(e.hasInterface());
    EXPECT_EQ("IFoo", e.getInterface());
    ASSERT_TRUE(e.hasInstance());
    EXPECT_EQ("instance", e.getInstance());
}

TEST_F(LibHidlGenUtilsTest, FqInstanceFqNameOnly) {
    FqInstance e;
    for (auto testString :
         {"android.hardware.foo@1.0::IFoo.Type", "@1.0::IFoo.Type", "android.hardware.foo@1.0",
          "IFoo.Type", "Type", "android.hardware.foo@1.0::IFoo.Type:MY_ENUM_VALUE",
          "@1.0::IFoo.Type:MY_ENUM_VALUE", "IFoo.Type:MY_ENUM_VALUE"}) {
        ASSERT_TRUE(e.setTo(testString));
        EXPECT_EQ(testString, e.string());
        ASSERT_FALSE(e.hasInstance());
    };
}

TEST_F(LibHidlGenUtilsTest, FqInstanceIdentifier) {
    FqInstance e;
    ASSERT_TRUE(e.setTo("Type"));
    EXPECT_EQ("Type", e.string());
    ASSERT_FALSE(e.hasInstance());
}

TEST_F(LibHidlGenUtilsTest, FqInstanceSetToByComponent) {
    FqInstance e;
    ASSERT_TRUE(e.setTo("android.hardware.foo", 1, 0, "IFoo", "default"));
    EXPECT_EQ("android.hardware.foo@1.0::IFoo/default", e.string());
    ASSERT_TRUE(e.setTo("android.hardware.foo", 1, 0, "IFoo"));
    EXPECT_EQ("android.hardware.foo@1.0::IFoo", e.string());
    ASSERT_TRUE(e.setTo("android.hardware.foo", 1, 0));
    EXPECT_EQ("android.hardware.foo@1.0", e.string());
    ASSERT_TRUE(e.setTo(1, 0, "IFoo", "default"));
    EXPECT_EQ("@1.0::IFoo/default", e.string());
    ASSERT_TRUE(e.setTo(1, 0, "IFoo"));
    EXPECT_EQ("@1.0::IFoo", e.string());
    ASSERT_TRUE(e.setTo("IFoo", "default"));
    EXPECT_EQ("IFoo/default", e.string());
}

TEST_F(LibHidlGenUtilsTest, FqDefaultVersion) {
    FQName n;
    FqInstance i;

    ASSERT_TRUE(FQName::parse("IFoo.test", &n));
    EXPECT_EQ((std::make_pair<size_t, size_t>(0u, 0u)), n.getVersion());
    ASSERT_TRUE(i.setTo("IFoo.test"));
    EXPECT_EQ((std::make_pair<size_t, size_t>(0u, 0u)), i.getVersion());
    ASSERT_TRUE(FQName::parse("package@1.2::IFoo", &n));
    EXPECT_EQ((std::make_pair<size_t, size_t>(1u, 2u)), n.getVersion());
    ASSERT_TRUE(i.setTo("package@1.2::IFoo"));
    EXPECT_EQ((std::make_pair<size_t, size_t>(1u, 2u)), i.getVersion());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
