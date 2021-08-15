/*
 * Copyright (C) 2011 The Android Open Source Project
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

#include <android-base/logging.h>
#include <android-base/scopeguard.h>
#include <gtest/gtest.h>

#include "InstalldNativeService.h"
#include "MatchExtensionGen.h"
#include "globals.h"
#include "utils.h"

#undef LOG_TAG
#define LOG_TAG "utils_test"

#define TEST_DATA_DIR "/data/"
#define TEST_ROOT_DIR "/system/"
#define TEST_APP_DIR "/data/app/"
#define TEST_APP_PRIVATE_DIR "/data/app-private/"
#define TEST_APP_EPHEMERAL_DIR "/data/app-ephemeral/"
#define TEST_ASEC_DIR "/mnt/asec/"
#define TEST_EXPAND_DIR "/mnt/expand/00000000-0000-0000-0000-000000000000/"

#define TEST_SYSTEM_DIR1 "/system/app/"
#define TEST_SYSTEM_DIR2 "/vendor/app/"

#define TEST_PROFILE_DIR "/data/misc/profiles"

namespace android {
namespace installd {

class UtilsTest : public testing::Test {
protected:
    virtual void SetUp() {
        setenv("ANDROID_LOG_TAGS", "*:v", 1);
        android::base::InitLogging(nullptr);

        init_globals_from_data_and_root(TEST_DATA_DIR, TEST_ROOT_DIR);
    }

    virtual void TearDown() {
    }

    std::string create_too_long_path(const std::string& seed) {
        std::string result = seed;
        for (size_t i = seed.size(); i < PKG_PATH_MAX; i++) {
            result += "a";
        }
        return result;
    }
};

TEST_F(UtilsTest, IsValidApkPath_BadPrefix) {
    // Bad prefixes directories
    const char *badprefix1 = "/etc/passwd";
    EXPECT_EQ(-1, validate_apk_path(badprefix1))
            << badprefix1 << " should not be allowed as a valid path";

    const char *badprefix2 = "../.." TEST_APP_DIR "../../../blah";
    EXPECT_EQ(-1, validate_apk_path(badprefix2))
            << badprefix2 << " should not be allowed as a valid path";

    const char *badprefix3 = "init.rc";
    EXPECT_EQ(-1, validate_apk_path(badprefix3))
            << badprefix3 << " should not be allowed as a valid path";

    const char *badprefix4 = "/init.rc";
    EXPECT_EQ(-1, validate_apk_path(badprefix4))
            << badprefix4 << " should not be allowed as a valid path";
}

TEST_F(UtilsTest, IsValidApkPath_Internal) {
    // Internal directories
    const char *internal1 = TEST_APP_DIR "example.apk";
    EXPECT_EQ(0, validate_apk_path(internal1))
            << internal1 << " should be allowed as a valid path";

    // b/16888084
    const char *path2 = TEST_APP_DIR "example.com/example.apk";
    EXPECT_EQ(0, validate_apk_path(path2))
            << path2 << " should be allowed as a valid path";

    const char *badint1 = TEST_APP_DIR "../example.apk";
    EXPECT_EQ(-1, validate_apk_path(badint1))
            << badint1 << " should be rejected as a invalid path";

    const char *badint2 = TEST_APP_DIR "/../example.apk";
    EXPECT_EQ(-1, validate_apk_path(badint2))
            << badint2 << " should be rejected as a invalid path";

    // Only one subdir should be allowed.
    const char *bad_path3 = TEST_APP_DIR "example.com/subdir/pkg.apk";
    EXPECT_EQ(-1, validate_apk_path(bad_path3))
            << bad_path3 << " should be rejected as a invalid path";

    const char *bad_path4 = TEST_APP_DIR "example.com/subdir/../pkg.apk";
    EXPECT_EQ(-1, validate_apk_path(bad_path4))
            << bad_path4 << " should be rejected as a invalid path";

    const char *bad_path5 = TEST_APP_DIR "example.com1/../example.com2/pkg.apk";
    EXPECT_EQ(-1, validate_apk_path(bad_path5))
            << bad_path5 << " should be rejected as a invalid path";
}

TEST_F(UtilsTest, IsValidApkPath_TopDir) {
    EXPECT_EQ(0, validate_apk_path(TEST_DATA_DIR "app/com.example"));
    EXPECT_EQ(0, validate_apk_path(TEST_EXPAND_DIR "app/com.example"));
    EXPECT_EQ(-1, validate_apk_path(TEST_DATA_DIR "data/com.example"));
    EXPECT_EQ(-1, validate_apk_path(TEST_EXPAND_DIR "data/com.example"));
}

TEST_F(UtilsTest, IsValidApkPath_TopFile) {
    EXPECT_EQ(0, validate_apk_path(TEST_DATA_DIR "app/com.example/base.apk"));
    EXPECT_EQ(0, validate_apk_path(TEST_EXPAND_DIR "app/com.example/base.apk"));
    EXPECT_EQ(-1, validate_apk_path(TEST_DATA_DIR "data/com.example/base.apk"));
    EXPECT_EQ(-1, validate_apk_path(TEST_EXPAND_DIR "data/com.example/base.apk"));
}

TEST_F(UtilsTest, IsValidApkPath_OatDir) {
    EXPECT_EQ(0, validate_apk_path_subdirs(TEST_DATA_DIR "app/com.example/oat"));
    EXPECT_EQ(0, validate_apk_path_subdirs(TEST_EXPAND_DIR "app/com.example/oat"));
    EXPECT_EQ(-1, validate_apk_path_subdirs(TEST_DATA_DIR "data/com.example/oat"));
    EXPECT_EQ(-1, validate_apk_path_subdirs(TEST_EXPAND_DIR "data/com.example/oat"));
}

TEST_F(UtilsTest, IsValidApkPath_OatDirDir) {
    EXPECT_EQ(0, validate_apk_path_subdirs(TEST_DATA_DIR "app/com.example/oat/arm64"));
    EXPECT_EQ(0, validate_apk_path_subdirs(TEST_EXPAND_DIR "app/com.example/oat/arm64"));
    EXPECT_EQ(-1, validate_apk_path_subdirs(TEST_DATA_DIR "data/com.example/oat/arm64"));
    EXPECT_EQ(-1, validate_apk_path_subdirs(TEST_EXPAND_DIR "data/com.example/oat/arm64"));
}

TEST_F(UtilsTest, IsValidApkPath_OatDirDirFile) {
    EXPECT_EQ(0, validate_apk_path_subdirs(TEST_DATA_DIR "app/com.example/oat/arm64/base.odex"));
    EXPECT_EQ(0, validate_apk_path_subdirs(TEST_EXPAND_DIR "app/com.example/oat/arm64/base.odex"));
    EXPECT_EQ(-1, validate_apk_path_subdirs(TEST_DATA_DIR "data/com.example/oat/arm64/base.odex"));
    EXPECT_EQ(-1, validate_apk_path_subdirs(TEST_EXPAND_DIR "data/com.example/oat/arm64/base.odex"));
}

TEST_F(UtilsTest, IsValidApkPath_Private) {
    // Internal directories
    const char *private1 = TEST_APP_PRIVATE_DIR "example.apk";
    EXPECT_EQ(0, validate_apk_path(private1))
            << private1 << " should be allowed as a valid path";

    // b/16888084
    const char *path2 = TEST_APP_DIR "example.com/example.apk";
    EXPECT_EQ(0, validate_apk_path(path2))
            << path2 << " should be allowed as a valid path";

    const char *badpriv1 = TEST_APP_PRIVATE_DIR "../example.apk";
    EXPECT_EQ(-1, validate_apk_path(badpriv1))
            << badpriv1 << " should be rejected as a invalid path";

    const char *badpriv2 = TEST_APP_PRIVATE_DIR "/../example.apk";
    EXPECT_EQ(-1, validate_apk_path(badpriv2))
            << badpriv2 << " should be rejected as a invalid path";

    // Only one subdir should be allowed.
    const char *bad_path3 = TEST_APP_PRIVATE_DIR "example.com/subdir/pkg.apk";
    EXPECT_EQ(-1, validate_apk_path(bad_path3))
            << bad_path3 << " should be rejected as a invalid path";

    const char *bad_path4 = TEST_APP_PRIVATE_DIR "example.com/subdir/../pkg.apk";
    EXPECT_EQ(-1, validate_apk_path(bad_path4))
            << bad_path4 << " should be rejected as a invalid path";

    const char *bad_path5 = TEST_APP_PRIVATE_DIR "example.com1/../example.com2/pkg.apk";
    EXPECT_EQ(-1, validate_apk_path(bad_path5))
            << bad_path5 << " should be rejected as a invalid path";
}


TEST_F(UtilsTest, IsValidApkPath_AsecGood1) {
    const char *asec1 = TEST_ASEC_DIR "example.apk";
    EXPECT_EQ(0, validate_apk_path(asec1))
            << asec1 << " should be allowed as a valid path";
}

TEST_F(UtilsTest, IsValidApkPath_AsecGood2) {
    const char *asec2 = TEST_ASEC_DIR "com.example.asec/pkg.apk";
    EXPECT_EQ(0, validate_apk_path(asec2))
            << asec2 << " should be allowed as a valid path";
}

TEST_F(UtilsTest, IsValidApkPath_EscapeFail) {
    const char *badasec1 = TEST_ASEC_DIR "../example.apk";
    EXPECT_EQ(-1, validate_apk_path(badasec1))
            << badasec1 << " should be rejected as a invalid path";
}

TEST_F(UtilsTest, IsValidApkPath_SubdirEscapeFail) {
    const char *badasec3 = TEST_ASEC_DIR "com.example.asec/../../../pkg.apk";
    EXPECT_EQ(-1, validate_apk_path(badasec3))
            << badasec3  << " should be rejected as a invalid path";
}

TEST_F(UtilsTest, IsValidApkPath_SlashEscapeFail) {
    const char *badasec4 = TEST_ASEC_DIR "/../example.apk";
    EXPECT_EQ(-1, validate_apk_path(badasec4))
            << badasec4 << " should be rejected as a invalid path";
}

TEST_F(UtilsTest, IsValidApkPath_CrazyDirFail) {
    const char *badasec5 = TEST_ASEC_DIR ".//../..";
    EXPECT_EQ(-1, validate_apk_path(badasec5))
            << badasec5 << " should be rejected as a invalid path";
}

TEST_F(UtilsTest, IsValidApkPath_SubdirEscapeSingleFail) {
    const char *badasec6 = TEST_ASEC_DIR "com.example.asec/../pkg.apk";
    EXPECT_EQ(-1, validate_apk_path(badasec6))
            << badasec6 << " should be rejected as a invalid path";
}

TEST_F(UtilsTest, IsValidApkPath_TwoSubdirFail) {
    const char *badasec7 = TEST_ASEC_DIR "com.example.asec/subdir1/pkg.apk";
    EXPECT_EQ(-1, validate_apk_path(badasec7))
            << badasec7 << " should be rejected as a invalid path";
}

TEST_F(UtilsTest, CheckSystemApp_Dir1) {
    const char *sysapp1 = TEST_SYSTEM_DIR1 "Voice.apk";
    EXPECT_EQ(0, validate_system_app_path(sysapp1))
            << sysapp1 << " should be allowed as a system path";
}

TEST_F(UtilsTest, CheckSystemApp_Dir2) {
    const char *sysapp2 = TEST_SYSTEM_DIR2 "com.example.myapp.apk";
    EXPECT_EQ(0, validate_system_app_path(sysapp2))
            << sysapp2 << " should be allowed as a system path";
}

TEST_F(UtilsTest, CheckSystemApp_EscapeFail) {
    const char *badapp1 = TEST_SYSTEM_DIR1 "../com.example.apk";
    EXPECT_EQ(-1, validate_system_app_path(badapp1))
            << badapp1 << " should be rejected not a system path";
}

TEST_F(UtilsTest, CheckSystemApp_DoubleEscapeFail) {
    const char *badapp2 = TEST_SYSTEM_DIR2 "/../../com.example.apk";
    EXPECT_EQ(-1, validate_system_app_path(badapp2))
            << badapp2 << " should be rejected not a system path";
}

TEST_F(UtilsTest, CheckSystemApp_BadPathEscapeFail) {
    const char *badapp3 = TEST_APP_DIR "/../../com.example.apk";
    EXPECT_EQ(-1, validate_system_app_path(badapp3))
            << badapp3 << " should be rejected not a system path";
}

TEST_F(UtilsTest, CheckSystemApp_Subdir) {
    const char *sysapp = TEST_SYSTEM_DIR1 "com.example/com.example.apk";
    EXPECT_EQ(0, validate_system_app_path(sysapp))
            << sysapp << " should be allowed as a system path";

    const char *badapp = TEST_SYSTEM_DIR1 "com.example/subdir/com.example.apk";
    EXPECT_EQ(-1, validate_system_app_path(badapp))
            << badapp << " should be rejected not a system path";

    const char *badapp1 = TEST_SYSTEM_DIR1 "com.example/subdir/../com.example.apk";
    EXPECT_EQ(-1, validate_system_app_path(badapp1))
            << badapp1 << " should be rejected not a system path";

    const char *badapp2 = TEST_SYSTEM_DIR1 "com.example1/../com.example2/com.example.apk";
    EXPECT_EQ(-1, validate_system_app_path(badapp2))
            << badapp2 << " should be rejected not a system path";
}

TEST_F(UtilsTest, CreateDataPath) {
    EXPECT_EQ("/data", create_data_path(nullptr));
    EXPECT_EQ("/mnt/expand/57f8f4bc-abf4-655f-bf67-946fc0f9f25b",
            create_data_path("57f8f4bc-abf4-655f-bf67-946fc0f9f25b"));
}

TEST_F(UtilsTest, CreateDataAppPath) {
    EXPECT_EQ("/data/app", create_data_app_path(nullptr));

    EXPECT_EQ("/mnt/expand/57f8f4bc-abf4-655f-bf67-946fc0f9f25b/app",
            create_data_app_path("57f8f4bc-abf4-655f-bf67-946fc0f9f25b"));
}

TEST_F(UtilsTest, CreateDataUserPath) {
    EXPECT_EQ("/data/data", create_data_user_ce_path(nullptr, 0));
    EXPECT_EQ("/data/user/10", create_data_user_ce_path(nullptr, 10));

    EXPECT_EQ("/mnt/expand/57f8f4bc-abf4-655f-bf67-946fc0f9f25b/user/0",
            create_data_user_ce_path("57f8f4bc-abf4-655f-bf67-946fc0f9f25b", 0));
    EXPECT_EQ("/mnt/expand/57f8f4bc-abf4-655f-bf67-946fc0f9f25b/user/10",
            create_data_user_ce_path("57f8f4bc-abf4-655f-bf67-946fc0f9f25b", 10));
}

TEST_F(UtilsTest, CreateDataMediaPath) {
    EXPECT_EQ("/data/media/0", create_data_media_path(nullptr, 0));
    EXPECT_EQ("/data/media/10", create_data_media_path(nullptr, 10));

    EXPECT_EQ("/mnt/expand/57f8f4bc-abf4-655f-bf67-946fc0f9f25b/media/0",
            create_data_media_path("57f8f4bc-abf4-655f-bf67-946fc0f9f25b", 0));
    EXPECT_EQ("/mnt/expand/57f8f4bc-abf4-655f-bf67-946fc0f9f25b/media/10",
            create_data_media_path("57f8f4bc-abf4-655f-bf67-946fc0f9f25b", 10));
}

TEST_F(UtilsTest, CreateDataAppPackagePath) {
    EXPECT_EQ("/data/app/com.example", create_data_app_package_path(nullptr, "com.example"));

    EXPECT_EQ("/mnt/expand/57f8f4bc-abf4-655f-bf67-946fc0f9f25b/app/com.example",
            create_data_app_package_path("57f8f4bc-abf4-655f-bf67-946fc0f9f25b", "com.example"));
}

TEST_F(UtilsTest, CreateDataUserPackagePath) {
    EXPECT_EQ("/data/data/com.example", create_data_user_ce_package_path(nullptr, 0, "com.example"));
    EXPECT_EQ("/data/user/10/com.example", create_data_user_ce_package_path(nullptr, 10, "com.example"));

    EXPECT_EQ("/mnt/expand/57f8f4bc-abf4-655f-bf67-946fc0f9f25b/user/0/com.example",
            create_data_user_ce_package_path("57f8f4bc-abf4-655f-bf67-946fc0f9f25b", 0, "com.example"));
    EXPECT_EQ("/mnt/expand/57f8f4bc-abf4-655f-bf67-946fc0f9f25b/user/10/com.example",
            create_data_user_ce_package_path("57f8f4bc-abf4-655f-bf67-946fc0f9f25b", 10, "com.example"));
}

TEST_F(UtilsTest, IsValidPackageName) {
    EXPECT_EQ(true, is_valid_package_name("android"));
    EXPECT_EQ(true, is_valid_package_name("com.example"));
    EXPECT_EQ(true, is_valid_package_name("com.example-1"));
    EXPECT_EQ(true, is_valid_package_name("com.example-1024"));
    EXPECT_EQ(true, is_valid_package_name("com.example.foo---KiJFj4a_tePVw95pSrjg=="));
    EXPECT_EQ(true, is_valid_package_name("really_LONG.a1234.package_name"));

    EXPECT_EQ(false, is_valid_package_name("1234.package"));
    EXPECT_EQ(false, is_valid_package_name("com.1234.package"));
    EXPECT_EQ(false, is_valid_package_name(""));
    EXPECT_EQ(false, is_valid_package_name("."));
    EXPECT_EQ(false, is_valid_package_name(".."));
    EXPECT_EQ(false, is_valid_package_name("../"));
    EXPECT_EQ(false, is_valid_package_name("com.example/../com.evil/"));
    EXPECT_EQ(false, is_valid_package_name("com.example-1/../com.evil/"));
    EXPECT_EQ(false, is_valid_package_name("/com.evil"));
}

TEST_F(UtilsTest, CreateDataUserProfilePath) {
    EXPECT_EQ("/data/misc/profiles/cur/0", create_primary_cur_profile_dir_path(0));
    EXPECT_EQ("/data/misc/profiles/cur/1", create_primary_cur_profile_dir_path(1));
}

TEST_F(UtilsTest, CreateDataUserProfilePackagePath) {
    EXPECT_EQ("/data/misc/profiles/cur/0/com.example",
            create_primary_current_profile_package_dir_path(0, "com.example"));
    EXPECT_EQ("/data/misc/profiles/cur/1/com.example",
            create_primary_current_profile_package_dir_path(1, "com.example"));
}

TEST_F(UtilsTest, CreateDataRefProfilePath) {
    EXPECT_EQ("/data/misc/profiles/ref", create_primary_ref_profile_dir_path());
}

TEST_F(UtilsTest, CreateDataRefProfilePackagePath) {
    EXPECT_EQ("/data/misc/profiles/ref/com.example",
        create_primary_reference_profile_package_dir_path("com.example"));
}

TEST_F(UtilsTest, CreatePrimaryCurrentProfile) {
    std::string expected_base =
        create_primary_current_profile_package_dir_path(0, "com.example") + "/primary.prof";
    EXPECT_EQ(expected_base,
            create_current_profile_path(/*user*/0, "com.example", "primary.prof",
                    /*is_secondary*/false));

    std::string expected_split =
        create_primary_current_profile_package_dir_path(0, "com.example") + "/split.prof";
    EXPECT_EQ(expected_split,
            create_current_profile_path(/*user*/0, "com.example", "split.prof",
                    /*is_secondary*/false));
}

