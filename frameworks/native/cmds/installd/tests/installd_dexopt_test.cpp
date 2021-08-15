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

#include <cstdlib>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/properties.h>
#include <android-base/scopeguard.h>
#include <android-base/stringprintf.h>
#include <android-base/unique_fd.h>
#include <binder/Status.h>
#include <cutils/properties.h>

#include <gtest/gtest.h>

#include <selinux/android.h>
#include <selinux/avc.h>

#include "binder_test_utils.h"
#include "dexopt.h"
#include "InstalldNativeService.h"
#include "globals.h"
#include "tests/test_utils.h"
#include "utils.h"

using android::base::ReadFully;
using android::base::unique_fd;

namespace android {
namespace installd {

// TODO(calin): try to dedup this code.
#if defined(__arm__)
static const std::string kRuntimeIsa = "arm";
#elif defined(__aarch64__)
static const std::string kRuntimeIsa = "arm64";
#elif defined(__mips__) && !defined(__LP64__)
static const std::string kRuntimeIsa = "mips";
#elif defined(__mips__) && defined(__LP64__)
static const std::string kRuntimeIsa = "mips64";
#elif defined(__i386__)
static const std::string kRuntimeIsa = "x86";
#elif defined(__x86_64__)
static const std::string kRuntimeIsa = "x86_64";
#else
static const std::string kRuntimeIsa = "none";
#endif

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

static void run_cmd(const std::string& cmd) {
    system(cmd.c_str());
}

template <typename Visitor>
static void run_cmd_and_process_output(const std::string& cmd, const Visitor& visitor) {
    FILE* file = popen(cmd.c_str(), "r");
    CHECK(file != nullptr) << "Failed to ptrace " << cmd;
    char* line = nullptr;
    while (true) {
        size_t n = 0u;
        ssize_t value = getline(&line, &n, file);
        if (value == -1) {
            break;
        }
        visitor(line);
    }
    free(line);
    fclose(file);
}

static int mkdir(const std::string& path, uid_t owner, gid_t group, mode_t mode) {
    int ret = ::mkdir(path.c_str(), mode);
    if (ret != 0) {
        return ret;
    }
    ret = ::chown(path.c_str(), owner, group);
    if (ret != 0) {
        return ret;
    }
    return ::chmod(path.c_str(), mode);
}

static int log_callback(int type, const char *fmt, ...) { // NOLINT
    va_list ap;
    int priority;

    switch (type) {
        case SELINUX_WARNING:
            priority = ANDROID_LOG_WARN;
            break;
        case SELINUX_INFO:
            priority = ANDROID_LOG_INFO;
            break;
        default:
            priority = ANDROID_LOG_ERROR;
            break;
    }
    va_start(ap, fmt);
    LOG_PRI_VA(priority, "SELinux", fmt, ap);
    va_end(ap);
    return 0;
}

static bool init_selinux() {
    int selinux_enabled = (is_selinux_enabled() > 0);

    union selinux_callback cb;
    cb.func_log = log_callback;
    selinux_set_callback(SELINUX_CB_LOG, cb);

    if (selinux_enabled && selinux_status_open(true) < 0) {
        LOG(ERROR) << "Could not open selinux status; exiting";
        return false;
    }

    return true;
}

// Base64 encoding of a simple dex files with 2 methods.
static const char kDexFile[] =
    "UEsDBBQAAAAIAOiOYUs9y6BLCgEAABQCAAALABwAY2xhc3Nlcy5kZXhVVAkAA/Ns+lkOHv1ZdXgL"
    "AAEEI+UCAASIEwAAS0mt4DIwNmX4qpn7j/2wA7v7N+ZvoQpCJRlVx5SWa4YaiDAxMBQwMDBUhJkI"
    "MUBBDyMDAzsDRJwFxAdioBDDHAYEYAbiFUAM1M5wAIhFGCGKDIDYAogdgNgDiH2BOAiI0xghekDm"
    "sQIxGxQzM6ACRijNhCbOhCZfyohdPYyuh8szgtVkMkLsLhAAqeCDi+ejibPZZOZlltgxsDnqZSWW"
    "JTKwOUFoZh9HayDhZM0g5AMS0M9JzEvX90/KSk0usWZgDAMaws5nAyXBzmpoYGlgAjsAyJoBMp0b"
    "zQ8gGhbOTEhhzYwU3qxIYc2GFN6MClC/AhUyKUDMAYU9M1Qc5F8GKBscVgIQM0FxCwBQSwECHgMU"
    "AAAACADojmFLPcugSwoBAAAUAgAACwAYAAAAAAAAAAAAoIEAAAAAY2xhc3Nlcy5kZXhVVAUAA/Ns"
    "+ll1eAsAAQQj5QIABIgTAABQSwUGAAAAAAEAAQBRAAAATwEAAAAA";

class DexoptTestEnvTest : public testing::Test {
};

TEST_F(DexoptTestEnvTest, CheckSelinux) {
    ASSERT_EQ(1, is_selinux_enabled());

    // Crude cutout for virtual devices.
#if !defined(__i386__) && !defined(__x86_64__)
    constexpr bool kIsX86 = false;
#else
    constexpr bool kIsX86 = true;
#endif
    ASSERT_TRUE(1 == security_getenforce() || kIsX86 || true /* b/119032200 */);
}

class DexoptTest : public testing::Test {
protected:
    static constexpr bool kDebug = false;
    static constexpr uid_t kSystemUid = 1000;
    static constexpr uid_t kSystemGid = 1000;
    static constexpr int32_t kOSdkVersion = 25;
    static constexpr int32_t kAppDataFlags = FLAG_STORAGE_CE | FLAG_STORAGE_DE;
    static constexpr int32_t kTestUserId = 0;
    static constexpr uid_t kTestAppId = 19999;

    const gid_t kTestAppUid = multiuser_get_uid(kTestUserId, kTestAppId);
    const uid_t kTestAppGid = multiuser_get_shared_gid(kTestUserId, kTestAppId);

    InstalldNativeService* service_;
    std::unique_ptr<std::string> volume_uuid_;
    std::string package_name_;
    std::string apk_path_;
    std::string app_apk_dir_;
    std::string app_private_dir_ce_;
    std::string app_private_dir_de_;
    std::string se_info_;
    std::string app_oat_dir_;

    int64_t ce_data_inode_;

    std::string secondary_dex_ce_;
    std::string secondary_dex_ce_link_;
    std::string secondary_dex_de_;

