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

#include <gtest/gtest.h>

#include <ConstantExpression.h>
#include <Coordinator.h>
#include <hidl-util/FQName.h>

#define EXPECT_EQ_OK(expectResult, call, ...)        \
    do {                                             \
        std::string result;                          \
        status_t err = (call)(__VA_ARGS__, &result); \
        EXPECT_EQ(err, ::android::OK);               \
        EXPECT_EQ(expectResult, result);             \
    } while (false)

namespace android {

class HidlGenHostTest : public ::testing::Test {};

TEST_F(HidlGenHostTest, CoordinatorTest) {
    Coordinator coordinator;

    std::string error;
    EXPECT_EQ(OK, coordinator.addPackagePath("a.b", "a1/b1", &error));
    EXPECT_TRUE(error.empty());
    EXPECT_NE(OK, coordinator.addPackagePath("a.b", "a2/b2/", &error));
    EXPECT_FALSE(error.empty());

    coordinator.addDefaultPackagePath("a.b", "a3/b3/"); // should take path above
    coordinator.addDefaultPackagePath("a.c", "a4/b4/"); // should succeed

    EXPECT_EQ_OK("a.b", coordinator.getPackageRoot, FQName("a.b.foo", "1.0"));
    EXPECT_EQ_OK("a.c", coordinator.getPackageRoot, FQName("a.c.foo.bar", "1.0", "IFoo"));

    // getPackagePath(fqname, relative, sanitized, ...)
    EXPECT_EQ_OK("a1/b1/foo/1.0/", coordinator.getPackagePath, FQName("a.b.foo", "1.0"), false,
                 false);
    EXPECT_EQ_OK("a4/b4/foo/bar/1.0/", coordinator.getPackagePath,
                 FQName("a.c.foo.bar", "1.0", "IFoo"), false, false);
    EXPECT_EQ_OK("a1/b1/foo/V1_0/", coordinator.getPackagePath, FQName("a.b.foo", "1.0"), false,
                 true);
    EXPECT_EQ_OK("a4/b4/foo/bar/V1_0/", coordinator.getPackagePath,
                 FQName("a.c.foo.bar", "1.0", "IFoo"), false, true);
    EXPECT_EQ_OK("foo/1.0/", coordinator.getPackagePath, FQName("a.b.foo", "1.0"), true, false);
    EXPECT_EQ_OK("foo/bar/1.0/", coordinator.getPackagePath, FQName("a.c.foo.bar", "1.0", "IFoo"),
                 true, false);
    EXPECT_EQ_OK("foo/V1_0/", coordinator.getPackagePath, FQName("a.b.foo", "1.0"), true, true);
    EXPECT_EQ_OK("foo/bar/V1_0/", coordinator.getPackagePath, FQName("a.c.foo.bar", "1.0", "IFoo"),
                 true, true);
}

TEST_F(HidlGenHostTest, CoordinatorFilepathTest) {
    using Location = Coordinator::Location;

    Coordinator coordinator;
    coordinator.setOutputPath("foo/");
    coordinator.setRootPath("bar/");

    std::string error;
    EXPECT_EQ(OK, coordinator.addPackagePath("a.b", "a1/b1", &error));
    EXPECT_TRUE(error.empty());

    const static FQName kName = FQName("a.b.c", "1.2");

    // get file names
    EXPECT_EQ_OK("foo/x.y", coordinator.getFilepath, kName, Location::DIRECT, "x.y");
    EXPECT_EQ_OK("foo/a1/b1/c/1.2/x.y", coordinator.getFilepath, kName, Location::PACKAGE_ROOT,
                 "x.y");
    EXPECT_EQ_OK("foo/a/b/c/1.2/x.y", coordinator.getFilepath, kName, Location::GEN_OUTPUT, "x.y");
    EXPECT_EQ_OK("foo/a/b/c/V1_2/x.y", coordinator.getFilepath, kName, Location::GEN_SANITIZED,
                 "x.y");

    // get directories
    EXPECT_EQ_OK("foo/", coordinator.getFilepath, kName, Location::DIRECT, "");
    EXPECT_EQ_OK("foo/a1/b1/c/1.2/", coordinator.getFilepath, kName, Location::PACKAGE_ROOT, "");
    EXPECT_EQ_OK("foo/a/b/c/1.2/", coordinator.getFilepath, kName, Location::GEN_OUTPUT, "");
    EXPECT_EQ_OK("foo/a/b/c/V1_2/", coordinator.getFilepath, kName, Location::GEN_SANITIZED, "");
}

TEST_F(HidlGenHostTest, LocationTest) {
    Location a{{"file", 3, 4}, {"file", 3, 5}};
    Location b{{"file", 3, 6}, {"file", 3, 7}};
    Location c{{"file", 4, 4}, {"file", 4, 5}};

    Location other{{"other", 0, 0}, {"other", 0, 1}};

    EXPECT_LT(a, b);
    EXPECT_LT(b, c);
    EXPECT_LT(a, c);
    EXPECT_FALSE(Location::inSameFile(a, other));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

}  // namespace android