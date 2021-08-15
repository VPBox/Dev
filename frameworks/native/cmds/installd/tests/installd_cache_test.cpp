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

#include <stdlib.h>
#include <string.h>
#include <sys/statvfs.h>
#include <sys/xattr.h>

#include <android-base/logging.h>
#include <android-base/stringprintf.h>
#include <cutils/properties.h>
#include <gtest/gtest.h>

#include "InstalldNativeService.h"
#include "globals.h"
#include "utils.h"

using android::base::StringPrintf;

namespace android {
namespace installd {

constexpr const char* kTestUuid = "TEST";

constexpr int64_t kKbInBytes = 1024;
constexpr int64_t kMbInBytes = 1024 * kKbInBytes;
constexpr int64_t kGbInBytes = 1024 * kMbInBytes;
constexpr int64_t kTbInBytes = 1024 * kGbInBytes;

#define FLAG_FREE_CACHE_V2 InstalldNativeService::FLAG_FREE_CACHE_V2
#define FLAG_FREE_CACHE_V2_DEFY_QUOTA InstalldNativeService::FLAG_FREE_CACHE_V2_DEFY_QUOTA

int get_property(const char *key, char *value, const char *default_value) {
    return property_get(key, value, default_value);
}

bool calculate_oat_file_path(char path[PKG_PATH_MAX] ATTRIBUTE_UNUSED,
        const char *oat_dir ATTRIBUTE_UNUSED,
        const char *apk_path ATTRIBUTE_UNUSED,
        const char *instruction_set ATTRIBUTE_UNUSED) {
    return false;
}

bool calculate_odex_file_path(char path[PKG_PATH_MAX] ATTRIBUTE_UNUSED,
        const char *apk_path ATTRIBUTE_UNUSED,
        const char *instruction_set ATTRIBUTE_UNUSED) {
    return false;
}

bool create_cache_path(char path[PKG_PATH_MAX] ATTRIBUTE_UNUSED,
        const char *src ATTRIBUTE_UNUSED,
        const char *instruction_set ATTRIBUTE_UNUSED) {
    return false;
}

static void mkdir(const char* path) {
    const char* fullPath = StringPrintf("/data/local/tmp/user/0/%s", path).c_str();
    ::mkdir(fullPath, 0755);
}

static void touch(const char* path, int len, int time) {
    const char* fullPath = StringPrintf("/data/local/tmp/user/0/%s", path).c_str();
    int fd = ::open(fullPath, O_RDWR | O_CREAT, 0644);
    ::fallocate(fd, 0, 0, len);
    ::close(fd);
    struct utimbuf times;
    times.actime = times.modtime = std::time(0) + time;
    ::utime(fullPath, &times);
}

static int exists(const char* path) {
    const char* fullPath = StringPrintf("/data/local/tmp/user/0/%s", path).c_str();
    return ::access(fullPath, F_OK);
}

static int64_t size(const char* path) {
    const char* fullPath = StringPrintf("/data/local/tmp/user/0/%s", path).c_str();
    struct stat buf;
    if (!stat(fullPath, &buf)) {
        return buf.st_size;
    } else {
        return -1;
    }
}

static int64_t free() {
    struct statvfs buf;
    if (!statvfs("/data/local/tmp", &buf)) {
        return static_cast<int64_t>(buf.f_bavail) * buf.f_frsize;
    } else {
        PLOG(ERROR) << "Failed to statvfs";
        return -1;
    }
}

static void setxattr(const char* path, const char* key) {
    const char* fullPath = StringPrintf("/data/local/tmp/user/0/%s", path).c_str();
    ::setxattr(fullPath, key, "", 0, 0);
}

class CacheTest : public testing::Test {
protected:
    InstalldNativeService* service;
    std::unique_ptr<std::string> testUuid;

    virtual void SetUp() {
        setenv("ANDROID_LOG_TAGS", "*:v", 1);
        android::base::InitLogging(nullptr);

        service = new InstalldNativeService();
        testUuid = std::make_unique<std::string>();
        *testUuid = std::string(kTestUuid);
        system("mkdir -p /data/local/tmp/user/0");
    }