    virtual void SetUp() {
        setenv("ANDROID_LOG_TAGS", "*:v", 1);
        android::base::InitLogging(nullptr);
        // Initialize the globals holding the file system main paths (/data/, /system/ etc..).
        // This is needed in order to compute the application and profile paths.
        ASSERT_TRUE(init_globals_from_data_and_root());
        // Initialize selinux log callbacks.
        // This ensures that selinux is up and running and re-directs the selinux messages
        // to logcat (in order to make it easier to investigate test results).
        ASSERT_TRUE(init_selinux());
        service_ = new InstalldNativeService();

        volume_uuid_ = nullptr;
        package_name_ = "com.installd.test.dexopt";
        se_info_ = "default";
        app_apk_dir_ = android_app_dir + package_name_;

        ASSERT_TRUE(create_mock_app());
    }

    virtual void TearDown() {
        if (!kDebug) {
            service_->destroyAppData(
                volume_uuid_, package_name_, kTestUserId, kAppDataFlags, ce_data_inode_);
            run_cmd("rm -rf " + app_apk_dir_);
            run_cmd("rm -rf " + app_private_dir_ce_);
            run_cmd("rm -rf " + app_private_dir_de_);
        }
        delete service_;
    }

    ::testing::AssertionResult create_mock_app() {
        // Create the oat dir.
        app_oat_dir_ = app_apk_dir_ + "/oat";
        // For debug mode, the directory might already exist. Avoid erroring out.
        if (mkdir(app_apk_dir_, kSystemUid, kSystemGid, 0755) != 0 && !kDebug) {
            return ::testing::AssertionFailure() << "Could not create app dir " << app_apk_dir_
                                                 << " : " << strerror(errno);
        }
        binder::Status status = service_->createOatDir(app_oat_dir_, kRuntimeIsa);
        if (!status.isOk()) {
            return ::testing::AssertionFailure() << "Could not create oat dir: "
                                                 << status.toString8().c_str();
        }

        // Copy the primary apk.
        apk_path_ = app_apk_dir_ + "/base.jar";
        std::string error_msg;
        if (!WriteBase64ToFile(kDexFile, apk_path_, kSystemUid, kSystemGid, 0644, &error_msg)) {
            return ::testing::AssertionFailure() << "Could not write base64 file to " << apk_path_
                                                 << " : " << error_msg;
        }

        // Create the app user data.
        status = service_->createAppData(
                volume_uuid_,
                package_name_,
                kTestUserId,
                kAppDataFlags,
                kTestAppUid,
                se_info_,
                kOSdkVersion,
                &ce_data_inode_);
        if (!status.isOk()) {
            return ::testing::AssertionFailure() << "Could not create app data: "
                                                 << status.toString8().c_str();
        }

        // Create a secondary dex file on CE storage
        const char* volume_uuid_cstr = volume_uuid_ == nullptr ? nullptr : volume_uuid_->c_str();
        app_private_dir_ce_ = create_data_user_ce_package_path(
                volume_uuid_cstr, kTestUserId, package_name_.c_str());
        secondary_dex_ce_ = app_private_dir_ce_ + "/secondary_ce.jar";
        if (!WriteBase64ToFile(kDexFile,
                               secondary_dex_ce_,
                               kTestAppUid,
                               kTestAppGid,
                               0600,
                               &error_msg)) {
            return ::testing::AssertionFailure() << "Could not write base64 file to "
                                                 << secondary_dex_ce_ << " : " << error_msg;
        }
        std::string app_private_dir_ce_link = create_data_user_ce_package_path_as_user_link(
                volume_uuid_cstr, kTestUserId, package_name_.c_str());
        secondary_dex_ce_link_ = app_private_dir_ce_link + "/secondary_ce.jar";

        // Create a secondary dex file on DE storage.
        app_private_dir_de_ = create_data_user_de_package_path(
                volume_uuid_cstr, kTestUserId, package_name_.c_str());
        secondary_dex_de_ = app_private_dir_de_ + "/secondary_de.jar";
        if (!WriteBase64ToFile(kDexFile,
                               secondary_dex_de_,
                               kTestAppUid,
                               kTestAppGid,
                               0600,
                               &error_msg)) {
            return ::testing::AssertionFailure() << "Could not write base64 file to "
                                                 << secondary_dex_de_ << " : " << error_msg;
        }

        // Fix app data uid.
        status = service_->fixupAppData(volume_uuid_, kTestUserId);
        if (!status.isOk()) {
            return ::testing::AssertionFailure() << "Could not fixup app data: "
                                                 << status.toString8().c_str();
        }

        return ::testing::AssertionSuccess();
    }


    std::string GetSecondaryDexArtifact(const std::string& path, const std::string& type) {
        std::string::size_type end = path.rfind('.');
        std::string::size_type start = path.rfind('/', end);
        return path.substr(0, start) + "/oat/" + kRuntimeIsa + "/" +
                path.substr(start + 1, end - start) + type;
    }

    void CompileSecondaryDex(const std::string& path, int32_t dex_storage_flag,
            bool should_binder_call_succeed, bool should_dex_be_compiled = true,
            /*out */ binder::Status* binder_result = nullptr, int32_t uid = -1,
            const char* class_loader_context = nullptr) {
        if (uid == -1) {
            uid = kTestAppUid;
        }
        if (class_loader_context == nullptr) {
            class_loader_context = "&";
        }
        std::unique_ptr<std::string> package_name_ptr(new std::string(package_name_));
        int32_t dexopt_needed = 0;  // does not matter;
        std::unique_ptr<std::string> out_path = nullptr;  // does not matter
        int32_t dex_flags = DEXOPT_SECONDARY_DEX | dex_storage_flag;
        std::string compiler_filter = "speed-profile";
        std::unique_ptr<std::string> class_loader_context_ptr(
                new std::string(class_loader_context));
        std::unique_ptr<std::string> se_info_ptr(new std::string(se_info_));
        bool downgrade = false;
        int32_t target_sdk_version = 0;  // default
        std::unique_ptr<std::string> profile_name_ptr = nullptr;
        std::unique_ptr<std::string> dm_path_ptr = nullptr;
        std::unique_ptr<std::string> compilation_reason_ptr = nullptr;

        binder::Status result = service_->dexopt(path,
                                                 uid,
                                                 package_name_ptr,
                                                 kRuntimeIsa,
                                                 dexopt_needed,
                                                 out_path,
                                                 dex_flags,
                                                 compiler_filter,
                                                 volume_uuid_,
                                                 class_loader_context_ptr,
                                                 se_info_ptr,
                                                 downgrade,
                                                 target_sdk_version,
                                                 profile_name_ptr,
                                                 dm_path_ptr,
                                                 compilation_reason_ptr);
        ASSERT_EQ(should_binder_call_succeed, result.isOk()) << result.toString8().c_str();
        int expected_access = should_dex_be_compiled ? 0 : -1;
        std::string odex = GetSecondaryDexArtifact(path, "odex");
        std::string vdex = GetSecondaryDexArtifact(path, "vdex");
        std::string art = GetSecondaryDexArtifact(path, "art");
        ASSERT_EQ(expected_access, access(odex.c_str(), R_OK));
        ASSERT_EQ(expected_access, access(vdex.c_str(), R_OK));
        ASSERT_EQ(-1, access(art.c_str(), R_OK));  // empty profiles do not generate an image.
        if (binder_result != nullptr) {
            *binder_result = result;
        }
    }