TEST_F(UtilsTest, CreatePrimaryReferenceProfile) {
    std::string expected_base =
        create_primary_reference_profile_package_dir_path("com.example") + "/primary.prof";
    EXPECT_EQ(expected_base,
            create_reference_profile_path("com.example", "primary.prof", /*is_secondary*/false));

    std::string expected_split =
        create_primary_reference_profile_package_dir_path("com.example") + "/split.prof";
    EXPECT_EQ(expected_split,
            create_reference_profile_path("com.example", "split.prof", /*is_secondary*/false));
}

TEST_F(UtilsTest, CreateProfileSnapshot) {
    std::string expected_base =
        create_primary_reference_profile_package_dir_path("com.example") + "/primary.prof.snapshot";
    EXPECT_EQ(expected_base, create_snapshot_profile_path("com.example", "primary.prof"));

    std::string expected_split =
        create_primary_reference_profile_package_dir_path("com.example") + "/split.prof.snapshot";
    EXPECT_EQ(expected_split, create_snapshot_profile_path("com.example", "split.prof"));
}

TEST_F(UtilsTest, CreateSecondaryCurrentProfile) {
    EXPECT_EQ("/data/user/0/com.example/oat/secondary.dex.cur.prof",
            create_current_profile_path(/*user*/0, "com.example",
                    "/data/user/0/com.example/secondary.dex", /*is_secondary*/true));
}

