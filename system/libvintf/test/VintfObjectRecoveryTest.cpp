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

#include <dirent.h>

#include <memory>

#include <android-base/logging.h>
#include <gtest/gtest.h>
#include <vintf/VintfObjectRecovery.h>

namespace android {
namespace vintf {

bool directoryEmpty(const char* path) {
    auto dir = std::unique_ptr<DIR, decltype(&closedir)>(opendir(path), &closedir);
    if (!dir) return true;
    for (struct dirent* dirent; (dirent = readdir(dir.get())) != nullptr;) {
        if (strcmp(dirent->d_name, ".") == 0) continue;
        if (strcmp(dirent->d_name, "..") == 0) continue;
        LOG(ERROR) << "Seen: " << path << "/" << dirent->d_name;
        return false;
    }
    return true;
}

struct VintfObjectRecoveryTest : public ::testing::Test {};

TEST_F(VintfObjectRecoveryTest, LoadAllVintf) {
    std::string error;
    ASSERT_EQ(COMPATIBLE, VintfObjectRecovery::CheckCompatibility({}, &error)) << error;

    EXPECT_TRUE(directoryEmpty("/mnt/system"));
    EXPECT_TRUE(directoryEmpty("/mnt/vendor"));
    EXPECT_TRUE(directoryEmpty("/mnt/odm"));

    ASSERT_EQ(COMPATIBLE, VintfObjectRecovery::CheckCompatibility({}, &error))
        << "Second CheckCompatibility call should still be successful because all "
        << "partitions should already be unmounted, but error: " << error;

    EXPECT_TRUE(directoryEmpty("/mnt/system"));
    EXPECT_TRUE(directoryEmpty("/mnt/vendor"));
    EXPECT_TRUE(directoryEmpty("/mnt/odm"));
}

}  // namespace vintf
}  // namespace android

int main(int argc, char** argv) {
    // There is no logcat in recovery.
    android::base::SetLogger(android::base::StderrLogger);

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