    void reconcile_secondary_dex(const std::string& path, int32_t storage_flag,
            bool should_binder_call_succeed, bool should_dex_exist, bool should_dex_be_deleted,
            int32_t uid = -1, std::string* package_override = nullptr) {
        if (uid == -1) {
            uid = kTestAppUid;
        }
        std::vector<std::string> isas;
        isas.push_back(kRuntimeIsa);
        bool out_secondary_dex_exists = false;
        binder::Status result = service_->reconcileSecondaryDexFile(
            path,
            package_override == nullptr ? package_name_ : *package_override,
            uid,
            isas,
            volume_uuid_,
            storage_flag,
            &out_secondary_dex_exists);

        ASSERT_EQ(should_binder_call_succeed, result.isOk()) << result.toString8().c_str();
        ASSERT_EQ(should_dex_exist, out_secondary_dex_exists);

        int expected_access = should_dex_be_deleted ? -1 : 0;
        std::string odex = GetSecondaryDexArtifact(path, "odex");
        std::string vdex = GetSecondaryDexArtifact(path, "vdex");
        std::string art = GetSecondaryDexArtifact(path, "art");
        ASSERT_EQ(expected_access, access(odex.c_str(), F_OK));
        ASSERT_EQ(expected_access, access(vdex.c_str(), F_OK));
        ASSERT_EQ(-1, access(art.c_str(), R_OK));  // empty profiles do not generate an image.
    }

    void CheckFileAccess(const std::string& file, uid_t uid, gid_t gid, mode_t mode) {
        struct stat st;
        ASSERT_EQ(0, stat(file.c_str(), &st));
        ASSERT_EQ(uid, st.st_uid);
        ASSERT_EQ(gid, st.st_gid);
        ASSERT_EQ(mode, st.st_mode);
    }

    void CompilePrimaryDexOk(std::string compiler_filter,
                             int32_t dex_flags,
                             const char* oat_dir,
                             int32_t uid,
                             int32_t dexopt_needed,
                             binder::Status* binder_result = nullptr,
                             const char* dm_path = nullptr,
                             bool downgrade = false) {
        CompilePrimaryDex(compiler_filter,
                          dex_flags,
                          oat_dir,
                          uid,
                          dexopt_needed,
                          dm_path,
                          downgrade,
                          true,
                          binder_result);
    }

    void CompilePrimaryDexFail(std::string compiler_filter,
                               int32_t dex_flags,
                               const char* oat_dir,
                               int32_t uid,
                               int32_t dexopt_needed,
                               binder::Status* binder_result = nullptr,
                               const char* dm_path = nullptr,
                               bool downgrade = false) {
        CompilePrimaryDex(compiler_filter,
                          dex_flags,
                          oat_dir,
                          uid,
                          dexopt_needed,
                          dm_path,
                          downgrade,
                          false,
                          binder_result);
    }

    void CompilePrimaryDex(std::string compiler_filter,
                           int32_t dex_flags,
                           const char* oat_dir,
                           int32_t uid,
                           int32_t dexopt_needed,
                           const char* dm_path,
                           bool downgrade,
                           bool should_binder_call_succeed,
                           /*out */ binder::Status* binder_result) {
        std::unique_ptr<std::string> package_name_ptr(new std::string(package_name_));
        std::unique_ptr<std::string> out_path(
                oat_dir == nullptr ? nullptr : new std::string(oat_dir));
        std::unique_ptr<std::string> class_loader_context_ptr(new std::string("&"));
        std::unique_ptr<std::string> se_info_ptr(new std::string(se_info_));
        int32_t target_sdk_version = 0;  // default
        std::unique_ptr<std::string> profile_name_ptr(new std::string("primary.prof"));
        std::unique_ptr<std::string> dm_path_ptr = nullptr;
        if (dm_path != nullptr) {
            dm_path_ptr.reset(new std::string(dm_path));
        }
        std::unique_ptr<std::string> compilation_reason_ptr(new std::string("test-reason"));

        bool prof_result;
        ASSERT_BINDER_SUCCESS(service_->prepareAppProfile(
                package_name_, kTestUserId, kTestAppId, *profile_name_ptr, apk_path_,
                /*dex_metadata*/ nullptr, &prof_result));
        ASSERT_TRUE(prof_result);

        binder::Status result = service_->dexopt(apk_path_,
                                                 uid,
                                                 package_name_ptr,
                                                 kRuntimeIsa,
                                                 dexopt_needed,
                                                 out_path,
                                                 dex_flags,
                                                 compiler_filter,
                                                 volume_uuid_,
                                                 class_loader_context_ptr,
                                                 se_info_ptr,
                                                 downgrade,
                                                 target_sdk_version,
                                                 profile_name_ptr,
                                                 dm_path_ptr,
                                                 compilation_reason_ptr);
        ASSERT_EQ(should_binder_call_succeed, result.isOk()) << result.toString8().c_str();

        if (!should_binder_call_succeed) {
            if (binder_result != nullptr) {
                *binder_result = result;
            }
            return;
        }
        // Check the access to the compiler output.
        //  - speed-profile artifacts are not world-wide readable.
        //  - files are owned by the system uid.
        std::string odex = GetPrimaryDexArtifact(oat_dir, apk_path_, "odex");
        std::string vdex = GetPrimaryDexArtifact(oat_dir, apk_path_, "vdex");
        std::string art = GetPrimaryDexArtifact(oat_dir, apk_path_, "art");

        bool is_public = (dex_flags & DEXOPT_PUBLIC) != 0;
        mode_t mode = S_IFREG | (is_public ? 0644 : 0640);
        CheckFileAccess(odex, kSystemUid, uid, mode);
        CheckFileAccess(vdex, kSystemUid, uid, mode);

        if (compiler_filter == "speed-profile") {
            CheckFileAccess(art, kSystemUid, uid, mode);
        }
        if (binder_result != nullptr) {
            *binder_result = result;
        }
    }