    virtual void TearDown() {
        delete service;
        system("rm -rf /data/local/tmp/user");
    }
};

TEST_F(CacheTest, FreeCache_All) {
    LOG(INFO) << "FreeCache_All";

    mkdir("com.example");
    touch("com.example/normal", 1 * kMbInBytes, 60);
    mkdir("com.example/cache");
    mkdir("com.example/cache/foo");
    touch("com.example/cache/foo/one", 1 * kMbInBytes, 60);
    touch("com.example/cache/foo/two", 2 * kMbInBytes, 120);

    EXPECT_EQ(0, exists("com.example/normal"));
    EXPECT_EQ(0, exists("com.example/cache/foo/one"));
    EXPECT_EQ(0, exists("com.example/cache/foo/two"));

    service->freeCache(testUuid, kTbInBytes, 0,
            FLAG_FREE_CACHE_V2 | FLAG_FREE_CACHE_V2_DEFY_QUOTA);

    EXPECT_EQ(0, exists("com.example/normal"));
    EXPECT_EQ(-1, exists("com.example/cache/foo/one"));
    EXPECT_EQ(-1, exists("com.example/cache/foo/two"));
}

TEST_F(CacheTest, FreeCache_Age) {
    LOG(INFO) << "FreeCache_Age";

    mkdir("com.example");
    mkdir("com.example/cache");
    mkdir("com.example/cache/foo");
    touch("com.example/cache/foo/one", kMbInBytes, 60);
    touch("com.example/cache/foo/two", kMbInBytes, 120);

    service->freeCache(testUuid, free() + kKbInBytes, 0,
            FLAG_FREE_CACHE_V2 | FLAG_FREE_CACHE_V2_DEFY_QUOTA);

    EXPECT_EQ(-1, exists("com.example/cache/foo/one"));
    EXPECT_EQ(0, exists("com.example/cache/foo/two"));

    service->freeCache(testUuid, free() + kKbInBytes, 0,
            FLAG_FREE_CACHE_V2 | FLAG_FREE_CACHE_V2_DEFY_QUOTA);

    EXPECT_EQ(-1, exists("com.example/cache/foo/one"));
    EXPECT_EQ(-1, exists("com.example/cache/foo/two"));
}

TEST_F(CacheTest, FreeCache_Tombstone) {
    LOG(INFO) << "FreeCache_Tombstone";

    mkdir("com.example");
    mkdir("com.example/cache");
    mkdir("com.example/cache/foo");
    touch("com.example/cache/foo/foo1", 1 * kMbInBytes, 60);
    touch("com.example/cache/foo/foo2", 1 * kMbInBytes, 60);
    mkdir("com.example/cache/bar");
    touch("com.example/cache/bar/bar1", 2 * kMbInBytes, 120);
    touch("com.example/cache/bar/bar2", 2 * kMbInBytes, 120);

    setxattr("com.example/cache/bar", "user.cache_tombstone");

    EXPECT_EQ(0, exists("com.example/cache/foo/foo1"));
    EXPECT_EQ(0, exists("com.example/cache/foo/foo2"));
    EXPECT_EQ(0, exists("com.example/cache/bar/bar1"));
    EXPECT_EQ(0, exists("com.example/cache/bar/bar2"));
    EXPECT_EQ(2 * kMbInBytes, size("com.example/cache/bar/bar1"));
    EXPECT_EQ(2 * kMbInBytes, size("com.example/cache/bar/bar2"));

    service->freeCache(testUuid, kTbInBytes, 0,
            FLAG_FREE_CACHE_V2 | FLAG_FREE_CACHE_V2_DEFY_QUOTA);

    EXPECT_EQ(-1, exists("com.example/cache/foo/foo1"));
    EXPECT_EQ(-1, exists("com.example/cache/foo/foo2"));
    EXPECT_EQ(0, exists("com.example/cache/bar/bar1"));
    EXPECT_EQ(0, exists("com.example/cache/bar/bar2"));
    EXPECT_EQ(0, size("com.example/cache/bar/bar1"));
    EXPECT_EQ(0, size("com.example/cache/bar/bar2"));
}

TEST_F(CacheTest, FreeCache_Group) {
    LOG(INFO) << "FreeCache_Group";

    mkdir("com.example");
    mkdir("com.example/cache");
    mkdir("com.example/cache/foo");
    touch("com.example/cache/foo/foo1", 1 * kMbInBytes, 60);
    touch("com.example/cache/foo/foo2", 1 * kMbInBytes, 120);

    setxattr("com.example/cache/foo", "user.cache_group");

    service->freeCache(testUuid, free() + kKbInBytes, 0,
            FLAG_FREE_CACHE_V2 | FLAG_FREE_CACHE_V2_DEFY_QUOTA);

    EXPECT_EQ(-1, exists("com.example/cache/foo/foo1"));
    EXPECT_EQ(-1, exists("com.example/cache/foo/foo2"));
}

TEST_F(CacheTest, FreeCache_GroupTombstone) {
    LOG(INFO) << "FreeCache_GroupTombstone";

    mkdir("com.example");
    mkdir("com.example/cache");

    // this dir must look really old for some reason?
    mkdir("com.example/cache/group");
    touch("com.example/cache/group/file1", kMbInBytes, 120);
    touch("com.example/cache/group/file2", kMbInBytes, 120);
    mkdir("com.example/cache/group/dir");
    touch("com.example/cache/group/dir/file1", kMbInBytes, 120);
    touch("com.example/cache/group/dir/file2", kMbInBytes, 120);
    mkdir("com.example/cache/group/tomb");
    touch("com.example/cache/group/tomb/file1", kMbInBytes, 120);
    touch("com.example/cache/group/tomb/file2", kMbInBytes, 120);
    mkdir("com.example/cache/group/tomb/dir");
    touch("com.example/cache/group/tomb/dir/file1", kMbInBytes, 120);
    touch("com.example/cache/group/tomb/dir/file2", kMbInBytes, 120);

    mkdir("com.example/cache/tomb");
    touch("com.example/cache/tomb/file1", kMbInBytes, 240);
    touch("com.example/cache/tomb/file2", kMbInBytes, 240);
    mkdir("com.example/cache/tomb/dir");
    touch("com.example/cache/tomb/dir/file1", kMbInBytes, 240);
    touch("com.example/cache/tomb/dir/file2", kMbInBytes, 240);
    mkdir("com.example/cache/tomb/group");
    touch("com.example/cache/tomb/group/file1", kMbInBytes, 60);
    touch("com.example/cache/tomb/group/file2", kMbInBytes, 60);
    mkdir("com.example/cache/tomb/group/dir");
    touch("com.example/cache/tomb/group/dir/file1", kMbInBytes, 60);
    touch("com.example/cache/tomb/group/dir/file2", kMbInBytes, 60);

    setxattr("com.example/cache/group", "user.cache_group");
    setxattr("com.example/cache/group/tomb", "user.cache_tombstone");
    setxattr("com.example/cache/tomb", "user.cache_tombstone");
    setxattr("com.example/cache/tomb/group", "user.cache_group");

    service->freeCache(testUuid, free() + kKbInBytes, 0,
            FLAG_FREE_CACHE_V2 | FLAG_FREE_CACHE_V2_DEFY_QUOTA);

    EXPECT_EQ(kMbInBytes, size("com.example/cache/group/file1"));
    EXPECT_EQ(kMbInBytes, size("com.example/cache/group/file2"));
    EXPECT_EQ(kMbInBytes, size("com.example/cache/group/dir/file1"));
    EXPECT_EQ(kMbInBytes, size("com.example/cache/group/dir/file2"));
    EXPECT_EQ(kMbInBytes, size("com.example/cache/group/tomb/file1"));
    EXPECT_EQ(kMbInBytes, size("com.example/cache/group/tomb/file2"));
    EXPECT_EQ(kMbInBytes, size("com.example/cache/group/tomb/dir/file1"));
    EXPECT_EQ(kMbInBytes, size("com.example/cache/group/tomb/dir/file2"));

    EXPECT_EQ(kMbInBytes, size("com.example/cache/tomb/file1"));
    EXPECT_EQ(kMbInBytes, size("com.example/cache/tomb/file2"));
    EXPECT_EQ(kMbInBytes, size("com.example/cache/tomb/dir/file1"));
    EXPECT_EQ(kMbInBytes, size("com.example/cache/tomb/dir/file2"));
    EXPECT_EQ(0, size("com.example/cache/tomb/group/file1"));
    EXPECT_EQ(0, size("com.example/cache/tomb/group/file2"));
    EXPECT_EQ(0, size("com.example/cache/tomb/group/dir/file1"));
    EXPECT_EQ(0, size("com.example/cache/tomb/group/dir/file2"));

    service->freeCache(testUuid, free() + kKbInBytes, 0,
            FLAG_FREE_CACHE_V2 | FLAG_FREE_CACHE_V2_DEFY_QUOTA);

    EXPECT_EQ(-1, size("com.example/cache/group/file1"));
    EXPECT_EQ(-1, size("com.example/cache/group/file2"));
    EXPECT_EQ(-1, size("com.example/cache/group/dir/file1"));
    EXPECT_EQ(-1, size("com.example/cache/group/dir/file2"));
    EXPECT_EQ(0, size("com.example/cache/group/tomb/file1"));
    EXPECT_EQ(0, size("com.example/cache/group/tomb/file2"));
    EXPECT_EQ(0, size("com.example/cache/group/tomb/dir/file1"));
    EXPECT_EQ(0, size("com.example/cache/group/tomb/dir/file2"));

    EXPECT_EQ(kMbInBytes, size("com.example/cache/tomb/file1"));
    EXPECT_EQ(kMbInBytes, size("com.example/cache/tomb/file2"));
    EXPECT_EQ(kMbInBytes, size("com.example/cache/tomb/dir/file1"));
    EXPECT_EQ(kMbInBytes, size("com.example/cache/tomb/dir/file2"));
    EXPECT_EQ(0, size("com.example/cache/tomb/group/file1"));
    EXPECT_EQ(0, size("com.example/cache/tomb/group/file2"));
    EXPECT_EQ(0, size("com.example/cache/tomb/group/dir/file1"));
    EXPECT_EQ(0, size("com.example/cache/tomb/group/dir/file2"));

    service->freeCache(testUuid, kTbInBytes, 0,
            FLAG_FREE_CACHE_V2 | FLAG_FREE_CACHE_V2_DEFY_QUOTA);

    EXPECT_EQ(-1, size("com.example/cache/group/file1"));
    EXPECT_EQ(-1, size("com.example/cache/group/file2"));
    EXPECT_EQ(-1, size("com.example/cache/group/dir/file1"));
    EXPECT_EQ(-1, size("com.example/cache/group/dir/file2"));
    EXPECT_EQ(0, size("com.example/cache/group/tomb/file1"));
    EXPECT_EQ(0, size("com.example/cache/group/tomb/file2"));
    EXPECT_EQ(0, size("com.example/cache/group/tomb/dir/file1"));
    EXPECT_EQ(0, size("com.example/cache/group/tomb/dir/file2"));

    EXPECT_EQ(0, size("com.example/cache/tomb/file1"));
    EXPECT_EQ(0, size("com.example/cache/tomb/file2"));
    EXPECT_EQ(0, size("com.example/cache/tomb/dir/file1"));
    EXPECT_EQ(0, size("com.example/cache/tomb/dir/file2"));
    EXPECT_EQ(0, size("com.example/cache/tomb/group/file1"));
    EXPECT_EQ(0, size("com.example/cache/tomb/group/file2"));
    EXPECT_EQ(0, size("com.example/cache/tomb/group/dir/file1"));
    EXPECT_EQ(0, size("com.example/cache/tomb/group/dir/file2"));
}

}  // namespace installd
}  // namespace android