TEST_F(UtilsTest, CreateSecondaryReferenceProfile) {
    EXPECT_EQ("/data/user/0/com.example/oat/secondary.dex.prof",
            create_reference_profile_path("com.example",
                    "/data/user/0/com.example/secondary.dex", /*is_secondary*/true));
}

static void pass_secondary_dex_validation(const std::string& package_name,
        const std::string& dex_path, int uid, int storage_flag) {
    EXPECT_TRUE(validate_secondary_dex_path(package_name, dex_path, /*volume_uuid*/ nullptr, uid,
            storage_flag))
            << dex_path << " should be allowed as a valid secondary dex path";
}

static void fail_secondary_dex_validation(const std::string& package_name,
        const std::string& dex_path, int uid, int storage_flag) {
    EXPECT_FALSE(validate_secondary_dex_path(package_name, dex_path, /*volume_uuid*/ nullptr, uid,
            storage_flag))
            << dex_path << " should not be allowed as a valid secondary dex path";
}

TEST_F(UtilsTest, ValidateSecondaryDexFilesPath) {
    std::string package_name = "com.test.app";
    std::string app_dir_ce_user_0 = "/data/data/" + package_name;
    std::string app_dir_ce_user_0_link = "/data/user/0/" + package_name;
    std::string app_dir_ce_user_10 = "/data/user/10/" + package_name;

    std::string app_dir_de_user_0 = "/data/user_de/0/" + package_name;
    std::string app_dir_de_user_10 = "/data/user_de/10/" + package_name;

    EXPECT_EQ(app_dir_ce_user_0,
            create_data_user_ce_package_path(nullptr, 0, package_name.c_str()));
    EXPECT_EQ(app_dir_ce_user_10,
            create_data_user_ce_package_path(nullptr, 10, package_name.c_str()));

    EXPECT_EQ(app_dir_de_user_0,
            create_data_user_de_package_path(nullptr, 0, package_name.c_str()));
    EXPECT_EQ(app_dir_de_user_10,
            create_data_user_de_package_path(nullptr, 10, package_name.c_str()));

    uid_t app_uid_for_user_0 = multiuser_get_uid(/*user_id*/0, /*app_id*/ 1234);
    uid_t app_uid_for_user_10 = multiuser_get_uid(/*user_id*/10, /*app_id*/ 1234);

    // Standard path for user 0 on CE storage.
    pass_secondary_dex_validation(
        package_name, app_dir_ce_user_0 + "/ce0.dex", app_uid_for_user_0, FLAG_STORAGE_CE);
    pass_secondary_dex_validation(
        package_name, app_dir_ce_user_0_link + "/ce0.dex", app_uid_for_user_0, FLAG_STORAGE_CE);
    // Standard path for user 10 on CE storage.
    pass_secondary_dex_validation(
        package_name, app_dir_ce_user_10 + "/ce10.dex", app_uid_for_user_10, FLAG_STORAGE_CE);

    // Standard path for user 0 on DE storage.
    pass_secondary_dex_validation(
        package_name, app_dir_de_user_0 + "/de0.dex", app_uid_for_user_0, FLAG_STORAGE_DE);
    // Standard path for user 10 on DE storage.
    pass_secondary_dex_validation(
        package_name, app_dir_de_user_10 + "/de0.dex", app_uid_for_user_10, FLAG_STORAGE_DE);

    // Dex path for user 0 accessed from user 10.
    fail_secondary_dex_validation(
        package_name, app_dir_ce_user_0 + "/path0_from10.dex",
        app_uid_for_user_10, FLAG_STORAGE_CE);

    // Dex path for CE storage accessed with DE.
    fail_secondary_dex_validation(
        package_name, app_dir_ce_user_0 + "/ce_from_de.dex", app_uid_for_user_0, FLAG_STORAGE_DE);

    // Dex path for DE storage accessed with CE.
    fail_secondary_dex_validation(
        package_name, app_dir_de_user_0 + "/de_from_ce.dex", app_uid_for_user_0, FLAG_STORAGE_CE);

    // Location which does not start with '/'.
    fail_secondary_dex_validation(
        package_name, "without_slash.dex", app_uid_for_user_10, FLAG_STORAGE_DE);

    // The dex file is not in the specified package directory.
    fail_secondary_dex_validation(
        "another.package", app_dir_ce_user_0 + "/for_another_package.dex",
        app_uid_for_user_0, FLAG_STORAGE_DE);

    // The dex path contains indirect directories.
    fail_secondary_dex_validation(
        package_name, app_dir_ce_user_0 + "/1/../foo.dex", app_uid_for_user_0, FLAG_STORAGE_CE);
    fail_secondary_dex_validation(
        package_name, app_dir_ce_user_0 + "/1/./foo.dex", app_uid_for_user_0, FLAG_STORAGE_CE);

    // Super long path.
    std::string too_long = create_too_long_path("too_long_");
    fail_secondary_dex_validation(
        package_name, app_dir_ce_user_10 + "/" + too_long, app_uid_for_user_10, FLAG_STORAGE_CE);
}