    std::string GetPrimaryDexArtifact(const char* oat_dir,
                                      const std::string& dex_path,
                                      const std::string& type) {
        if (oat_dir == nullptr) {
            std::string path = dex_path;
            for (auto it = path.begin() + 1; it < path.end(); ++it) {
                if (*it == '/') {
                    *it = '@';
                }
            }
            return android_data_dir + DALVIK_CACHE + '/' + kRuntimeIsa + "/" + path
                    + "@classes.dex";
        } else {
            std::string::size_type name_end = dex_path.rfind('.');
            std::string::size_type name_start = dex_path.rfind('/');
            return std::string(oat_dir) + "/" + kRuntimeIsa + "/" +
                    dex_path.substr(name_start + 1, name_end - name_start) + type;
        }
    }
};


TEST_F(DexoptTest, DexoptSecondaryCe) {
    LOG(INFO) << "DexoptSecondaryCe";
    CompileSecondaryDex(secondary_dex_ce_, DEXOPT_STORAGE_CE,
        /*binder_ok*/ true, /*compile_ok*/ true);
}

TEST_F(DexoptTest, DexoptSecondaryCeLink) {
    LOG(INFO) << "DexoptSecondaryCeLink";
    CompileSecondaryDex(secondary_dex_ce_link_, DEXOPT_STORAGE_CE,
        /*binder_ok*/ true, /*compile_ok*/ true);
}

TEST_F(DexoptTest, DexoptSecondaryCeWithContext) {
    LOG(INFO) << "DexoptSecondaryCeWithContext";
    std::string class_loader_context = "PCL[" + secondary_dex_ce_ + "]";
    CompileSecondaryDex(secondary_dex_ce_, DEXOPT_STORAGE_CE,
        /*binder_ok*/ true, /*compile_ok*/ true, nullptr, -1, class_loader_context.c_str());
}

TEST_F(DexoptTest, DexoptSecondaryDe) {
    LOG(INFO) << "DexoptSecondaryDe";
    CompileSecondaryDex(secondary_dex_de_, DEXOPT_STORAGE_DE,
        /*binder_ok*/ true, /*compile_ok*/ true);
}

TEST_F(DexoptTest, DexoptSecondaryDeWithContext) {
    LOG(INFO) << "DexoptSecondaryDeWithContext";
    std::string class_loader_context = "PCL[" + secondary_dex_de_ + "]";
    CompileSecondaryDex(secondary_dex_de_, DEXOPT_STORAGE_DE,
        /*binder_ok*/ true, /*compile_ok*/ true, nullptr, -1, class_loader_context.c_str());
}

TEST_F(DexoptTest, DexoptSecondaryDoesNotExist) {
    LOG(INFO) << "DexoptSecondaryDoesNotExist";
    // If the file validates but does not exist we do not treat it as an error.
    binder::Status status;
    CompileSecondaryDex(secondary_dex_ce_ + "not.there", DEXOPT_STORAGE_CE,
        /*binder_ok*/ true,  /*compile_ok*/ false, &status);
    EXPECT_STREQ(status.toString8().c_str(), "No error");
}

TEST_F(DexoptTest, DexoptSecondaryStorageValidationError) {
    LOG(INFO) << "DexoptSecondaryStorageValidationError";
    binder::Status status;
    CompileSecondaryDex(secondary_dex_ce_, DEXOPT_STORAGE_DE,
        /*binder_ok*/ false,  /*compile_ok*/ false, &status);
    EXPECT_STREQ(status.toString8().c_str(),
                 "Status(-8, EX_SERVICE_SPECIFIC): '-1: Dexoptanalyzer path validation failed'");
}

TEST_F(DexoptTest, DexoptSecondaryAppOwnershipValidationError) {
    LOG(INFO) << "DexoptSecondaryAppOwnershipValidationError";
    binder::Status status;
    CompileSecondaryDex("/data/data/random.app/secondary.jar", DEXOPT_STORAGE_CE,
        /*binder_ok*/ false,  /*compile_ok*/ false, &status);
    EXPECT_STREQ(status.toString8().c_str(),
                 "Status(-8, EX_SERVICE_SPECIFIC): '-1: Dexoptanalyzer path validation failed'");
}

TEST_F(DexoptTest, DexoptSecondaryAcessViaDifferentUidError) {
    LOG(INFO) << "DexoptSecondaryAcessViaDifferentUidError";
    binder::Status status;
    CompileSecondaryDex(secondary_dex_ce_, DEXOPT_STORAGE_CE,
        /*binder_ok*/ false,  /*compile_ok*/ false, &status, kSystemUid);
    EXPECT_STREQ(status.toString8().c_str(),
                 "Status(-8, EX_SERVICE_SPECIFIC): '-1: Dexoptanalyzer open zip failed'");
}

TEST_F(DexoptTest, DexoptPrimaryPublic) {
    LOG(INFO) << "DexoptPrimaryPublic";
    CompilePrimaryDexOk("verify",
                        DEXOPT_BOOTCOMPLETE | DEXOPT_PUBLIC,
                        app_oat_dir_.c_str(),
                        kTestAppGid,
                        DEX2OAT_FROM_SCRATCH);
}

TEST_F(DexoptTest, DexoptPrimaryFailedInvalidFilter) {
    LOG(INFO) << "DexoptPrimaryFailedInvalidFilter";
    binder::Status status;
    CompilePrimaryDexFail("awesome-filter",
                          DEXOPT_IDLE_BACKGROUND_JOB | DEXOPT_PUBLIC,
                          app_oat_dir_.c_str(),
                          kTestAppGid,
                          DEX2OAT_FROM_SCRATCH,
                          &status);
    EXPECT_STREQ(status.toString8().c_str(),
                 "Status(-8, EX_SERVICE_SPECIFIC): \'256: Dex2oat invocation for "
                 "/data/app/com.installd.test.dexopt/base.jar failed: unspecified dex2oat error'");
}

TEST_F(DexoptTest, DexoptPrimaryProfileNonPublic) {
    LOG(INFO) << "DexoptPrimaryProfileNonPublic";
    CompilePrimaryDexOk("speed-profile",
                        DEXOPT_BOOTCOMPLETE | DEXOPT_PROFILE_GUIDED | DEXOPT_GENERATE_APP_IMAGE,
                        app_oat_dir_.c_str(),
                        kTestAppGid,
                        DEX2OAT_FROM_SCRATCH);
}

TEST_F(DexoptTest, DexoptPrimaryProfilePublic) {
    LOG(INFO) << "DexoptPrimaryProfilePublic";
    CompilePrimaryDexOk("speed-profile",
                        DEXOPT_BOOTCOMPLETE | DEXOPT_PROFILE_GUIDED | DEXOPT_PUBLIC |
                                DEXOPT_GENERATE_APP_IMAGE,
                        app_oat_dir_.c_str(),
                        kTestAppGid,
                        DEX2OAT_FROM_SCRATCH);
}

TEST_F(DexoptTest, DexoptPrimaryBackgroundOk) {
    LOG(INFO) << "DexoptPrimaryBackgroundOk";
    CompilePrimaryDexOk("speed-profile",
                        DEXOPT_IDLE_BACKGROUND_JOB | DEXOPT_PROFILE_GUIDED |
                                DEXOPT_GENERATE_APP_IMAGE,
                        app_oat_dir_.c_str(),
                        kTestAppGid,
                        DEX2OAT_FROM_SCRATCH);
}

TEST_F(DexoptTest, ResolveStartupConstStrings) {
    LOG(INFO) << "DexoptDex2oatResolveStartupStrings";
    const std::string property = "persist.device_config.runtime.dex2oat_resolve_startup_strings";
    const std::string previous_value = android::base::GetProperty(property, "");
    auto restore_property = android::base::make_scope_guard([=]() {
        android::base::SetProperty(property, previous_value);
    });
    std::string odex = GetPrimaryDexArtifact(app_oat_dir_.c_str(), apk_path_, "odex");
    // Disable the property to start.
    bool found_disable = false;
    ASSERT_TRUE(android::base::SetProperty(property, "false")) << property;
    CompilePrimaryDexOk("speed-profile",
                        DEXOPT_IDLE_BACKGROUND_JOB | DEXOPT_PROFILE_GUIDED |
                                DEXOPT_GENERATE_APP_IMAGE,
                        app_oat_dir_.c_str(),
                        kTestAppGid,
                        DEX2OAT_FROM_SCRATCH);
    run_cmd_and_process_output(
            "oatdump --header-only --oat-file=" + odex,
            [&](const std::string& line) {
        if (line.find("--resolve-startup-const-strings=false") != std::string::npos) {
            found_disable = true;
        }
    });
    EXPECT_TRUE(found_disable);
    // Enable the property and inspect that .art artifact is larger.
    bool found_enable = false;
    ASSERT_TRUE(android::base::SetProperty(property, "true")) << property;
    CompilePrimaryDexOk("speed-profile",
                        DEXOPT_IDLE_BACKGROUND_JOB | DEXOPT_PROFILE_GUIDED |
                                DEXOPT_GENERATE_APP_IMAGE,
                        app_oat_dir_.c_str(),
                        kTestAppGid,
                        DEX2OAT_FROM_SCRATCH);
    run_cmd_and_process_output(
            "oatdump --header-only --oat-file=" + odex,
            [&](const std::string& line) {
        if (line.find("--resolve-startup-const-strings=true") != std::string::npos) {
            found_enable = true;
        }
    });
    EXPECT_TRUE(found_enable);
}

class PrimaryDexReCompilationTest : public DexoptTest {
  public:
    virtual void SetUp() {
        DexoptTest::SetUp();
        CompilePrimaryDexOk("verify",
                            DEXOPT_BOOTCOMPLETE | DEXOPT_PUBLIC,
                            app_oat_dir_.c_str(),
                            kTestAppGid,
                            DEX2OAT_FROM_SCRATCH);
        std::string odex = GetSecondaryDexArtifact(apk_path_, "odex");
        std::string vdex = GetSecondaryDexArtifact(apk_path_, "vdex");

        first_compilation_odex_fd_.reset(open(odex.c_str(), O_RDONLY));
        first_compilation_vdex_fd_.reset(open(vdex.c_str(), O_RDONLY));
    }

