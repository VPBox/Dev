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

#include <sstream>
#include <string>

#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/statvfs.h>
#include <sys/stat.h>
#include <sys/xattr.h>

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/properties.h>
#include <android-base/scopeguard.h>
#include <android-base/stringprintf.h>
#include <cutils/properties.h>
#include <gtest/gtest.h>

#include "binder_test_utils.h"
#include "InstalldNativeService.h"
#include "dexopt.h"
#include "globals.h"
#include "utils.h"

using android::base::StringPrintf;

namespace android {
namespace installd {

constexpr const char* kTestUuid = "TEST";

#define FLAG_FORCE InstalldNativeService::FLAG_FORCE

int get_property(const char *key, char *value, const char *default_value) {
    return property_get(key, value, default_value);
}

bool calculate_oat_file_path(char path[PKG_PATH_MAX], const char *oat_dir, const char *apk_path,
        const char *instruction_set) {
    return calculate_oat_file_path_default(path, oat_dir, apk_path, instruction_set);
}

bool calculate_odex_file_path(char path[PKG_PATH_MAX], const char *apk_path,
        const char *instruction_set) {
    return calculate_odex_file_path_default(path, apk_path, instruction_set);
}

bool create_cache_path(char path[PKG_PATH_MAX], const char *src, const char *instruction_set) {
    return create_cache_path_default(path, src, instruction_set);
}

static std::string get_full_path(const char* path) {
    return StringPrintf("/data/local/tmp/user/0/%s", path);
}

static void mkdir(const char* path, uid_t owner, gid_t group, mode_t mode) {
    const std::string fullPath = get_full_path(path);
    EXPECT_EQ(::mkdir(fullPath.c_str(), mode), 0);
    EXPECT_EQ(::chown(fullPath.c_str(), owner, group), 0);
    EXPECT_EQ(::chmod(fullPath.c_str(), mode), 0);
}

static void touch(const char* path, uid_t owner, gid_t group, mode_t mode) {
    int fd = ::open(get_full_path(path).c_str(), O_RDWR | O_CREAT, mode);
    EXPECT_NE(fd, -1);
    EXPECT_EQ(::fchown(fd, owner, group), 0);
    EXPECT_EQ(::fchmod(fd, mode), 0);
    EXPECT_EQ(::close(fd), 0);
}

static int stat_gid(const char* path) {
    struct stat buf;
    EXPECT_EQ(::stat(get_full_path(path).c_str(), &buf), 0);
    return buf.st_gid;
}

static int stat_mode(const char* path) {
    struct stat buf;
    EXPECT_EQ(::stat(get_full_path(path).c_str(), &buf), 0);
    return buf.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO | S_ISGID);
}

class ServiceTest : public testing::Test {
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

        init_globals_from_data_and_root();
    }

    virtual void TearDown() {
        delete service;
        system("rm -rf /data/local/tmp/user");
    }
};

TEST_F(ServiceTest, FixupAppData_Upgrade) {
    LOG(INFO) << "FixupAppData_Upgrade";

    mkdir("com.example", 10000, 10000, 0700);
    mkdir("com.example/normal", 10000, 10000, 0700);
    mkdir("com.example/cache", 10000, 10000, 0700);
    touch("com.example/cache/file", 10000, 10000, 0700);

    service->fixupAppData(testUuid, 0);

    EXPECT_EQ(10000, stat_gid("com.example/normal"));
    EXPECT_EQ(20000, stat_gid("com.example/cache"));
    EXPECT_EQ(20000, stat_gid("com.example/cache/file"));

    EXPECT_EQ(0700, stat_mode("com.example/normal"));
    EXPECT_EQ(02771, stat_mode("com.example/cache"));
    EXPECT_EQ(0700, stat_mode("com.example/cache/file"));
}