TEST_F(UtilsTest, ValidateApkPath) {
    EXPECT_EQ(0, validate_apk_path("/data/app/com.example"));
    EXPECT_EQ(0, validate_apk_path("/data/app/com.example/file"));
    EXPECT_EQ(0, validate_apk_path("/data/app/com.example//file"));
    EXPECT_NE(0, validate_apk_path("/data/app/com.example/dir/"));
    EXPECT_NE(0, validate_apk_path("/data/app/com.example/dir/file"));
    EXPECT_NE(0, validate_apk_path("/data/app/com.example/dir/dir/file"));
    EXPECT_NE(0, validate_apk_path("/data/app/com.example/dir/dir//file"));
    EXPECT_NE(0, validate_apk_path("/data/app/com.example/dir/dir/dir/file"));
    EXPECT_NE(0, validate_apk_path("/data/app/com.example/dir/dir/dir//file"));
}

TEST_F(UtilsTest, ValidateApkPathSubdirs) {
    EXPECT_EQ(0, validate_apk_path_subdirs("/data/app/com.example"));
    EXPECT_EQ(0, validate_apk_path_subdirs("/data/app/com.example/file"));
    EXPECT_EQ(0, validate_apk_path_subdirs("/data/app/com.example//file"));
    EXPECT_EQ(0, validate_apk_path_subdirs("/data/app/com.example/dir/"));
    EXPECT_EQ(0, validate_apk_path_subdirs("/data/app/com.example/dir/file"));
    EXPECT_EQ(0, validate_apk_path_subdirs("/data/app/com.example/dir/dir/file"));
    EXPECT_EQ(0, validate_apk_path_subdirs("/data/app/com.example/dir/dir//file"));
    EXPECT_NE(0, validate_apk_path_subdirs("/data/app/com.example/dir/dir/dir/file"));
    EXPECT_NE(0, validate_apk_path_subdirs("/data/app/com.example/dir/dir/dir//file"));
}