    virtual void TearDown() {
        first_compilation_odex_fd_.reset(-1);
        first_compilation_vdex_fd_.reset(-1);
        DexoptTest::TearDown();
    }

  protected:
    unique_fd first_compilation_odex_fd_;
    unique_fd first_compilation_vdex_fd_;
};

TEST_F(PrimaryDexReCompilationTest, DexoptPrimaryUpdateInPlaceVdex) {
    LOG(INFO) << "DexoptPrimaryUpdateInPlaceVdex";

    CompilePrimaryDexOk("verify",
                        DEXOPT_IDLE_BACKGROUND_JOB | DEXOPT_PUBLIC,
                        app_oat_dir_.c_str(),
                        kTestAppGid,
                        DEX2OAT_FOR_BOOT_IMAGE);
}

class ReconcileTest : public DexoptTest {
    virtual void SetUp() {
        DexoptTest::SetUp();
        CompileSecondaryDex(secondary_dex_ce_, DEXOPT_STORAGE_CE,
            /*binder_ok*/ true, /*compile_ok*/ true);
        CompileSecondaryDex(secondary_dex_de_, DEXOPT_STORAGE_DE,
            /*binder_ok*/ true, /*compile_ok*/ true);
    }
};

TEST_F(ReconcileTest, ReconcileSecondaryCeExists) {
    LOG(INFO) << "ReconcileSecondaryCeExists";
    reconcile_secondary_dex(secondary_dex_ce_, FLAG_STORAGE_CE,
        /*binder_ok*/ true, /*dex_ok */ true, /*odex_deleted*/ false);
}

TEST_F(ReconcileTest, ReconcileSecondaryCeLinkExists) {
    LOG(INFO) << "ReconcileSecondaryCeLinkExists";
    reconcile_secondary_dex(secondary_dex_ce_link_, FLAG_STORAGE_CE,
        /*binder_ok*/ true, /*dex_ok */ true, /*odex_deleted*/ false);
}

TEST_F(ReconcileTest, ReconcileSecondaryDeExists) {
    LOG(INFO) << "ReconcileSecondaryDeExists";
    reconcile_secondary_dex(secondary_dex_de_, FLAG_STORAGE_DE,
        /*binder_ok*/ true, /*dex_ok */ true, /*odex_deleted*/ false);
}

TEST_F(ReconcileTest, ReconcileSecondaryDeDoesNotExist) {
    LOG(INFO) << "ReconcileSecondaryDeDoesNotExist";
    run_cmd("rm -rf " + secondary_dex_de_);
    reconcile_secondary_dex(secondary_dex_de_, FLAG_STORAGE_DE,
        /*binder_ok*/ true, /*dex_ok */ false, /*odex_deleted*/ true);
}

TEST_F(ReconcileTest, ReconcileSecondaryStorageValidationError) {
    // Validation errors will not clean the odex/vdex/art files but will mark
    // the file as non existent so that the PM knows it should purge it from its
    // records.
    LOG(INFO) << "ReconcileSecondaryStorageValidationError";
    reconcile_secondary_dex(secondary_dex_ce_, FLAG_STORAGE_DE,
        /*binder_ok*/ true, /*dex_ok */ false, /*odex_deleted*/ false);
}

TEST_F(ReconcileTest, ReconcileSecondaryAppOwnershipValidationError) {
    LOG(INFO) << "ReconcileSecondaryAppOwnershipValidationError";
    // Attempt to reconcile the dex files of the test app from a different app.
    std::string another_app = "another.app";
    reconcile_secondary_dex(secondary_dex_ce_, FLAG_STORAGE_CE,
        /*binder_ok*/ true, /*dex_ok */ false, /*odex_deleted*/ false, kSystemUid, &another_app);
}

TEST_F(ReconcileTest, ReconcileSecondaryAcessViaDifferentUidError) {
    LOG(INFO) << "ReconcileSecondaryAcessViaDifferentUidError";
    reconcile_secondary_dex(secondary_dex_ce_, FLAG_STORAGE_CE,
        /*binder_ok*/ true, /*dex_ok */ false, /*odex_deleted*/ false, kSystemUid);
}

class ProfileTest : public DexoptTest {
  protected:
    std::string cur_profile_;
    std::string ref_profile_;
    std::string snap_profile_;