TEST_F(ServiceTest, FixupAppData_Moved) {
    LOG(INFO) << "FixupAppData_Moved";

    mkdir("com.example", 10000, 10000, 0700);
    mkdir("com.example/foo", 10000, 10000, 0700);
    touch("com.example/foo/file", 10000, 20000, 0700);
    mkdir("com.example/bar", 10000, 20000, 0700);
    touch("com.example/bar/file", 10000, 20000, 0700);

    service->fixupAppData(testUuid, 0);

    EXPECT_EQ(10000, stat_gid("com.example/foo"));
    EXPECT_EQ(20000, stat_gid("com.example/foo/file"));
    EXPECT_EQ(10000, stat_gid("com.example/bar"));
    EXPECT_EQ(10000, stat_gid("com.example/bar/file"));

    service->fixupAppData(testUuid, FLAG_FORCE);

    EXPECT_EQ(10000, stat_gid("com.example/foo"));
    EXPECT_EQ(10000, stat_gid("com.example/foo/file"));
    EXPECT_EQ(10000, stat_gid("com.example/bar"));
    EXPECT_EQ(10000, stat_gid("com.example/bar/file"));
}

TEST_F(ServiceTest, HashSecondaryDex) {
    LOG(INFO) << "HashSecondaryDex";

    mkdir("com.example", 10000, 10000, 0700);
    mkdir("com.example/foo", 10000, 10000, 0700);
    touch("com.example/foo/file", 10000, 20000, 0700);

    std::vector<uint8_t> result;
    std::string dexPath = get_full_path("com.example/foo/file");
    EXPECT_BINDER_SUCCESS(service->hashSecondaryDexFile(
        dexPath, "com.example", 10000, testUuid, FLAG_STORAGE_CE, &result));

    EXPECT_EQ(result.size(), 32U);

    std::ostringstream output;
    output << std::hex << std::setfill('0');
    for (auto b : result) {
        output << std::setw(2) << +b;
    }

    // This is the SHA256 of an empty string (sha256sum /dev/null)
    EXPECT_EQ(output.str(), "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
}

TEST_F(ServiceTest, HashSecondaryDex_NoSuch) {
    LOG(INFO) << "HashSecondaryDex_NoSuch";

    std::vector<uint8_t> result;
    std::string dexPath = get_full_path("com.example/foo/file");
    EXPECT_BINDER_SUCCESS(service->hashSecondaryDexFile(
        dexPath, "com.example", 10000, testUuid, FLAG_STORAGE_CE, &result));

    EXPECT_EQ(result.size(), 0U);
}

TEST_F(ServiceTest, HashSecondaryDex_Unreadable) {
    LOG(INFO) << "HashSecondaryDex_Unreadable";

    mkdir("com.example", 10000, 10000, 0700);
    mkdir("com.example/foo", 10000, 10000, 0700);
    touch("com.example/foo/file", 10000, 20000, 0300);

    std::vector<uint8_t> result;
    std::string dexPath = get_full_path("com.example/foo/file");
    EXPECT_BINDER_SUCCESS(service->hashSecondaryDexFile(
        dexPath, "com.example", 10000, testUuid, FLAG_STORAGE_CE, &result));

    EXPECT_EQ(result.size(), 0U);
}

TEST_F(ServiceTest, HashSecondaryDex_WrongApp) {
    LOG(INFO) << "HashSecondaryDex_WrongApp";

    mkdir("com.example", 10000, 10000, 0700);
    mkdir("com.example/foo", 10000, 10000, 0700);
    touch("com.example/foo/file", 10000, 20000, 0700);

    std::vector<uint8_t> result;
    std::string dexPath = get_full_path("com.example/foo/file");
    EXPECT_BINDER_FAIL(service->hashSecondaryDexFile(
        dexPath, "com.wrong", 10000, testUuid, FLAG_STORAGE_CE, &result));
}

TEST_F(ServiceTest, CalculateOat) {
    char buf[PKG_PATH_MAX];

    EXPECT_TRUE(calculate_oat_file_path(buf, "/path/to/oat", "/path/to/file.apk", "isa"));
    EXPECT_EQ("/path/to/oat/isa/file.odex", std::string(buf));

    EXPECT_FALSE(calculate_oat_file_path(buf, "/path/to/oat", "/path/to/file", "isa"));
    EXPECT_FALSE(calculate_oat_file_path(buf, "/path/to/oat", "file", "isa"));
}

TEST_F(ServiceTest, CalculateOdex) {
    char buf[PKG_PATH_MAX];

    EXPECT_TRUE(calculate_odex_file_path(buf, "/path/to/file.apk", "isa"));
    EXPECT_EQ("/path/to/oat/isa/file.odex", std::string(buf));
}

TEST_F(ServiceTest, CalculateCache) {
    char buf[PKG_PATH_MAX];

    EXPECT_TRUE(create_cache_path(buf, "/path/to/file.apk", "isa"));
    EXPECT_EQ("/data/dalvik-cache/isa/path@to@file.apk@classes.dex", std::string(buf));
}

static bool mkdirs(const std::string& path, mode_t mode) {
    struct stat sb;
    if (stat(path.c_str(), &sb) != -1 && S_ISDIR(sb.st_mode)) {
        return true;
    }

    if (!mkdirs(android::base::Dirname(path), mode)) {
        return false;
    }

    if (::mkdir(path.c_str(), mode) != 0) {
        PLOG(DEBUG) << "Failed to create folder " << path;
        return false;
    }
    return true;
}

class AppDataSnapshotTest : public testing::Test {
private:
    std::string rollback_ce_base_dir;
    std::string rollback_de_base_dir;

protected:
    InstalldNativeService* service;

    std::string fake_package_ce_path;
    std::string fake_package_de_path;

    virtual void SetUp() {
        setenv("ANDROID_LOG_TAGS", "*:v", 1);
        android::base::InitLogging(nullptr);

        service = new InstalldNativeService();
        ASSERT_TRUE(mkdirs("/data/local/tmp/user/0", 0700));

        init_globals_from_data_and_root();

        rollback_ce_base_dir = create_data_misc_ce_rollback_base_path("TEST", 0);
        rollback_de_base_dir = create_data_misc_de_rollback_base_path("TEST", 0);

        fake_package_ce_path = create_data_user_ce_package_path("TEST", 0, "com.foo");
        fake_package_de_path = create_data_user_de_package_path("TEST", 0, "com.foo");

        ASSERT_TRUE(mkdirs(rollback_ce_base_dir, 0700));
        ASSERT_TRUE(mkdirs(rollback_de_base_dir, 0700));
        ASSERT_TRUE(mkdirs(fake_package_ce_path, 0700));
        ASSERT_TRUE(mkdirs(fake_package_de_path, 0700));
    }

    virtual void TearDown() {
        ASSERT_EQ(0, delete_dir_contents_and_dir(rollback_ce_base_dir, true));
        ASSERT_EQ(0, delete_dir_contents_and_dir(rollback_de_base_dir, true));
        ASSERT_EQ(0, delete_dir_contents(fake_package_ce_path, true));
        ASSERT_EQ(0, delete_dir_contents(fake_package_de_path, true));

        delete service;
        ASSERT_EQ(0, delete_dir_contents_and_dir("/data/local/tmp/user/0", true));
    }
};

TEST_F(AppDataSnapshotTest, CreateAppDataSnapshot) {
  auto rollback_ce_dir = create_data_misc_ce_rollback_path("TEST", 0, 37);
  auto rollback_de_dir = create_data_misc_de_rollback_path("TEST", 0, 37);

  ASSERT_TRUE(android::base::WriteStringToFile(
          "TEST_CONTENT_CE", fake_package_ce_path + "/file1",
          0700, 10000, 20000, false /* follow_symlinks */));
  ASSERT_TRUE(android::base::WriteStringToFile(
          "TEST_CONTENT_DE", fake_package_de_path + "/file1",
          0700, 10000, 20000, false /* follow_symlinks */));

  // Request a snapshot of the CE content but not the DE content.
  int64_t ce_snapshot_inode;
  ASSERT_BINDER_SUCCESS(service->snapshotAppData(std::make_unique<std::string>("TEST"),
          "com.foo", 0, 37, FLAG_STORAGE_CE, &ce_snapshot_inode));
  struct stat buf;
  memset(&buf, 0, sizeof(buf));
  ASSERT_EQ(0, stat((rollback_ce_dir + "/com.foo").c_str(), &buf));
  ASSERT_EQ(ce_snapshot_inode, (int64_t) buf.st_ino);

  std::string ce_content, de_content;
  // At this point, we should have the CE content but not the DE content.
  ASSERT_TRUE(android::base::ReadFileToString(
      rollback_ce_dir + "/com.foo/file1", &ce_content, false /* follow_symlinks */));
  ASSERT_FALSE(android::base::ReadFileToString(
      rollback_de_dir + "/com.foo/file1", &de_content, false /* follow_symlinks */));
  ASSERT_EQ("TEST_CONTENT_CE", ce_content);

  // Modify the CE content, so we can assert later that it's reflected
  // in the snapshot.
  ASSERT_TRUE(android::base::WriteStringToFile(
          "TEST_CONTENT_CE_MODIFIED", fake_package_ce_path + "/file1",
          0700, 10000, 20000, false /* follow_symlinks */));

  // Request a snapshot of the DE content but not the CE content.
  ASSERT_BINDER_SUCCESS(service->snapshotAppData(std::make_unique<std::string>("TEST"),
          "com.foo", 0, 37, FLAG_STORAGE_DE, &ce_snapshot_inode));
  // Only DE content snapshot was requested.
  ASSERT_EQ(ce_snapshot_inode, 0);

  // At this point, both the CE as well as the DE content should be fully
  // populated.
  ASSERT_TRUE(android::base::ReadFileToString(
      rollback_ce_dir + "/com.foo/file1", &ce_content, false /* follow_symlinks */));
  ASSERT_TRUE(android::base::ReadFileToString(
      rollback_de_dir + "/com.foo/file1", &de_content, false /* follow_symlinks */));
  ASSERT_EQ("TEST_CONTENT_CE", ce_content);
  ASSERT_EQ("TEST_CONTENT_DE", de_content);

  // Modify the DE content, so we can assert later that it's reflected
  // in our final snapshot.
  ASSERT_TRUE(android::base::WriteStringToFile(
          "TEST_CONTENT_DE_MODIFIED", fake_package_de_path + "/file1",
          0700, 10000, 20000, false /* follow_symlinks */));

  // Request a snapshot of both the CE as well as the DE content.
  ASSERT_BINDER_SUCCESS(service->snapshotAppData(std::make_unique<std::string>("TEST"),
          "com.foo", 0, 37, FLAG_STORAGE_DE | FLAG_STORAGE_CE, nullptr));

  ASSERT_TRUE(android::base::ReadFileToString(
      rollback_ce_dir + "/com.foo/file1", &ce_content, false /* follow_symlinks */));
  ASSERT_TRUE(android::base::ReadFileToString(
      rollback_de_dir + "/com.foo/file1", &de_content, false /* follow_symlinks */));
  ASSERT_EQ("TEST_CONTENT_CE_MODIFIED", ce_content);
  ASSERT_EQ("TEST_CONTENT_DE_MODIFIED", de_content);
}

TEST_F(AppDataSnapshotTest, CreateAppDataSnapshot_TwoSnapshotsWithTheSameId) {
  auto rollback_ce_dir = create_data_misc_ce_rollback_path("TEST", 0, 67);
  auto rollback_de_dir = create_data_misc_de_rollback_path("TEST", 0, 67);

  auto another_fake_package_ce_path = create_data_user_ce_package_path("TEST", 0, "com.bar");
  auto another_fake_package_de_path = create_data_user_de_package_path("TEST", 0, "com.bar");

  // Since this test sets up data for another package, some bookkeeping is required.
  auto deleter = [&]() {
      ASSERT_EQ(0, delete_dir_contents_and_dir(another_fake_package_ce_path, true));
      ASSERT_EQ(0, delete_dir_contents_and_dir(another_fake_package_de_path, true));
  };
  auto scope_guard = android::base::make_scope_guard(deleter);

  ASSERT_TRUE(mkdirs(another_fake_package_ce_path, 0700));
  ASSERT_TRUE(mkdirs(another_fake_package_de_path, 0700));

  ASSERT_TRUE(android::base::WriteStringToFile(
          "TEST_CONTENT_CE", fake_package_ce_path + "/file1",
          0700, 10000, 20000, false /* follow_symlinks */));
  ASSERT_TRUE(android::base::WriteStringToFile(
          "TEST_CONTENT_DE", fake_package_de_path + "/file1",
          0700, 10000, 20000, false /* follow_symlinks */));
  ASSERT_TRUE(android::base::WriteStringToFile(
          "ANOTHER_TEST_CONTENT_CE", another_fake_package_ce_path + "/file1",
          0700, 10000, 20000, false /* follow_symlinks */));
  ASSERT_TRUE(android::base::WriteStringToFile(
          "ANOTHER_TEST_CONTENT_DE", another_fake_package_de_path + "/file1",
          0700, 10000, 20000, false /* follow_symlinks */));

  // Request snapshot for the package com.foo.
  ASSERT_BINDER_SUCCESS(service->snapshotAppData(std::make_unique<std::string>("TEST"),
          "com.foo", 0, 67, FLAG_STORAGE_DE | FLAG_STORAGE_CE, nullptr));
  // Now request snapshot with the same id for the package com.bar
  ASSERT_BINDER_SUCCESS(service->snapshotAppData(std::make_unique<std::string>("TEST"),
          "com.bar", 0, 67, FLAG_STORAGE_DE | FLAG_STORAGE_CE, nullptr));

  // Check that both snapshots have correct data in them.
  std::string com_foo_ce_content, com_foo_de_content;
  std::string com_bar_ce_content, com_bar_de_content;
  ASSERT_TRUE(android::base::ReadFileToString(
      rollback_ce_dir + "/com.foo/file1", &com_foo_ce_content, false /* follow_symlinks */));
  ASSERT_TRUE(android::base::ReadFileToString(
      rollback_de_dir + "/com.foo/file1", &com_foo_de_content, false /* follow_symlinks */));
  ASSERT_TRUE(android::base::ReadFileToString(
      rollback_ce_dir + "/com.bar/file1", &com_bar_ce_content, false /* follow_symlinks */));
  ASSERT_TRUE(android::base::ReadFileToString(
      rollback_de_dir + "/com.bar/file1", &com_bar_de_content, false /* follow_symlinks */));
  ASSERT_EQ("TEST_CONTENT_CE", com_foo_ce_content);
  ASSERT_EQ("TEST_CONTENT_DE", com_foo_de_content);
  ASSERT_EQ("ANOTHER_TEST_CONTENT_CE", com_bar_ce_content);
  ASSERT_EQ("ANOTHER_TEST_CONTENT_DE", com_bar_de_content);
}

TEST_F(AppDataSnapshotTest, CreateAppDataSnapshot_AppDataAbsent) {
  auto rollback_ce_dir = create_data_misc_ce_rollback_path("TEST", 0, 73);
  auto rollback_de_dir = create_data_misc_de_rollback_path("TEST", 0, 73);

  // Similuating app data absence.
  ASSERT_EQ(0, delete_dir_contents_and_dir(fake_package_ce_path, true));
  ASSERT_EQ(0, delete_dir_contents_and_dir(fake_package_de_path, true));

  int64_t ce_snapshot_inode;
  ASSERT_BINDER_SUCCESS(service->snapshotAppData(std::make_unique<std::string>("TEST"),
          "com.foo", 0, 73, FLAG_STORAGE_CE, &ce_snapshot_inode));
  ASSERT_BINDER_SUCCESS(service->snapshotAppData(std::make_unique<std::string>("TEST"),
          "com.foo", 0, 73, FLAG_STORAGE_DE, nullptr));
  // No CE content snapshot was performed.
  ASSERT_EQ(ce_snapshot_inode, 0);

  // The snapshot calls must succeed but there should be no snapshot
  // created.
  struct stat sb;
  ASSERT_EQ(-1, stat((rollback_ce_dir + "/com.foo").c_str(), &sb));
  ASSERT_EQ(-1, stat((rollback_de_dir + "/com.foo").c_str(), &sb));
}

TEST_F(AppDataSnapshotTest, CreateAppDataSnapshot_ClearsExistingSnapshot) {
  auto rollback_ce_dir = create_data_misc_ce_rollback_package_path("TEST", 0, 13, "com.foo");
  auto rollback_de_dir = create_data_misc_de_rollback_package_path("TEST", 0, 13, "com.foo");

  ASSERT_TRUE(mkdirs(rollback_ce_dir, 0700));
  ASSERT_TRUE(mkdirs(rollback_de_dir, 0700));

  // Simulate presence of an existing snapshot
  ASSERT_TRUE(android::base::WriteStringToFile(
          "TEST_CONTENT_CE", rollback_ce_dir + "/file1",
          0700, 10000, 20000, false /* follow_symlinks */));
  ASSERT_TRUE(android::base::WriteStringToFile(
          "TEST_CONTENT_DE", rollback_de_dir + "/file1",
          0700, 10000, 20000, false /* follow_symlinks */));

  // Create app data.
  ASSERT_TRUE(android::base::WriteStringToFile(
          "TEST_CONTENT_2_CE", fake_package_ce_path + "/file2",
          0700, 10000, 20000, false /* follow_symlinks */));
  ASSERT_TRUE(android::base::WriteStringToFile(
          "TEST_CONTENT_2_DE", fake_package_de_path + "/file2",
          0700, 10000, 20000, false /* follow_symlinks */));

  ASSERT_BINDER_SUCCESS(service->snapshotAppData(std::make_unique<std::string>("TEST"),
          "com.foo", 0, 13, FLAG_STORAGE_DE | FLAG_STORAGE_CE, nullptr));

  // Previous snapshot (with data for file1) must be cleared.
  struct stat sb;
  ASSERT_EQ(-1, stat((rollback_ce_dir + "/file1").c_str(), &sb));
  ASSERT_EQ(-1, stat((rollback_de_dir + "/file1").c_str(), &sb));
  // New snapshot (with data for file2) must be present.
  ASSERT_NE(-1, stat((rollback_ce_dir + "/file2").c_str(), &sb));
  ASSERT_NE(-1, stat((rollback_de_dir + "/file2").c_str(), &sb));
}

TEST_F(AppDataSnapshotTest, SnapshotAppData_WrongVolumeUuid) {
  // Setup rollback folders to make sure that fails due to wrong volumeUuid being
  // passed, not because of some other reason.
  auto rollback_ce_dir = create_data_misc_ce_rollback_path("TEST", 0, 17);
  auto rollback_de_dir = create_data_misc_de_rollback_path("TEST", 0, 17);

  ASSERT_TRUE(mkdirs(rollback_ce_dir, 0700));
  ASSERT_TRUE(mkdirs(rollback_de_dir, 0700));

  EXPECT_BINDER_FAIL(service->snapshotAppData(std::make_unique<std::string>("FOO"),
          "com.foo", 0, 17, FLAG_STORAGE_DE, nullptr));
}

TEST_F(AppDataSnapshotTest, CreateAppDataSnapshot_ClearsCache) {
  auto fake_package_ce_cache_path = fake_package_ce_path + "/cache";
  auto fake_package_ce_code_cache_path = fake_package_ce_path + "/code_cache";
  auto fake_package_de_cache_path = fake_package_de_path + "/cache";
  auto fake_package_de_code_cache_path = fake_package_de_path + "/code_cache";

  ASSERT_TRUE(mkdirs(fake_package_ce_cache_path, 0700));
  ASSERT_TRUE(mkdirs(fake_package_ce_code_cache_path, 0700));
  ASSERT_TRUE(mkdirs(fake_package_de_cache_path, 0700));
  ASSERT_TRUE(mkdirs(fake_package_de_code_cache_path, 0700));

  ASSERT_TRUE(android::base::WriteStringToFile(
          "TEST_CONTENT_CE", fake_package_ce_cache_path + "/file1",
          0700, 10000, 20000, false /* follow_symlinks */));
  ASSERT_TRUE(android::base::WriteStringToFile(
          "TEST_CONTENT_CE", fake_package_ce_code_cache_path + "/file1",
          0700, 10000, 20000, false /* follow_symlinks */));
  ASSERT_TRUE(android::base::WriteStringToFile(
          "TEST_CONTENT_DE", fake_package_de_cache_path + "/file1",
          0700, 10000, 20000, false /* follow_symlinks */));
  ASSERT_TRUE(android::base::WriteStringToFile(
          "TEST_CONTENT_DE", fake_package_de_code_cache_path + "/file1",
          0700, 10000, 20000, false /* follow_symlinks */));
  ASSERT_BINDER_SUCCESS(service->snapshotAppData(std::make_unique<std::string>("TEST"),
          "com.foo", 0, 23, FLAG_STORAGE_CE | FLAG_STORAGE_DE, nullptr));
  // The snapshot call must clear cache.
  struct stat sb;
  ASSERT_EQ(-1, stat((fake_package_ce_cache_path + "/file1").c_str(), &sb));
  ASSERT_EQ(-1, stat((fake_package_ce_code_cache_path + "/file1").c_str(), &sb));
  ASSERT_EQ(-1, stat((fake_package_de_cache_path + "/file1").c_str(), &sb));
  ASSERT_EQ(-1, stat((fake_package_de_code_cache_path + "/file1").c_str(), &sb));
}

TEST_F(AppDataSnapshotTest, RestoreAppDataSnapshot) {
  auto rollback_ce_dir = create_data_misc_ce_rollback_path("TEST", 0, 239);
  auto rollback_de_dir = create_data_misc_de_rollback_path("TEST", 0, 239);

  ASSERT_TRUE(mkdirs(rollback_ce_dir, 0700));
  ASSERT_TRUE(mkdirs(rollback_de_dir, 0700));

  // Write contents to the rollback location. We'll write the same files to the
  // app data location and make sure the restore has overwritten them.
  ASSERT_TRUE(mkdirs(rollback_ce_dir + "/com.foo/", 0700));
  ASSERT_TRUE(mkdirs(rollback_de_dir + "/com.foo/", 0700));
  ASSERT_TRUE(android::base::WriteStringToFile(
          "CE_RESTORE_CONTENT", rollback_ce_dir + "/com.foo/file1",
          0700, 10000, 20000, false /* follow_symlinks */));
  ASSERT_TRUE(android::base::WriteStringToFile(
          "DE_RESTORE_CONTENT", rollback_de_dir + "/com.foo/file1",
          0700, 10000, 20000, false /* follow_symlinks */));
  ASSERT_TRUE(android::base::WriteStringToFile(
          "TEST_CONTENT_CE", fake_package_ce_path + "/file1",
          0700, 10000, 20000, false /* follow_symlinks */));
  ASSERT_TRUE(android::base::WriteStringToFile(
          "TEST_CONTENT_DE", fake_package_de_path + "/file1",
          0700, 10000, 20000, false /* follow_symlinks */));

  ASSERT_BINDER_SUCCESS(service->restoreAppDataSnapshot(std::make_unique<std::string>("TEST"),
          "com.foo", 10000, "", 0, 239, FLAG_STORAGE_DE | FLAG_STORAGE_CE));

  std::string ce_content, de_content;
  ASSERT_TRUE(android::base::ReadFileToString(
      fake_package_ce_path + "/file1", &ce_content, false /* follow_symlinks */));
  ASSERT_TRUE(android::base::ReadFileToString(
      fake_package_de_path + "/file1", &de_content, false /* follow_symlinks */));
  ASSERT_EQ("CE_RESTORE_CONTENT", ce_content);
  ASSERT_EQ("DE_RESTORE_CONTENT", de_content);
}

TEST_F(AppDataSnapshotTest, CreateSnapshotThenDestroyIt) {
  auto rollback_ce_dir = create_data_misc_ce_rollback_path("TEST", 0, 57);
  auto rollback_de_dir = create_data_misc_de_rollback_path("TEST", 0, 57);

  // Prepare data for snapshot.
  ASSERT_TRUE(android::base::WriteStringToFile(
          "TEST_CONTENT_CE", fake_package_ce_path + "/file1",
          0700, 10000, 20000, false /* follow_symlinks */));
  ASSERT_TRUE(android::base::WriteStringToFile(
          "TEST_CONTENT_DE", fake_package_de_path + "/file1",
          0700, 10000, 20000, false /* follow_symlinks */));

  int64_t ce_snapshot_inode;
  // Request a snapshot of both the CE as well as the DE content.
  ASSERT_TRUE(service->snapshotAppData(std::make_unique<std::string>("TEST"),
          "com.foo", 0, 57, FLAG_STORAGE_DE | FLAG_STORAGE_CE, &ce_snapshot_inode).isOk());
  // Because CE data snapshot was requested, ce_snapshot_inode can't be null.
  ASSERT_NE(0, ce_snapshot_inode);
  // Check snapshot is there.
  struct stat sb;
  ASSERT_EQ(0, stat((rollback_ce_dir + "/com.foo").c_str(), &sb));
  ASSERT_EQ(0, stat((rollback_de_dir + "/com.foo").c_str(), &sb));


  ASSERT_TRUE(service->destroyAppDataSnapshot(std::make_unique<std::string>("TEST"),
          "com.foo", 0, ce_snapshot_inode, 57, FLAG_STORAGE_DE | FLAG_STORAGE_CE).isOk());
  // Check snapshot is deleted.
  ASSERT_EQ(-1, stat((rollback_ce_dir + "/com.foo").c_str(), &sb));
  ASSERT_EQ(-1, stat((rollback_de_dir + "/com.foo").c_str(), &sb));
}

TEST_F(AppDataSnapshotTest, DestroyAppDataSnapshot_CeSnapshotInodeIsZero) {
  auto rollback_ce_dir = create_data_misc_ce_rollback_path("TEST", 0, 1543);
  auto rollback_de_dir = create_data_misc_de_rollback_path("TEST", 0, 1543);

  // Create a snapshot
  ASSERT_TRUE(mkdirs(rollback_ce_dir + "/com.foo/", 0700));
  ASSERT_TRUE(mkdirs(rollback_de_dir + "/com.foo/", 0700));
  ASSERT_TRUE(android::base::WriteStringToFile(
          "CE_RESTORE_CONTENT", rollback_ce_dir + "/com.foo/file1",
          0700, 10000, 20000, false /* follow_symlinks */));
  ASSERT_TRUE(android::base::WriteStringToFile(
          "DE_RESTORE_CONTENT", rollback_de_dir + "/com.foo/file1",
          0700, 10000, 20000, false /* follow_symlinks */));

  ASSERT_TRUE(service->destroyAppDataSnapshot(std::make_unique<std::string>("TEST"),
          "com.foo", 0, 0, 1543, FLAG_STORAGE_DE | FLAG_STORAGE_CE).isOk());

  // Check snapshot is deleted.
  struct stat sb;
  ASSERT_EQ(-1, stat((rollback_ce_dir + "/com.foo").c_str(), &sb));
  ASSERT_EQ(-1, stat((rollback_de_dir + "/com.foo").c_str(), &sb));

  // Check that deleting already deleted snapshot is no-op.
  ASSERT_TRUE(service->destroyAppDataSnapshot(std::make_unique<std::string>("TEST"),
          "com.foo", 0, 0, 1543, FLAG_STORAGE_DE | FLAG_STORAGE_CE).isOk());
}

TEST_F(AppDataSnapshotTest, DestroyAppDataSnapshot_WrongVolumeUuid) {
  // Setup rollback data to make sure that test fails due to wrong volumeUuid
  // being passed, not because of some other reason.
  auto rollback_ce_dir = create_data_misc_ce_rollback_path("TEST", 0, 43);
  auto rollback_de_dir = create_data_misc_de_rollback_path("TEST", 0, 43);

  ASSERT_TRUE(mkdirs(rollback_ce_dir, 0700));
  ASSERT_TRUE(mkdirs(rollback_de_dir, 0700));

  ASSERT_FALSE(service->destroyAppDataSnapshot(std::make_unique<std::string>("BAR"),
          "com.foo", 0, 0, 43, FLAG_STORAGE_DE).isOk());
}

TEST_F(AppDataSnapshotTest, RestoreAppDataSnapshot_WrongVolumeUuid) {
  // Setup rollback data to make sure that fails due to wrong volumeUuid being
  // passed, not because of some other reason.
  auto rollback_ce_dir = create_data_misc_ce_rollback_path("TEST", 0, 41);
  auto rollback_de_dir = create_data_misc_de_rollback_path("TEST", 0, 41);

  ASSERT_TRUE(mkdirs(rollback_ce_dir, 0700));
  ASSERT_TRUE(mkdirs(rollback_de_dir, 0700));

  EXPECT_BINDER_FAIL(service->restoreAppDataSnapshot(std::make_unique<std::string>("BAR"),
          "com.foo", 10000, "", 0, 41, FLAG_STORAGE_DE));
}

}  // namespace installd
}  // namespace android