TEST_F(UtilsTest, MatchExtension_Valid) {
    EXPECT_EQ(AID_MEDIA_VIDEO, MatchExtension("mpg"));
    EXPECT_EQ(AID_MEDIA_VIDEO, MatchExtension("mpeg"));
    EXPECT_EQ(AID_MEDIA_VIDEO, MatchExtension("mPeG"));
    EXPECT_EQ(AID_MEDIA_VIDEO, MatchExtension("MPEG"));
}

TEST_F(UtilsTest, MatchExtension_Invalid) {
    EXPECT_EQ(0, MatchExtension("log"));
    EXPECT_EQ(0, MatchExtension("3amp"));
    EXPECT_EQ(0, MatchExtension("fpe"));
    EXPECT_EQ(0, MatchExtension("docx"));
}

TEST_F(UtilsTest, TestRollbackPaths) {
    EXPECT_EQ("/data/misc_ce/0/rollback/239/com.foo",
            create_data_misc_ce_rollback_package_path(nullptr, 0, 239, "com.foo"));
    EXPECT_EQ("/data/misc_ce/10/rollback/37/com.foo",
            create_data_misc_ce_rollback_package_path(nullptr, 10, 37, "com.foo"));

    EXPECT_EQ("/data/misc_de/0/rollback/73/com.foo",
            create_data_misc_de_rollback_package_path(nullptr, 0, 73, "com.foo"));
    EXPECT_EQ("/data/misc_de/10/rollback/13/com.foo",
            create_data_misc_de_rollback_package_path(nullptr, 10, 13, "com.foo"));

    EXPECT_EQ("/data/misc_ce/0/rollback/57",
            create_data_misc_ce_rollback_path(nullptr, 0, 57));
    EXPECT_EQ("/data/misc_ce/10/rollback/1543",
            create_data_misc_ce_rollback_path(nullptr, 10, 1543));

    EXPECT_EQ("/data/misc_de/0/rollback/43",
            create_data_misc_de_rollback_path(nullptr, 0, 43));
    EXPECT_EQ("/data/misc_de/10/rollback/41",
            create_data_misc_de_rollback_path(nullptr, 10, 41));

    EXPECT_EQ("/data/misc_ce/0/rollback/17/com.foo",
            create_data_misc_ce_rollback_package_path(nullptr, 0, 17, "com.foo", 0));
    EXPECT_EQ("/data/misc_ce/0/rollback/19/com.foo",
            create_data_misc_ce_rollback_package_path(nullptr, 0, 19, "com.foo", 239));

    auto rollback_ce_path = create_data_misc_ce_rollback_path(nullptr, 0, 53);
    auto rollback_ce_package_path = create_data_misc_ce_rollback_package_path(nullptr, 0, 53,
            "com.foo");
    auto deleter = [&rollback_ce_path]() {
        delete_dir_contents_and_dir(rollback_ce_path, true /* ignore_if_missing */);
    };
    auto scope_guard = android::base::make_scope_guard(deleter);

    EXPECT_NE(-1, mkdir(rollback_ce_path.c_str(), 700));
    EXPECT_NE(-1, mkdir(rollback_ce_package_path.c_str(), 700));

    ino_t ce_data_inode;
    EXPECT_EQ(0, get_path_inode(rollback_ce_package_path, &ce_data_inode));

    EXPECT_EQ("/data/misc_ce/0/rollback/53/com.foo",
            create_data_misc_ce_rollback_package_path(nullptr, 0, 53, "com.foo", ce_data_inode));
    // Check that path defined by inode is picked even if it's not the same as
    // the fallback one.
    EXPECT_EQ("/data/misc_ce/0/rollback/53/com.foo",
            create_data_misc_ce_rollback_package_path(nullptr, 0, 53, "com.bar", ce_data_inode));

    // These last couple of cases are never exercised in production because we
    // only snapshot apps in the primary data partition. Exercise them here for
    // the sake of completeness.
    EXPECT_EQ("/mnt/expand/57f8f4bc-abf4-655f-bf67-946fc0f9f25b/misc_ce/0/rollback/7/com.example",
            create_data_misc_ce_rollback_package_path("57f8f4bc-abf4-655f-bf67-946fc0f9f25b", 0, 7,
                    "com.example"));
    EXPECT_EQ("/mnt/expand/57f8f4bc-abf4-655f-bf67-946fc0f9f25b/misc_de/0/rollback/11/com.example",
            create_data_misc_de_rollback_package_path("57f8f4bc-abf4-655f-bf67-946fc0f9f25b", 0, 11,
                    "com.example"));
}

TEST_F(UtilsTest, TestCreateDirIfNeeded) {
    system("mkdir -p /data/local/tmp/user/0");

    auto deleter = [&]() {
        delete_dir_contents_and_dir("/data/local/tmp/user/0", true /* ignore_if_missing */);
    };
    auto scope_guard = android::base::make_scope_guard(deleter);

    // Create folder and check it's permissions.
    ASSERT_EQ(0, create_dir_if_needed("/data/local/tmp/user/0/foo", 0700));
    struct stat st;
    ASSERT_EQ(0, stat("/data/local/tmp/user/0/foo", &st));
    ASSERT_EQ(0700, st.st_mode & ALLPERMS);

    // Check that create_dir_if_needed is no-op if folder already exists with
    // correct permissions.
    ASSERT_EQ(0, create_dir_if_needed("/data/local/tmp/user/0/foo", 0700));

    // Check -1 is returned if folder exists but with different permissions.
    ASSERT_EQ(-1, create_dir_if_needed("/data/local/tmp/user/0/foo", 0750));

    // Check that call fails if parent doesn't exist.
    ASSERT_NE(0, create_dir_if_needed("/data/local/tmp/user/0/bar/baz", 0700));
}

}  // namespace installd
}  // namespace android