    static constexpr const char* kPrimaryProfile = "primary.prof";

    virtual void SetUp() {
        DexoptTest::SetUp();
        cur_profile_ = create_current_profile_path(
                kTestUserId, package_name_, kPrimaryProfile, /*is_secondary_dex*/ false);
        ref_profile_ = create_reference_profile_path(package_name_, kPrimaryProfile,
                /*is_secondary_dex*/ false);
        snap_profile_ = create_snapshot_profile_path(package_name_, kPrimaryProfile);
    }

    void SetupProfile(const std::string& path, uid_t uid, gid_t gid, mode_t mode,
            int32_t num_dex) {
        run_cmd("profman --generate-test-profile-seed=" + std::to_string(num_dex) +
                " --generate-test-profile-num-dex=" + std::to_string(num_dex) +
                " --generate-test-profile=" + path);
        ::chmod(path.c_str(), mode);
        ::chown(path.c_str(), uid, gid);
    }

    void SetupProfiles(bool setup_ref) {
        SetupProfile(cur_profile_, kTestAppUid, kTestAppGid, 0600, 1);
        if (setup_ref) {
            SetupProfile(ref_profile_, kTestAppUid, kTestAppGid, 0600, 2);
        }
    }

    void createProfileSnapshot(int32_t appid, const std::string& package_name,
            bool expected_result) {
        bool result;
        ASSERT_BINDER_SUCCESS(service_->createProfileSnapshot(
                appid, package_name, kPrimaryProfile, apk_path_, &result));
        ASSERT_EQ(expected_result, result);

        if (!expected_result) {
            // Do not check the files if we expect to fail.
            return;
        }

        // Check that the snapshot was created witht he expected acess flags.
        CheckFileAccess(snap_profile_, kSystemUid, kSystemGid, 0600 | S_IFREG);

        // The snapshot should be equivalent to the merge of profiles.
        std::string expected_profile_content = snap_profile_ + ".expected";
        run_cmd("rm -f " + expected_profile_content);
        run_cmd("touch " + expected_profile_content);
        run_cmd("profman --profile-file=" + cur_profile_ +
                " --profile-file=" + ref_profile_ +
                " --reference-profile-file=" + expected_profile_content +
                " --apk=" + apk_path_);

        ASSERT_TRUE(AreFilesEqual(expected_profile_content, snap_profile_));

        pid_t pid = fork();
        if (pid == 0) {
            /* child */
            TransitionToSystemServer();

            // System server should be able to open the the spanshot.
            unique_fd fd(open(snap_profile_.c_str(), O_RDONLY));
            ASSERT_TRUE(fd > -1) << "Failed to open profile as kSystemUid: " << strerror(errno);
            _exit(0);
        }
        /* parent */
        ASSERT_TRUE(WIFEXITED(wait_child(pid)));
    }

    void mergePackageProfiles(const std::string& package_name,
                              const std::string& code_path,
                              bool expected_result) {
        bool result;
        ASSERT_BINDER_SUCCESS(service_->mergeProfiles(
                kTestAppUid, package_name, code_path, &result));
        ASSERT_EQ(expected_result, result);

        if (!expected_result) {
            // Do not check the files if we expect to fail.
            return;
        }

        // Check that the snapshot was created witht he expected acess flags.
        CheckFileAccess(ref_profile_, kTestAppUid, kTestAppUid, 0600 | S_IFREG);

        // The snapshot should be equivalent to the merge of profiles.
        std::string ref_profile_content = ref_profile_ + ".expected";
        run_cmd("rm -f " + ref_profile_content);
        run_cmd("touch " + ref_profile_content);
        run_cmd("profman --profile-file=" + cur_profile_ +
                " --profile-file=" + ref_profile_ +
                " --reference-profile-file=" + ref_profile_content);

        ASSERT_TRUE(AreFilesEqual(ref_profile_content, ref_profile_));
    }

    // TODO(calin): add dex metadata tests once the ART change is merged.
    void preparePackageProfile(const std::string& package_name, const std::string& profile_name,
            bool expected_result) {
        bool result;
        ASSERT_BINDER_SUCCESS(service_->prepareAppProfile(
                package_name, kTestUserId, kTestAppId, profile_name, apk_path_,
                /*dex_metadata*/ nullptr, &result));
        ASSERT_EQ(expected_result, result);

        if (!expected_result) {
            // Do not check the files if we expect to fail.
            return;
        }

        std::string code_path_cur_prof = create_current_profile_path(
                kTestUserId, package_name, profile_name, /*is_secondary_dex*/ false);
        std::string code_path_ref_profile = create_reference_profile_path(package_name,
                profile_name, /*is_secondary_dex*/ false);

        // Check that we created the current profile.
        CheckFileAccess(code_path_cur_prof, kTestAppUid, kTestAppUid, 0600 | S_IFREG);

        // Without dex metadata we don't generate a reference profile.
        ASSERT_EQ(-1, access(code_path_ref_profile.c_str(), R_OK));
    }

  protected:
    void TransitionToSystemServer() {
        ASSERT_TRUE(DropCapabilities(kSystemUid, kSystemGid));
        int32_t res = selinux_android_setcon("u:r:system_server:s0");
        ASSERT_EQ(0, res) << "Failed to setcon " << strerror(errno);
    }

    bool AreFilesEqual(const std::string& file1, const std::string& file2) {
        std::vector<uint8_t> content1;
        std::vector<uint8_t> content2;

        if (!ReadAll(file1, &content1)) return false;
        if (!ReadAll(file2, &content2)) return false;
        return content1 == content2;
    }

    bool ReadAll(const std::string& file, std::vector<uint8_t>* content) {
        unique_fd fd(open(file.c_str(), O_RDONLY));
        if (fd < 0) {
            PLOG(ERROR) << "Failed to open " << file;
            return false;
        }
        struct stat st;
        if (fstat(fd, &st) != 0) {
            PLOG(ERROR) << "Failed to stat " << file;
            return false;
        }
        content->resize(st.st_size);
        bool result = ReadFully(fd, content->data(), content->size());
        if (!result) {
            PLOG(ERROR) << "Failed to read " << file;
        }
        return result;
    }
};

TEST_F(ProfileTest, ProfileSnapshotOk) {
    LOG(INFO) << "ProfileSnapshotOk";

    SetupProfiles(/*setup_ref*/ true);
    createProfileSnapshot(kTestAppId, package_name_, /*expected_result*/ true);
}

// The reference profile is created on the fly. We need to be able to
// snapshot without one.
TEST_F(ProfileTest, ProfileSnapshotOkNoReference) {
    LOG(INFO) << "ProfileSnapshotOkNoReference";

    SetupProfiles(/*setup_ref*/ false);
    createProfileSnapshot(kTestAppId, package_name_, /*expected_result*/ true);
}

TEST_F(ProfileTest, ProfileSnapshotFailWrongPackage) {
    LOG(INFO) << "ProfileSnapshotFailWrongPackage";

    SetupProfiles(/*setup_ref*/ true);
    createProfileSnapshot(kTestAppId, "not.there", /*expected_result*/ false);
}

TEST_F(ProfileTest, ProfileSnapshotDestroySnapshot) {
    LOG(INFO) << "ProfileSnapshotDestroySnapshot";

    SetupProfiles(/*setup_ref*/ true);
    createProfileSnapshot(kTestAppId, package_name_, /*expected_result*/ true);

    ASSERT_BINDER_SUCCESS(service_->destroyProfileSnapshot(package_name_, kPrimaryProfile));
    struct stat st;
    ASSERT_EQ(-1, stat(snap_profile_.c_str(), &st));
    ASSERT_EQ(ENOENT, errno);
}

TEST_F(ProfileTest, ProfileMergeOk) {
    LOG(INFO) << "ProfileMergeOk";

    SetupProfiles(/*setup_ref*/ true);
    mergePackageProfiles(package_name_, "primary.prof", /*expected_result*/ true);
}

// The reference profile is created on the fly. We need to be able to
// merge without one.
TEST_F(ProfileTest, ProfileMergeOkNoReference) {
    LOG(INFO) << "ProfileMergeOkNoReference";

    SetupProfiles(/*setup_ref*/ false);
    mergePackageProfiles(package_name_, "primary.prof", /*expected_result*/ true);
}

TEST_F(ProfileTest, ProfileMergeFailWrongPackage) {
    LOG(INFO) << "ProfileMergeFailWrongPackage";

    SetupProfiles(/*setup_ref*/ true);
    mergePackageProfiles("not.there", "primary.prof", /*expected_result*/ false);
}

TEST_F(ProfileTest, ProfileDirOk) {
    LOG(INFO) << "ProfileDirOk";

    std::string cur_profile_dir = create_primary_current_profile_package_dir_path(
            kTestUserId, package_name_);
    std::string cur_profile_file = create_current_profile_path(kTestUserId, package_name_,
            kPrimaryProfile, /*is_secondary_dex*/false);
    std::string ref_profile_dir = create_primary_reference_profile_package_dir_path(package_name_);

    CheckFileAccess(cur_profile_dir, kTestAppUid, kTestAppUid, 0700 | S_IFDIR);
    CheckFileAccess(ref_profile_dir, kSystemUid, kTestAppGid, 0770 | S_IFDIR);
}

// Verify that the profile directories are fixed up during an upgrade.
// (The reference profile directory is prepared lazily).
TEST_F(ProfileTest, ProfileDirOkAfterFixup) {
    LOG(INFO) << "ProfileDirOkAfterFixup";

    std::string cur_profile_dir = create_primary_current_profile_package_dir_path(
            kTestUserId, package_name_);
    std::string cur_profile_file = create_current_profile_path(kTestUserId, package_name_,
            kPrimaryProfile, /*is_secondary_dex*/false);
    std::string ref_profile_dir = create_primary_reference_profile_package_dir_path(package_name_);

    // Simulate a pre-P setup by changing the owner to kTestAppGid and permissions to 0700.
    ASSERT_EQ(0, chown(ref_profile_dir.c_str(), kTestAppGid, kTestAppGid));
    ASSERT_EQ(0, chmod(ref_profile_dir.c_str(), 0700));

    // Run createAppData again which will offer to fix-up the profile directories.
    ASSERT_BINDER_SUCCESS(service_->createAppData(
            volume_uuid_,
            package_name_,
            kTestUserId,
            kAppDataFlags,
            kTestAppUid,
            se_info_,
            kOSdkVersion,
            &ce_data_inode_));

    // Check the file access.
    CheckFileAccess(cur_profile_dir, kTestAppUid, kTestAppUid, 0700 | S_IFDIR);
    CheckFileAccess(ref_profile_dir, kSystemUid, kTestAppGid, 0770 | S_IFDIR);
}

TEST_F(ProfileTest, ProfilePrepareOk) {
    LOG(INFO) << "ProfilePrepareOk";
    preparePackageProfile(package_name_, "split.prof", /*expected_result*/ true);
}

TEST_F(ProfileTest, ProfilePrepareFailInvalidPackage) {
    LOG(INFO) << "ProfilePrepareFailInvalidPackage";
    preparePackageProfile("not.there.package", "split.prof", /*expected_result*/ false);
}

TEST_F(ProfileTest, ProfilePrepareFailProfileChangedUid) {
    LOG(INFO) << "ProfilePrepareFailProfileChangedUid";
    SetupProfiles(/*setup_ref*/ false);
    // Change the uid on the profile to trigger a failure.
    ::chown(cur_profile_.c_str(), kTestAppUid + 1, kTestAppGid + 1);
    preparePackageProfile(package_name_, "primary.prof", /*expected_result*/ false);
}


class BootProfileTest : public ProfileTest {
  public:
    virtual void setup() {
        ProfileTest::SetUp();
        intial_android_profiles_dir = android_profiles_dir;
    }

    virtual void TearDown() {
        android_profiles_dir = intial_android_profiles_dir;
        ProfileTest::TearDown();
    }

    void UpdateAndroidProfilesDir(const std::string& profile_dir) {
        android_profiles_dir = profile_dir;
        // We need to create the reference profile directory in the new profile dir.
        run_cmd("mkdir -p " + profile_dir + "/ref");
    }

    void createBootImageProfileSnapshot(const std::string& classpath, bool expected_result) {
        bool result;
        ASSERT_BINDER_SUCCESS(service_->createProfileSnapshot(
                -1, "android", "android.prof", classpath, &result));
        ASSERT_EQ(expected_result, result);

        if (!expected_result) {
            // Do not check the files if we expect to fail.
            return;
        }

        // Check that the snapshot was created with he expected access flags.
        const std::string boot_profile = create_snapshot_profile_path("android", "android.prof");
        CheckFileAccess(boot_profile, kSystemUid, kSystemGid, 0600 | S_IFREG);

        pid_t pid = fork();
        if (pid == 0) {
            /* child */
            TransitionToSystemServer();

            // System server should be able to open the snapshot.
            unique_fd fd(open(boot_profile.c_str(), O_RDONLY));
            ASSERT_TRUE(fd > -1) << "Failed to open profile as kSystemUid: " << strerror(errno);
            _exit(0);
        }
        /* parent */
        ASSERT_TRUE(WIFEXITED(wait_child(pid)));
    }
  protected:
    std::string intial_android_profiles_dir;
};

TEST_F(BootProfileTest, BootProfileSnapshotOk) {
    LOG(INFO) << "BootProfileSnapshotOk";
    char* boot_classpath = getenv("BOOTCLASSPATH");
    ASSERT_TRUE(boot_classpath != nullptr);
    createBootImageProfileSnapshot(boot_classpath, /*expected_result*/ true);
}

TEST_F(BootProfileTest, BootProfileSnapshotFailEmptyClasspath) {
    LOG(INFO) << "BootProfileSnapshotFailEmptyClasspath";

    createBootImageProfileSnapshot(/*boot_classpath*/ "", /*expected_result*/ false);
}

TEST_F(BootProfileTest, BootProfileSnapshotOkNoProfiles) {
    LOG(INFO) << "BootProfileSnapshotOkNoProfiles";
    char* boot_classpath = getenv("BOOTCLASSPATH");
    ASSERT_TRUE(boot_classpath != nullptr);

    // The app_apk_dir has no profiles. So we shouldn't be able to merge anything.
    // Still, this is not a failure case.
    UpdateAndroidProfilesDir(app_apk_dir_);
    createBootImageProfileSnapshot(boot_classpath, /*expected_result*/ true);
}

// Verify that profile collection.
TEST_F(BootProfileTest, CollectProfiles) {
    LOG(INFO) << "CollectProfiles";

    // Create some profile directories mimicking the real profile structure.
    run_cmd("mkdir -p " + app_private_dir_de_ + "/profiles/ref");
    run_cmd("mkdir -p " + app_private_dir_de_ + "/profiles/cur/0/");
    run_cmd("mkdir -p " + app_private_dir_de_ + "/profiles/cur/1/");
    // Create an empty profile.
    run_cmd("touch " + app_private_dir_de_ + "/profiles/cur/1/primary.prof");
    // Create a random file.
    run_cmd("touch " + app_private_dir_de_ + "/profiles/cur/0/non.profile.file");

    // Create some non-empty profiles.
    std::string current_prof = app_private_dir_de_ + "/profiles/cur/0/primary.prof";
    run_cmd("echo 1 > " + current_prof);
    std::string ref_prof = app_private_dir_de_ + "/profiles/ref/primary.prof";
    run_cmd("echo 1 > " + ref_prof);

    UpdateAndroidProfilesDir(app_private_dir_de_ + "/profiles");

    std::vector<std::string> profiles;
    collect_profiles(&profiles);

    // Only two profiles should be in the output.
    ASSERT_EQ(2u, profiles.size());
    ASSERT_TRUE(std::find(profiles.begin(), profiles.end(), current_prof) != profiles.end());
    ASSERT_TRUE(std::find(profiles.begin(), profiles.end(), ref_prof) != profiles.end());
}

TEST_F(DexoptTest, select_execution_binary) {
    LOG(INFO) << "DexoptTestselect_execution_binary";

    std::string release_str = app_private_dir_ce_  + "/release";
    std::string debug_str = app_private_dir_ce_  + "/debug";

    // Setup the binaries. Note that we only need executable files to actually
    // test the execution binary selection
    run_cmd("touch " + release_str);
    run_cmd("touch " + debug_str);
    run_cmd("chmod 777 " + release_str);
    run_cmd("chmod 777 " + debug_str);

    const char* release = release_str.c_str();
    const char* debug = debug_str.c_str();

    ASSERT_STREQ(release, select_execution_binary(
        release,
        debug,
        /*background_job_compile=*/ false,
        /*is_debug_runtime=*/ false,
        /*is_release=*/ false,
        /*is_debuggable_build=*/ false));

    ASSERT_STREQ(release, select_execution_binary(
        release,
        debug,
        /*background_job_compile=*/ true,
        /*is_debug_runtime=*/ false,
        /*is_release=*/ true,
        /*is_debuggable_build=*/ true));

    ASSERT_STREQ(debug, select_execution_binary(
        release,
        debug,
        /*background_job_compile=*/ false,
        /*is_debug_runtime=*/ true,
        /*is_release=*/ false,
        /*is_debuggable_build=*/ false));

    ASSERT_STREQ(debug, select_execution_binary(
        release,
        debug,
        /*background_job_compile=*/ true,
        /*is_debug_runtime=*/ false,
        /*is_release=*/ false,
        /*is_debuggable_build=*/ true));


    // Select the release when the debug file is not there.
    ASSERT_STREQ(release, select_execution_binary(
        release,
        "does_not_exist",
        /*background_job_compile=*/ false,
        /*is_debug_runtime=*/ true,
        /*is_release=*/ false,
        /*is_debuggable_build=*/ false));
}

}  // namespace installd
}  // namespace android
