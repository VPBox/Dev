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

#include <stdio.h>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <functional>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include <grp.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/macros.h>
#include <android-base/properties.h>
#include <android-base/scopeguard.h>
#include <android-base/stringprintf.h>
#include <android-base/strings.h>
#include <android/os/IVold.h>
#include <binder/IServiceManager.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <libdm/dm.h>
#include <selinux/selinux.h>

#include <android/apex/ApexInfo.h>
#include <android/apex/IApexService.h>

#include "apex_constants.h"
#include "apex_file.h"
#include "apex_manifest.h"
#include "apexd_private.h"
#include "apexd_session.h"
#include "apexd_test_utils.h"
#include "apexd_utils.h"
#include "status_or.h"

#include "session_state.pb.h"

using apex::proto::SessionState;

namespace android {
namespace apex {

using android::sp;
using android::String16;
using android::apex::testing::ApexInfoEq;
using android::apex::testing::CreateSessionInfo;
using android::apex::testing::IsOk;
using android::apex::testing::SessionInfoEq;
using android::base::Join;
using android::base::StringPrintf;
using ::testing::Contains;
using ::testing::EndsWith;
using ::testing::HasSubstr;
using ::testing::Not;
using ::testing::UnorderedElementsAre;
using ::testing::UnorderedElementsAreArray;

namespace fs = std::filesystem;

class ApexServiceTest : public ::testing::Test {
 public:
  ApexServiceTest() {
    using android::IBinder;
    using android::IServiceManager;

    sp<IServiceManager> sm = android::defaultServiceManager();
    sp<IBinder> binder = sm->getService(String16("apexservice"));
    if (binder != nullptr) {
      service_ = android::interface_cast<IApexService>(binder);
    }
    binder = sm->getService(String16("vold"));
    if (binder != nullptr) {
      vold_service_ = android::interface_cast<android::os::IVold>(binder);
    }
  }

 protected:
  void SetUp() override {
    ASSERT_NE(nullptr, service_.get());
    ASSERT_NE(nullptr, vold_service_.get());
    android::binder::Status status =
        vold_service_->supportsCheckpoint(&supports_fs_checkpointing_);
    ASSERT_TRUE(IsOk(status));
    CleanUp();
  }

  void TearDown() override { CleanUp(); }

  static std::string GetTestDataDir() {
    return android::base::GetExecutableDirectory();
  }
  static std::string GetTestFile(const std::string& name) {
    return GetTestDataDir() + "/" + name;
  }

  static bool HaveSelinux() { return 1 == is_selinux_enabled(); }

  static bool IsSelinuxEnforced() { return 0 != security_getenforce(); }

  StatusOr<bool> IsActive(const std::string& name, int64_t version) {
    std::vector<ApexInfo> list;
    android::binder::Status status = service_->getActivePackages(&list);
    if (status.isOk()) {
      for (const ApexInfo& p : list) {
        if (p.packageName == name && p.versionCode == version) {
          return StatusOr<bool>(true);
        }
      }
      return StatusOr<bool>(false);
    }
    return StatusOr<bool>::MakeError(status.exceptionMessage().c_str());
  }

  StatusOr<std::vector<ApexInfo>> GetAllPackages() {
    std::vector<ApexInfo> list;
    android::binder::Status status = service_->getAllPackages(&list);
    if (status.isOk()) {
      return StatusOr<std::vector<ApexInfo>>(list);
    }

    return StatusOr<std::vector<ApexInfo>>::MakeError(
        status.toString8().c_str());
  }

  StatusOr<std::vector<ApexInfo>> GetActivePackages() {
    std::vector<ApexInfo> list;
    android::binder::Status status = service_->getActivePackages(&list);
    if (status.isOk()) {
      return StatusOr<std::vector<ApexInfo>>(list);
    }

    return StatusOr<std::vector<ApexInfo>>::MakeError(
        status.exceptionMessage().c_str());
  }

  StatusOr<std::vector<ApexInfo>> GetInactivePackages() {
    std::vector<ApexInfo> list;
    android::binder::Status status = service_->getAllPackages(&list);
    list.erase(std::remove_if(
                   list.begin(), list.end(),
                   [](const ApexInfo& apexInfo) { return apexInfo.isActive; }),
               list.end());
    if (status.isOk()) {
      return StatusOr<std::vector<ApexInfo>>(std::move(list));
    }

    return StatusOr<std::vector<ApexInfo>>::MakeError(
        status.toString8().c_str());
  }

  StatusOr<ApexInfo> GetActivePackage(const std::string& name) {
    ApexInfo package;
    android::binder::Status status = service_->getActivePackage(name, &package);
    if (status.isOk()) {
      return StatusOr<ApexInfo>(package);
    }

    return StatusOr<ApexInfo>::MakeError(status.exceptionMessage().c_str());
  }

  std::string GetPackageString(const ApexInfo& p) {
    return p.packageName + "@" + std::to_string(p.versionCode) +
           " [path=" + p.packagePath + "]";
  }

  std::vector<std::string> GetPackagesStrings(
      const std::vector<ApexInfo>& list) {
    std::vector<std::string> ret;
    ret.reserve(list.size());
    for (const ApexInfo& p : list) {
      ret.push_back(GetPackageString(p));
    }
    return ret;
  }

  std::vector<std::string> GetActivePackagesStrings() {
    std::vector<ApexInfo> list;
    android::binder::Status status = service_->getActivePackages(&list);
    if (status.isOk()) {
      std::vector<std::string> ret(list.size());
      for (const ApexInfo& p : list) {
        ret.push_back(GetPackageString(p));
      }
      return ret;
    }

    std::vector<std::string> error;
    error.push_back("ERROR");
    return error;
  }

  StatusOr<std::vector<ApexInfo>> GetFactoryPackages() {
    std::vector<ApexInfo> list;
    android::binder::Status status = service_->getAllPackages(&list);
    list.erase(
        std::remove_if(list.begin(), list.end(),
                       [](ApexInfo& apexInfo) { return !apexInfo.isFactory; }),
        list.end());
    if (status.isOk()) {
      return StatusOr<std::vector<ApexInfo>>(std::move(list));
    }

    return StatusOr<std::vector<ApexInfo>>::MakeError(
        status.toString8().c_str());
  }

  static std::vector<std::string> ListDir(const std::string& path) {
    std::vector<std::string> ret;
    std::error_code ec;
    if (!fs::is_directory(path, ec)) {
      return ret;
    }
    WalkDir(path, [&](const fs::directory_entry& entry) {
      std::string tmp;
      switch (entry.symlink_status(ec).type()) {
        case fs::file_type::directory:
          tmp = "[dir]";
          break;
        case fs::file_type::symlink:
          tmp = "[lnk]";
          break;
        case fs::file_type::regular:
          tmp = "[reg]";
          break;
        default:
          tmp = "[other]";
      }
      ret.push_back(tmp.append(entry.path().filename()));
    });
    std::sort(ret.begin(), ret.end());
    return ret;
  }

  static std::string GetLogcat() {
    // For simplicity, log to file and read it.
    std::string file = GetTestFile("logcat.tmp.txt");
    std::vector<std::string> args{
        "/system/bin/logcat",
        "-d",
        "-f",
        file,
    };
    std::string error_msg;
    int res = ForkAndRun(args, &error_msg);
    CHECK_EQ(0, res) << error_msg;

    std::string data;
    CHECK(android::base::ReadFileToString(file, &data));

    unlink(file.c_str());

    return data;
  }

  struct PrepareTestApexForInstall {
    static constexpr const char* kTestDir = "/data/app-staging/apexservice_tmp";

    // This is given to the constructor.
    std::string test_input;           // Original test file.
    std::string selinux_label_input;  // SELinux label to apply.
    std::string test_dir_input;

    // This is derived from the input.
    std::string test_file;            // Prepared path. Under test_dir_input.
    std::string test_installed_file;  // Where apexd will store it.

    std::string package;  // APEX package name.
    uint64_t version;     // APEX version

    explicit PrepareTestApexForInstall(
        const std::string& test,
        const std::string& test_dir = std::string(kTestDir),
        const std::string& selinux_label = "staging_data_file") {
      test_input = test;
      selinux_label_input = selinux_label;
      test_dir_input = test_dir;

      test_file = test_dir_input + "/" + android::base::Basename(test);

      package = "";  // Explicitly mark as not initialized.

      StatusOr<ApexFile> apex_file = ApexFile::Open(test);
      if (!apex_file.Ok()) {
        return;
      }

      const ApexManifest& manifest = apex_file->GetManifest();
      package = manifest.name();
      version = manifest.version();

      test_installed_file = std::string(kActiveApexPackagesDataDir) + "/" +
                            package + "@" + std::to_string(version) + ".apex";
    }

    bool Prepare() {
      if (package.empty()) {
        // Failure in constructor. Redo work to get error message.
        auto fail_fn = [&]() {
          StatusOr<ApexFile> apex_file = ApexFile::Open(test_input);
          ASSERT_FALSE(IsOk(apex_file));
          ASSERT_TRUE(apex_file.Ok())
              << test_input << " failed to load: " << apex_file.ErrorMessage();
        };
        fail_fn();
        return false;
      }

      auto prepare = [](const std::string& src, const std::string& trg,
                        const std::string& selinux_label) {
        ASSERT_EQ(0, access(src.c_str(), F_OK))
            << src << ": " << strerror(errno);
        const std::string trg_dir = android::base::Dirname(trg);
        if (0 != mkdir(trg_dir.c_str(), 0777)) {
          int saved_errno = errno;
          ASSERT_EQ(saved_errno, EEXIST) << trg << ":" << strerror(saved_errno);
        }

        // Do not use a hardlink, even though it's the simplest solution.
        // b/119569101.
        {
          std::ifstream src_stream(src, std::ios::binary);
          ASSERT_TRUE(src_stream.good());
          std::ofstream trg_stream(trg, std::ios::binary);
          ASSERT_TRUE(trg_stream.good());

          trg_stream << src_stream.rdbuf();
        }

        ASSERT_EQ(0, chmod(trg.c_str(), 0666)) << strerror(errno);
        struct group* g = getgrnam("system");
        ASSERT_NE(nullptr, g);
        ASSERT_EQ(0, chown(trg.c_str(), /* root uid */ 0, g->gr_gid))
            << strerror(errno);

        int rc = setfilecon(
            trg_dir.c_str(),
            std::string("u:object_r:" + selinux_label + ":s0").c_str());
        ASSERT_TRUE(0 == rc || !HaveSelinux()) << strerror(errno);
        rc = setfilecon(
            trg.c_str(),
            std::string("u:object_r:" + selinux_label + ":s0").c_str());
        ASSERT_TRUE(0 == rc || !HaveSelinux()) << strerror(errno);
      };
      prepare(test_input, test_file, selinux_label_input);
      return !HasFatalFailure();
    }

    ~PrepareTestApexForInstall() {
      if (unlink(test_file.c_str()) != 0) {
        PLOG(ERROR) << "Unable to unlink " << test_file;
      }
      if (rmdir(test_dir_input.c_str()) != 0) {
        PLOG(ERROR) << "Unable to rmdir " << test_dir_input;
      }

      if (!package.empty()) {
        // For cleanliness, also attempt to delete apexd's file.
        // TODO: to the unstaging using APIs
        if (unlink(test_installed_file.c_str()) != 0) {
          PLOG(ERROR) << "Unable to unlink " << test_installed_file;
        }
      }
    }
  };

  std::string GetDebugStr(PrepareTestApexForInstall* installer) {
    StringLog log;

    if (installer != nullptr) {
      log << "test_input=" << installer->test_input << " ";
      log << "test_file=" << installer->test_file << " ";
      log << "test_installed_file=" << installer->test_installed_file << " ";
      log << "package=" << installer->package << " ";
      log << "version=" << installer->version << " ";
    }

    log << "active=[" << Join(GetActivePackagesStrings(), ',') << "] ";
    log << kActiveApexPackagesDataDir << "=["
        << Join(ListDir(kActiveApexPackagesDataDir), ',') << "] ";
    log << kApexRoot << "=[" << Join(ListDir(kApexRoot), ',') << "]";

    return log;
  }

  sp<IApexService> service_;
  sp<android::os::IVold> vold_service_;
  bool supports_fs_checkpointing_;

 private:
  void CleanUp() {
    auto status = WalkDir(kApexDataDir, [](const fs::directory_entry& p) {
      std::error_code ec;
      fs::file_status status = p.status(ec);
      ASSERT_FALSE(ec) << "Failed to stat " << p.path() << " : "
                       << ec.message();
      if (fs::is_directory(status)) {
        fs::remove_all(p.path(), ec);
      } else {
        fs::remove(p.path(), ec);
      }
      ASSERT_FALSE(ec) << "Failed to delete " << p.path() << " : "
                       << ec.message();
    });
    ASSERT_TRUE(IsOk(status));
  }
};

namespace {

bool RegularFileExists(const std::string& path) {
  struct stat buf;
  if (0 != stat(path.c_str(), &buf)) {
    return false;
  }
  return S_ISREG(buf.st_mode);
}

}  // namespace

TEST_F(ApexServiceTest, HaveSelinux) {
  // We want to test under selinux.
  EXPECT_TRUE(HaveSelinux());
}

// Skip for b/119032200.
TEST_F(ApexServiceTest, DISABLED_EnforceSelinux) {
  // Crude cutout for virtual devices.
#if !defined(__i386__) && !defined(__x86_64__)
  constexpr bool kIsX86 = false;
#else
  constexpr bool kIsX86 = true;
#endif
  EXPECT_TRUE(IsSelinuxEnforced() || kIsX86);
}

TEST_F(ApexServiceTest, StageFailAccess) {
  if (!IsSelinuxEnforced()) {
    LOG(WARNING) << "Skipping InstallFailAccess because of selinux";
    return;
  }

  // Use an extra copy, so that even if this test fails (incorrectly installs),
  // we have the testdata file still around.
  std::string orig_test_file = GetTestFile("apex.apexd_test.apex");
  std::string test_file = orig_test_file + ".2";
  ASSERT_EQ(0, link(orig_test_file.c_str(), test_file.c_str()))
      << strerror(errno);
  struct Deleter {
    std::string to_delete;
    explicit Deleter(const std::string& t) : to_delete(t) {}
    ~Deleter() {
      if (unlink(to_delete.c_str()) != 0) {
        PLOG(ERROR) << "Could not unlink " << to_delete;
      }
    }
  };
  Deleter del(test_file);

  bool success;
  android::binder::Status st = service_->stagePackage(test_file, &success);
  ASSERT_FALSE(IsOk(st));
  std::string error = st.exceptionMessage().c_str();
  EXPECT_NE(std::string::npos, error.find("Failed to open package")) << error;
  EXPECT_NE(std::string::npos, error.find("I/O error")) << error;
}

// TODO(jiyong): re-enable this test. This test is disabled because the build
// system now always bundles the public key that was used to sign the APEX.
// In debuggable build, the bundled public key is used as the last fallback.
// As a result, the verification is always successful (and thus test fails).
// In order to re-enable this test, we have to manually create an APEX
// where public key is not bundled.
#if 0
TEST_F(ApexServiceTest, StageFailKey) {
  PrepareTestApexForInstall installer(
      GetTestFile("apex.apexd_test_no_inst_key.apex"));
  if (!installer.Prepare()) {
    return;
  }
  ASSERT_EQ(std::string("com.android.apex.test_package.no_inst_key"),
            installer.package);

  bool success;
  android::binder::Status st =
      service_->stagePackage(installer.test_file, &success);
  ASSERT_FALSE(IsOk(st));

  // May contain one of two errors.
  std::string error = st.exceptionMessage().c_str();

  constexpr const char* kExpectedError1 = "Failed to get realpath of ";
  const size_t pos1 = error.find(kExpectedError1);
  constexpr const char* kExpectedError2 =
      "/etc/security/apex/com.android.apex.test_package.no_inst_key";
  const size_t pos2 = error.find(kExpectedError2);

  constexpr const char* kExpectedError3 =
      "Error verifying "
      "/data/app-staging/apexservice_tmp/apex.apexd_test_no_inst_key.apex: "
      "couldn't verify public key: Failed to compare the bundled public key "
      "with key";
  const size_t pos3 = error.find(kExpectedError3);

  const size_t npos = std::string::npos;
  EXPECT_TRUE((pos1 != npos && pos2 != npos) || pos3 != npos) << error;
}
#endif

TEST_F(ApexServiceTest, StageSuccess) {
  PrepareTestApexForInstall installer(GetTestFile("apex.apexd_test.apex"));
  if (!installer.Prepare()) {
    return;
  }
  ASSERT_EQ(std::string("com.android.apex.test_package"), installer.package);

  bool success;
  ASSERT_TRUE(IsOk(service_->stagePackage(installer.test_file, &success)));
  ASSERT_TRUE(success);
  EXPECT_TRUE(RegularFileExists(installer.test_installed_file));
}

TEST_F(ApexServiceTest,
       SubmitStagegSessionSuccessDoesNotLeakTempVerityDevices) {
  using android::dm::DeviceMapper;

  PrepareTestApexForInstall installer(GetTestFile("apex.apexd_test.apex"),
                                      "/data/app-staging/session_1543",
                                      "staging_data_file");
  if (!installer.Prepare()) {
    return;
  }

  ApexInfoList list;
  bool success;
  ASSERT_TRUE(IsOk(service_->submitStagedSession(1543, {}, &list, &success)));
  ASSERT_TRUE(success);

  std::vector<DeviceMapper::DmBlockDevice> devices;
  DeviceMapper& dm = DeviceMapper::Instance();
  ASSERT_TRUE(dm.GetAvailableDevices(&devices));

  for (const auto& device : devices) {
    ASSERT_THAT(device.name(), Not(EndsWith(".tmp")));
  }
}

TEST_F(ApexServiceTest, SubmitStagedSessionStoresBuildFingerprint) {
  PrepareTestApexForInstall installer(GetTestFile("apex.apexd_test.apex"),
                                      "/data/app-staging/session_1547",
                                      "staging_data_file");
  if (!installer.Prepare()) {
    return;
  }
  ApexInfoList list;
  bool success;
  ASSERT_TRUE(IsOk(service_->submitStagedSession(1547, {}, &list, &success)));

  auto session = ApexSession::GetSession(1547);
  ASSERT_FALSE(session->GetBuildFingerprint().empty());
}

TEST_F(ApexServiceTest, SubmitStagedSessionFailDoesNotLeakTempVerityDevices) {
  using android::dm::DeviceMapper;

  PrepareTestApexForInstall installer(
      GetTestFile("apex.apexd_test_manifest_mismatch.apex"),
      "/data/app-staging/session_239", "staging_data_file");
  if (!installer.Prepare()) {
    return;
  }

  ApexInfoList list;
  bool success;
  ASSERT_TRUE(IsOk(service_->submitStagedSession(239, {}, &list, &success)));
  ASSERT_FALSE(success);

  std::vector<DeviceMapper::DmBlockDevice> devices;
  DeviceMapper& dm = DeviceMapper::Instance();
  ASSERT_TRUE(dm.GetAvailableDevices(&devices));

  for (const auto& device : devices) {
    ASSERT_THAT(device.name(), Not(EndsWith(".tmp")));
  }
}

TEST_F(ApexServiceTest, StageSuccess_ClearsPreviouslyActivePackage) {
  PrepareTestApexForInstall installer1(GetTestFile("apex.apexd_test_v2.apex"));
  PrepareTestApexForInstall installer2(
      GetTestFile("apex.apexd_test_different_app.apex"));
  PrepareTestApexForInstall installer3(GetTestFile("apex.apexd_test.apex"));
  auto install_fn = [&](PrepareTestApexForInstall& installer) {
    if (!installer.Prepare()) {
      return;
    }
    bool success;
    ASSERT_TRUE(IsOk(service_->stagePackage(installer.test_file, &success)));
    ASSERT_TRUE(success);
    EXPECT_TRUE(RegularFileExists(installer.test_installed_file));
  };
  install_fn(installer1);
  install_fn(installer2);
  // Simulating a rollback. After this call test_v2_apex_path should be removed.
  install_fn(installer3);

  EXPECT_FALSE(RegularFileExists(installer1.test_installed_file));
  EXPECT_TRUE(RegularFileExists(installer2.test_installed_file));
  EXPECT_TRUE(RegularFileExists(installer3.test_installed_file));
}

TEST_F(ApexServiceTest, StageAlreadyStagedPackageSuccess) {
  PrepareTestApexForInstall installer(GetTestFile("apex.apexd_test.apex"));
  if (!installer.Prepare()) {
    return;
  }
  ASSERT_EQ(std::string("com.android.apex.test_package"), installer.package);

  bool success = false;
  ASSERT_TRUE(IsOk(service_->stagePackage(installer.test_file, &success)));
  ASSERT_TRUE(success);
  ASSERT_TRUE(RegularFileExists(installer.test_installed_file));

  success = false;
  ASSERT_TRUE(IsOk(service_->stagePackage(installer.test_file, &success)));
  ASSERT_TRUE(success);
  ASSERT_TRUE(RegularFileExists(installer.test_installed_file));
}

TEST_F(ApexServiceTest, MultiStageSuccess) {
  PrepareTestApexForInstall installer(GetTestFile("apex.apexd_test.apex"));
  if (!installer.Prepare()) {
    return;
  }
  ASSERT_EQ(std::string("com.android.apex.test_package"), installer.package);

  // TODO: Add second test. Right now, just use a separate version.
  PrepareTestApexForInstall installer2(GetTestFile("apex.apexd_test_v2.apex"));
  if (!installer2.Prepare()) {
    return;
  }
  ASSERT_EQ(std::string("com.android.apex.test_package"), installer2.package);

  std::vector<std::string> packages;
  packages.push_back(installer.test_file);
  packages.push_back(installer2.test_file);

  bool success;
  ASSERT_TRUE(IsOk(service_->stagePackages(packages, &success)));
  ASSERT_TRUE(success);
  EXPECT_TRUE(RegularFileExists(installer.test_installed_file));
  EXPECT_TRUE(RegularFileExists(installer2.test_installed_file));
}

template <typename NameProvider>
class ApexServiceActivationTest : public ApexServiceTest {
 public:
  ApexServiceActivationTest() : stage_package(true) {}

  explicit ApexServiceActivationTest(bool stage_package)
      : stage_package(stage_package) {}

  void SetUp() override {
    ApexServiceTest::SetUp();
    ASSERT_NE(nullptr, service_.get());

    installer_ = std::make_unique<PrepareTestApexForInstall>(
        GetTestFile(NameProvider::GetTestName()));
    if (!installer_->Prepare()) {
      return;
    }
    ASSERT_EQ(NameProvider::GetPackageName(), installer_->package);

    {
      // Check package is not active.
      StatusOr<bool> active =
          IsActive(installer_->package, installer_->version);
      ASSERT_TRUE(IsOk(active));
      ASSERT_FALSE(*active);
    }

    if (stage_package) {
      bool success;
      ASSERT_TRUE(
          IsOk(service_->stagePackage(installer_->test_file, &success)));
      ASSERT_TRUE(success);
    }
  }

  void TearDown() override {
    // Attempt to deactivate.
    if (installer_ != nullptr) {
      if (stage_package) {
        service_->deactivatePackage(installer_->test_installed_file);
      } else {
        service_->deactivatePackage(installer_->test_file);
      }
    }

    installer_.reset();
    // ApexServiceTest::TearDown will wipe out everything under /data/apex.
    // Since some of that information is required for deactivePackage binder
    // call, it's required to be called after deactivating package.
    ApexServiceTest::TearDown();
  }

  std::unique_ptr<PrepareTestApexForInstall> installer_;

 private:
  bool stage_package;
};

struct SuccessNameProvider {
  static std::string GetTestName() { return "apex.apexd_test.apex"; }
  static std::string GetPackageName() {
    return "com.android.apex.test_package";
  }
};

struct ManifestMismatchNameProvider {
  static std::string GetTestName() {
    return "apex.apexd_test_manifest_mismatch.apex";
  }
  static std::string GetPackageName() {
    return "com.android.apex.test_package";
  }
};

class ApexServiceActivationManifestMismatchFailure
    : public ApexServiceActivationTest<ManifestMismatchNameProvider> {
 public:
  ApexServiceActivationManifestMismatchFailure()
      : ApexServiceActivationTest(false) {}
};

TEST_F(ApexServiceActivationManifestMismatchFailure,
       ActivateFailsWithManifestMismatch) {
  android::binder::Status st = service_->activatePackage(installer_->test_file);
  ASSERT_FALSE(IsOk(st));

  std::string error = st.exceptionMessage().c_str();
  ASSERT_THAT(
      error,
      HasSubstr(
          "Manifest inside filesystem does not match manifest outside it"));
}

class ApexServiceActivationSuccessTest
    : public ApexServiceActivationTest<SuccessNameProvider> {};

TEST_F(ApexServiceActivationSuccessTest, Activate) {
  ASSERT_TRUE(IsOk(service_->activatePackage(installer_->test_installed_file)))
      << GetDebugStr(installer_.get());

  {
    // Check package is active.
    StatusOr<bool> active = IsActive(installer_->package, installer_->version);
    ASSERT_TRUE(IsOk(active));
    ASSERT_TRUE(*active) << Join(GetActivePackagesStrings(), ',');
  }

  {
    // Check that the "latest" view exists.
    std::string latest_path =
        std::string(kApexRoot) + "/" + installer_->package;
    struct stat buf;
    ASSERT_EQ(0, stat(latest_path.c_str(), &buf)) << strerror(errno);
    // Check that it is a folder.
    EXPECT_TRUE(S_ISDIR(buf.st_mode));

    // Collect direct entries of a folder.
    auto collect_entries_fn = [](const std::string& path) {
      std::vector<std::string> ret;
      WalkDir(path, [&](const fs::directory_entry& entry) {
        if (!entry.is_directory()) {
          return;
        }
        ret.emplace_back(entry.path().filename());
      });
      std::sort(ret.begin(), ret.end());
      return ret;
    };

    std::string versioned_path = std::string(kApexRoot) + "/" +
                                 installer_->package + "@" +
                                 std::to_string(installer_->version);
    std::vector<std::string> versioned_folder_entries =
        collect_entries_fn(versioned_path);
    std::vector<std::string> latest_folder_entries =
        collect_entries_fn(latest_path);

    EXPECT_TRUE(versioned_folder_entries == latest_folder_entries)
        << "Versioned: " << Join(versioned_folder_entries, ',')
        << " Latest: " << Join(latest_folder_entries, ',');
  }
}

TEST_F(ApexServiceActivationSuccessTest, GetActivePackages) {
  ASSERT_TRUE(IsOk(service_->activatePackage(installer_->test_installed_file)))
      << GetDebugStr(installer_.get());

  StatusOr<std::vector<ApexInfo>> active = GetActivePackages();
  ASSERT_TRUE(IsOk(active));
  ApexInfo match;

  for (const ApexInfo& info : *active) {
    if (info.packageName == installer_->package) {
      match = info;
      break;
    }
  }

  ASSERT_EQ(installer_->package, match.packageName);
  ASSERT_EQ(installer_->version, static_cast<uint64_t>(match.versionCode));
  ASSERT_EQ(installer_->test_installed_file, match.packagePath);
}

TEST_F(ApexServiceActivationSuccessTest, GetActivePackage) {
  ASSERT_TRUE(IsOk(service_->activatePackage(installer_->test_installed_file)))
      << GetDebugStr(installer_.get());

  StatusOr<ApexInfo> active = GetActivePackage(installer_->package);
  ASSERT_TRUE(IsOk(active));

  ASSERT_EQ(installer_->package, active->packageName);
  ASSERT_EQ(installer_->version, static_cast<uint64_t>(active->versionCode));
  ASSERT_EQ(installer_->test_installed_file, active->packagePath);
}

TEST_F(ApexServiceTest, GetFactoryPackages) {
  using ::android::base::StartsWith;
  StatusOr<std::vector<ApexInfo>> factoryPackages = GetFactoryPackages();
  ASSERT_TRUE(IsOk(factoryPackages));
  ASSERT_TRUE(factoryPackages->size() > 0);

  for (const ApexInfo& package : *factoryPackages) {
    ASSERT_TRUE(isPathForBuiltinApexes(package.packagePath));
  }
}

TEST_F(ApexServiceTest, NoPackagesAreBothActiveAndInactive) {
  StatusOr<std::vector<ApexInfo>> activePackages = GetActivePackages();
  ASSERT_TRUE(IsOk(activePackages));
  ASSERT_TRUE(activePackages->size() > 0);
  StatusOr<std::vector<ApexInfo>> inactivePackages = GetInactivePackages();
  ASSERT_TRUE(IsOk(inactivePackages));
  std::vector<std::string> activePackagesStrings =
      GetPackagesStrings(*activePackages);
  std::vector<std::string> inactivePackagesStrings =
      GetPackagesStrings(*inactivePackages);
  std::sort(activePackagesStrings.begin(), activePackagesStrings.end());
  std::sort(inactivePackagesStrings.begin(), inactivePackagesStrings.end());
  std::vector<std::string> intersection;
  std::set_intersection(
      activePackagesStrings.begin(), activePackagesStrings.end(),
      inactivePackagesStrings.begin(), inactivePackagesStrings.end(),
      std::back_inserter(intersection));
  ASSERT_EQ(intersection.size(), 0UL);
}

TEST_F(ApexServiceTest, GetAllPackages) {
  StatusOr<std::vector<ApexInfo>> allPackages = GetAllPackages();
  ASSERT_TRUE(IsOk(allPackages));
  ASSERT_TRUE(allPackages->size() > 0);
  StatusOr<std::vector<ApexInfo>> activePackages = GetActivePackages();
  std::vector<std::string> activeStrings = GetPackagesStrings(*activePackages);
  StatusOr<std::vector<ApexInfo>> factoryPackages = GetFactoryPackages();
  std::vector<std::string> factoryStrings =
      GetPackagesStrings(*factoryPackages);
  for (ApexInfo& apexInfo : *allPackages) {
    std::string packageString = GetPackageString(apexInfo);
    bool shouldBeActive = std::find(activeStrings.begin(), activeStrings.end(),
                                    packageString) != activeStrings.end();
    bool shouldBeFactory =
        std::find(factoryStrings.begin(), factoryStrings.end(),
                  packageString) != factoryStrings.end();
    ASSERT_EQ(shouldBeActive, apexInfo.isActive);
    ASSERT_EQ(shouldBeFactory, apexInfo.isFactory);
  }
}

TEST_F(ApexServiceActivationSuccessTest, StageAlreadyActivePackageSameVersion) {
  ASSERT_TRUE(IsOk(service_->activatePackage(installer_->test_installed_file)))
      << GetDebugStr(installer_.get());

  bool success = false;
  ASSERT_TRUE(IsOk(service_->stagePackage(installer_->test_file, &success)));
  ASSERT_TRUE(success);
}

class ApexServiceDeactivationTest : public ApexServiceActivationSuccessTest {
 public:
  void SetUp() override {
    ApexServiceActivationSuccessTest::SetUp();

    ASSERT_TRUE(installer_ != nullptr);
  }

  void TearDown() override {
    installer_.reset();
    ApexServiceActivationSuccessTest::TearDown();
  }

  std::unique_ptr<PrepareTestApexForInstall> installer_;
};

TEST_F(ApexServiceActivationSuccessTest, DmDeviceTearDown) {
  std::string package_id =
      installer_->package + "@" + std::to_string(installer_->version);

  auto find_fn = [](const std::string& name) {
    auto& dm = dm::DeviceMapper::Instance();
    std::vector<dm::DeviceMapper::DmBlockDevice> devices;
    if (!dm.GetAvailableDevices(&devices)) {
      return StatusOr<bool>::Fail("GetAvailableDevices failed");
    }
    for (const auto& device : devices) {
      if (device.name() == name) {
        return StatusOr<bool>(true);
      }
    }
    return StatusOr<bool>(false);
  };

#define ASSERT_FIND(type)                     \
  {                                           \
    StatusOr<bool> res = find_fn(package_id); \
    ASSERT_TRUE(res.Ok());                    \
    ASSERT_##type(*res);                      \
  }

  ASSERT_FIND(FALSE);

  ASSERT_TRUE(IsOk(service_->activatePackage(installer_->test_installed_file)))
      << GetDebugStr(installer_.get());

  ASSERT_FIND(TRUE);

  ASSERT_TRUE(
      IsOk(service_->deactivatePackage(installer_->test_installed_file)));

  ASSERT_FIND(FALSE);

  installer_.reset();  // Skip TearDown deactivatePackage.
}

class ApexServicePrePostInstallTest : public ApexServiceTest {
 public:
  template <typename Fn>
  void RunPrePost(Fn fn, const std::vector<std::string>& apex_names,
                  const char* test_message, bool expect_success = true) {
    // Using unique_ptr is just the easiest here.
    using InstallerUPtr = std::unique_ptr<PrepareTestApexForInstall>;
    std::vector<InstallerUPtr> installers;
    std::vector<std::string> pkgs;

    for (const std::string& apex_name : apex_names) {
      InstallerUPtr installer(
          new PrepareTestApexForInstall(GetTestFile(apex_name)));
      if (!installer->Prepare()) {
        return;
      }
      pkgs.push_back(installer->test_file);
      installers.emplace_back(std::move(installer));
    }
    android::binder::Status st = (service_.get()->*fn)(pkgs);
    if (expect_success) {
      ASSERT_TRUE(IsOk(st));
    } else {
      ASSERT_FALSE(IsOk(st));
    }

    if (test_message != nullptr) {
      std::string logcat = GetLogcat();
      EXPECT_NE(std::string::npos, logcat.find(test_message)) << logcat;
    }

    // Ensure that the package is neither active nor mounted.
    for (const InstallerUPtr& installer : installers) {
      StatusOr<bool> active = IsActive(installer->package, installer->version);
      ASSERT_TRUE(IsOk(active));
      EXPECT_FALSE(*active);
    }
    for (const InstallerUPtr& installer : installers) {
      StatusOr<ApexFile> apex = ApexFile::Open(installer->test_input);
      ASSERT_TRUE(IsOk(apex));
      std::string path =
          apexd_private::GetPackageMountPoint(apex->GetManifest());
      std::string entry = std::string("[dir]").append(path);
      std::vector<std::string> slash_apex = ListDir(kApexRoot);
      auto it = std::find(slash_apex.begin(), slash_apex.end(), entry);
      EXPECT_TRUE(it == slash_apex.end()) << Join(slash_apex, ',');
    }
  }
};

TEST_F(ApexServicePrePostInstallTest, Preinstall) {
  RunPrePost(&IApexService::preinstallPackages,
             {"apex.apexd_test_preinstall.apex"}, "sh      : PreInstall Test");
}

TEST_F(ApexServicePrePostInstallTest, MultiPreinstall) {
  constexpr const char* kLogcatText =
      "sh      : /apex/com.android.apex.test_package/etc/sample_prebuilt_file";
  RunPrePost(&IApexService::preinstallPackages,
             {"apex.apexd_test_preinstall.apex", "apex.apexd_test.apex"},
             kLogcatText);
}

TEST_F(ApexServicePrePostInstallTest, PreinstallFail) {
  RunPrePost(&IApexService::preinstallPackages,
             {"apex.apexd_test_prepostinstall.fail.apex"},
             /* test_message= */ nullptr, /* expect_success= */ false);
}

TEST_F(ApexServicePrePostInstallTest, Postinstall) {
  RunPrePost(&IApexService::postinstallPackages,
             {"apex.apexd_test_postinstall.apex"},
             "sh      : PostInstall Test");
}

TEST_F(ApexServicePrePostInstallTest, MultiPostinstall) {
  constexpr const char* kLogcatText =
      "sh      : /apex/com.android.apex.test_package/etc/sample_prebuilt_file";
  RunPrePost(&IApexService::postinstallPackages,
             {"apex.apexd_test_postinstall.apex", "apex.apexd_test.apex"},
             kLogcatText);
}

TEST_F(ApexServicePrePostInstallTest, PostinstallFail) {
  RunPrePost(&IApexService::postinstallPackages,
             {"apex.apexd_test_prepostinstall.fail.apex"},
             /* test_message= */ nullptr, /* expect_success= */ false);
}

TEST_F(ApexServiceTest, SubmitSingleSessionTestSuccess) {
  PrepareTestApexForInstall installer(GetTestFile("apex.apexd_test.apex"),
                                      "/data/app-staging/session_123",
                                      "staging_data_file");
  if (!installer.Prepare()) {
    FAIL() << GetDebugStr(&installer);
  }

  ApexInfoList list;
  bool ret_value;
  std::vector<int> empty_child_session_ids;
  ASSERT_TRUE(IsOk(service_->submitStagedSession(123, empty_child_session_ids,
                                                 &list, &ret_value)))
      << GetDebugStr(&installer);
  EXPECT_TRUE(ret_value);
  EXPECT_EQ(1u, list.apexInfos.size());
  ApexInfo match;
  for (const ApexInfo& info : list.apexInfos) {
    if (info.packageName == installer.package) {
      match = info;
      break;
    }
  }

  ASSERT_EQ(installer.package, match.packageName);
  ASSERT_EQ(installer.version, static_cast<uint64_t>(match.versionCode));
  ASSERT_EQ(installer.test_file, match.packagePath);

  ApexSessionInfo session;
  ASSERT_TRUE(IsOk(service_->getStagedSessionInfo(123, &session)))
      << GetDebugStr(&installer);
  ApexSessionInfo expected = CreateSessionInfo(123);
  expected.isVerified = true;
  EXPECT_THAT(session, SessionInfoEq(expected));

  ASSERT_TRUE(IsOk(service_->markStagedSessionReady(123, &ret_value)))
      << GetDebugStr(&installer);
  ASSERT_TRUE(ret_value);

  ASSERT_TRUE(IsOk(service_->getStagedSessionInfo(123, &session)))
      << GetDebugStr(&installer);
  expected.isVerified = false;
  expected.isStaged = true;
  EXPECT_THAT(session, SessionInfoEq(expected));

  // Call markStagedSessionReady again. Should be a no-op.
  ASSERT_TRUE(IsOk(service_->markStagedSessionReady(123, &ret_value)))
      << GetDebugStr(&installer);
  ASSERT_TRUE(ret_value);

  ASSERT_TRUE(IsOk(service_->getStagedSessionInfo(123, &session)))
      << GetDebugStr(&installer);
  EXPECT_THAT(session, SessionInfoEq(expected));

  // See if the session is reported with getSessions() as well
  std::vector<ApexSessionInfo> sessions;
  ASSERT_TRUE(IsOk(service_->getSessions(&sessions)))
      << GetDebugStr(&installer);
  ASSERT_THAT(sessions, UnorderedElementsAre(SessionInfoEq(expected)));
}

TEST_F(ApexServiceTest, SubmitSingleStagedSessionDeletesPreviousSessions) {
  PrepareTestApexForInstall installer(GetTestFile("apex.apexd_test.apex"),
                                      "/data/app-staging/session_239",
                                      "staging_data_file");
  if (!installer.Prepare()) {
    FAIL() << GetDebugStr(&installer);
  }

  // First simulate existence of a bunch of sessions.
  auto session1 = ApexSession::CreateSession(37);
  ASSERT_TRUE(IsOk(session1));
  auto session2 = ApexSession::CreateSession(57);
  ASSERT_TRUE(IsOk(session2));
  auto session3 = ApexSession::CreateSession(73);
  ASSERT_TRUE(IsOk(session3));
  ASSERT_TRUE(IsOk(session1->UpdateStateAndCommit(SessionState::VERIFIED)));
  ASSERT_TRUE(IsOk(session2->UpdateStateAndCommit(SessionState::STAGED)));
  ASSERT_TRUE(IsOk(session3->UpdateStateAndCommit(SessionState::SUCCESS)));

  std::vector<ApexSessionInfo> sessions;
  ASSERT_TRUE(IsOk(service_->getSessions(&sessions)));

  ApexSessionInfo expected_session1 = CreateSessionInfo(37);
  expected_session1.isVerified = true;
  ApexSessionInfo expected_session2 = CreateSessionInfo(57);
  expected_session2.isStaged = true;
  ApexSessionInfo expected_session3 = CreateSessionInfo(73);
  expected_session3.isSuccess = true;
  ASSERT_THAT(sessions, UnorderedElementsAre(SessionInfoEq(expected_session1),
                                             SessionInfoEq(expected_session2),
                                             SessionInfoEq(expected_session3)));

  ApexInfoList list;
  bool ret_value;
  std::vector<int> empty_child_session_ids;
  ASSERT_TRUE(IsOk(service_->submitStagedSession(239, empty_child_session_ids,
                                                 &list, &ret_value)));
  EXPECT_TRUE(ret_value);

  sessions.clear();
  ASSERT_TRUE(IsOk(service_->getSessions(&sessions)));

  ApexSessionInfo new_session = CreateSessionInfo(239);
  new_session.isVerified = true;
  ASSERT_THAT(sessions, UnorderedElementsAre(SessionInfoEq(new_session)));
}

// TODO(jiyong): re-enable this test. This test is disabled because the build
// system now always bundles the public key that was used to sign the APEX.
// In debuggable build, the bundled public key is used as the last fallback.
// As a result, the verification is always successful (and thus test fails).
// In order to re-enable this test, we have to manually create an APEX
// where public key is not bundled.
#if 0
TEST_F(ApexServiceTest, SubmitSingleSessionTestFail) {
  PrepareTestApexForInstall installer(
      GetTestFile("apex.apexd_test_no_inst_key.apex"),
      "/data/app-staging/session_456", "staging_data_file");
  if (!installer.Prepare()) {
    FAIL() << GetDebugStr(&installer);
  }

  ApexInfoList list;
  bool ret_value;
  std::vector<int> empty_child_session_ids;
  ASSERT_TRUE(IsOk(service_->submitStagedSession(456, empty_child_session_ids,
                                                 &list, &ret_value)))
      << GetDebugStr(&installer);
  EXPECT_FALSE(ret_value);

  ApexSessionInfo session;
  ASSERT_TRUE(IsOk(service_->getStagedSessionInfo(456, &session)))
      << GetDebugStr(&installer);
  ApexSessionInfo expected = CreateSessionInfo(-1);
  expected.isUnknown = true;
  EXPECT_THAT(session, SessionInfoEq(expected));
}
#endif

TEST_F(ApexServiceTest, SubmitMultiSessionTestSuccess) {
  // Parent session id: 10
  // Children session ids: 20 30
  PrepareTestApexForInstall installer(GetTestFile("apex.apexd_test.apex"),
                                      "/data/app-staging/session_20",
                                      "staging_data_file");
  PrepareTestApexForInstall installer2(
      GetTestFile("apex.apexd_test_different_app.apex"),
      "/data/app-staging/session_30", "staging_data_file");
  if (!installer.Prepare() || !installer2.Prepare()) {
    FAIL() << GetDebugStr(&installer) << GetDebugStr(&installer2);
  }

  ApexInfoList list;
  bool ret_value;
  std::vector<int> child_session_ids = {20, 30};
  ASSERT_TRUE(IsOk(
      service_->submitStagedSession(10, child_session_ids, &list, &ret_value)))
      << GetDebugStr(&installer);
  ASSERT_TRUE(ret_value);
  EXPECT_EQ(2u, list.apexInfos.size());
  ApexInfo match;
  bool package1_found = false;
  bool package2_found = false;
  for (const ApexInfo& info : list.apexInfos) {
    if (info.packageName == installer.package) {
      ASSERT_EQ(installer.package, info.packageName);
      ASSERT_EQ(installer.version, static_cast<uint64_t>(info.versionCode));
      ASSERT_EQ(installer.test_file, info.packagePath);
      package1_found = true;
    } else if (info.packageName == installer2.package) {
      ASSERT_EQ(installer2.package, info.packageName);
      ASSERT_EQ(installer2.version, static_cast<uint64_t>(info.versionCode));
      ASSERT_EQ(installer2.test_file, info.packagePath);
      package2_found = true;
    } else {
      FAIL() << "Unexpected package found " << info.packageName
             << GetDebugStr(&installer) << GetDebugStr(&installer2);
    }
  }
  ASSERT_TRUE(package1_found);
  ASSERT_TRUE(package2_found);

  ApexSessionInfo session;
  ASSERT_TRUE(IsOk(service_->getStagedSessionInfo(10, &session)))
      << GetDebugStr(&installer);
  ApexSessionInfo expected = CreateSessionInfo(10);
  expected.isVerified = true;
  ASSERT_THAT(session, SessionInfoEq(expected));

  ASSERT_TRUE(IsOk(service_->markStagedSessionReady(10, &ret_value)))
      << GetDebugStr(&installer);
  ASSERT_TRUE(ret_value);

  ASSERT_TRUE(IsOk(service_->getStagedSessionInfo(10, &session)))
      << GetDebugStr(&installer);
  expected.isVerified = false;
  expected.isStaged = true;
  ASSERT_THAT(session, SessionInfoEq(expected));
}

// TODO(jiyong): re-enable this test. This test is disabled because the build
// system now always bundles the public key that was used to sign the APEX.
// In debuggable build, the bundled public key is used as the last fallback.
// As a result, the verification is always successful (and thus test fails).
// In order to re-enable this test, we have to manually create an APEX
// where public key is not bundled.
#if 0
TEST_F(ApexServiceTest, SubmitMultiSessionTestFail) {
  // Parent session id: 11
  // Children session ids: 21 31
  PrepareTestApexForInstall installer(GetTestFile("apex.apexd_test.apex"),
                                      "/data/app-staging/session_21",
                                      "staging_data_file");
  PrepareTestApexForInstall installer2(
      GetTestFile("apex.apexd_test_no_inst_key.apex"),
      "/data/app-staging/session_31", "staging_data_file");
  if (!installer.Prepare() || !installer2.Prepare()) {
    FAIL() << GetDebugStr(&installer) << GetDebugStr(&installer2);
  }
  ApexInfoList list;
  bool ret_value;
  std::vector<int> child_session_ids = {21, 31};
  ASSERT_TRUE(IsOk(
      service_->submitStagedSession(11, child_session_ids, &list, &ret_value)))
      << GetDebugStr(&installer);
  ASSERT_FALSE(ret_value);
}
#endif

TEST_F(ApexServiceTest, MarkStagedSessionReadyFail) {
  // We should fail if we ask information about a session we don't know.
  bool ret_value;
  ASSERT_TRUE(IsOk(service_->markStagedSessionReady(666, &ret_value)));
  ASSERT_FALSE(ret_value);

  ApexSessionInfo session;
  ASSERT_TRUE(IsOk(service_->getStagedSessionInfo(666, &session)));
  ApexSessionInfo expected = CreateSessionInfo(-1);
  expected.isUnknown = true;
  ASSERT_THAT(session, SessionInfoEq(expected));
}

TEST_F(ApexServiceTest, MarkStagedSessionSuccessfulFailsNoSession) {
  ASSERT_FALSE(IsOk(service_->markStagedSessionSuccessful(37)));

  ApexSessionInfo session_info;
  ASSERT_TRUE(IsOk(service_->getStagedSessionInfo(37, &session_info)));
  ApexSessionInfo expected = CreateSessionInfo(-1);
  expected.isUnknown = true;
  ASSERT_THAT(session_info, SessionInfoEq(expected));
}

TEST_F(ApexServiceTest, MarkStagedSessionSuccessfulFailsSessionInWrongState) {
  auto session = ApexSession::CreateSession(73);
  ASSERT_TRUE(IsOk(session));
  ASSERT_TRUE(
      IsOk(session->UpdateStateAndCommit(::apex::proto::SessionState::STAGED)));

  ASSERT_FALSE(IsOk(service_->markStagedSessionSuccessful(73)));

  ApexSessionInfo session_info;
  ASSERT_TRUE(IsOk(service_->getStagedSessionInfo(73, &session_info)));
  ApexSessionInfo expected = CreateSessionInfo(73);
  expected.isStaged = true;
  ASSERT_THAT(session_info, SessionInfoEq(expected));
}

TEST_F(ApexServiceTest, MarkStagedSessionSuccessfulActivatedSession) {
  auto session = ApexSession::CreateSession(239);
  ASSERT_TRUE(IsOk(session));
  ASSERT_TRUE(IsOk(
      session->UpdateStateAndCommit(::apex::proto::SessionState::ACTIVATED)));

  ASSERT_TRUE(IsOk(service_->markStagedSessionSuccessful(239)));

  ApexSessionInfo session_info;
  ASSERT_TRUE(IsOk(service_->getStagedSessionInfo(239, &session_info)));
  ApexSessionInfo expected = CreateSessionInfo(239);
  expected.isSuccess = true;
  ASSERT_THAT(session_info, SessionInfoEq(expected));
}

TEST_F(ApexServiceTest, MarkStagedSessionSuccessfulNoOp) {
  auto session = ApexSession::CreateSession(1543);
  ASSERT_TRUE(IsOk(session));
  ASSERT_TRUE(IsOk(
      session->UpdateStateAndCommit(::apex::proto::SessionState::SUCCESS)));

  ASSERT_TRUE(IsOk(service_->markStagedSessionSuccessful(1543)));

  ApexSessionInfo session_info;
  ASSERT_TRUE(IsOk(service_->getStagedSessionInfo(1543, &session_info)));
  ApexSessionInfo expected = CreateSessionInfo(1543);
  expected.isSuccess = true;
  ASSERT_THAT(session_info, SessionInfoEq(expected));
}

TEST_F(ApexServiceTest, AbortActiveSessionNoSessions) {
  // First ensure there are no sessions.
  std::vector<ApexSessionInfo> sessions;
  ASSERT_TRUE(IsOk(service_->getSessions(&sessions)));
  ASSERT_EQ(0u, sessions.size());
  ASSERT_TRUE(IsOk(service_->abortActiveSession()));
}

TEST_F(ApexServiceTest, AbortActiveSession) {
  auto session = ApexSession::CreateSession(239);
  session->UpdateStateAndCommit(SessionState::VERIFIED);

  std::vector<ApexSessionInfo> sessions;
  ASSERT_TRUE(IsOk(service_->getSessions(&sessions)));
  ASSERT_EQ(1u, sessions.size());

  ASSERT_TRUE(IsOk(service_->abortActiveSession()));

  sessions.clear();
  ASSERT_TRUE(IsOk(service_->getSessions(&sessions)));
  ASSERT_EQ(0u, sessions.size());
}

TEST_F(ApexServiceTest, BackupActivePackages) {
  if (supports_fs_checkpointing_) {
    GTEST_SKIP() << "Can't run if filesystem checkpointing is enabled";
  }
  PrepareTestApexForInstall installer1(GetTestFile("apex.apexd_test.apex"));
  PrepareTestApexForInstall installer2(
      GetTestFile("apex.apexd_test_different_app.apex"));
  PrepareTestApexForInstall installer3(GetTestFile("apex.apexd_test_v2.apex"),
                                       "/data/app-staging/session_23",
                                       "staging_data_file");

  if (!installer1.Prepare() || !installer2.Prepare() || !installer3.Prepare()) {
    return;
  }

  // Activate some packages, in order to backup them later.
  bool ret = false;
  std::vector<std::string> pkgs = {installer1.test_file, installer2.test_file};
  ASSERT_TRUE(IsOk(service_->stagePackages(pkgs, &ret)));
  ASSERT_TRUE(ret);

  // Make sure that /data/apex/active has activated packages.
  auto active_pkgs =
      ReadDir(kActiveApexPackagesDataDir, [](auto _) { return true; });
  ASSERT_TRUE(IsOk(active_pkgs));
  ASSERT_THAT(*active_pkgs,
              UnorderedElementsAre(installer1.test_installed_file,
                                   installer2.test_installed_file));

  ApexInfoList list;
  std::vector<int> empty_child_session_ids;
  ASSERT_TRUE(IsOk(
      service_->submitStagedSession(23, empty_child_session_ids, &list, &ret)));
  ASSERT_TRUE(ret);

  auto backups = ReadDir(kApexBackupDir, [](auto _) { return true; });
  ASSERT_TRUE(IsOk(backups));
  auto backup1 =
      StringPrintf("%s/com.android.apex.test_package@1.apex", kApexBackupDir);
  auto backup2 =
      StringPrintf("%s/com.android.apex.test_package_2@1.apex", kApexBackupDir);
  ASSERT_THAT(*backups, UnorderedElementsAre(backup1, backup2));
}

TEST_F(ApexServiceTest, BackupActivePackagesClearsPreviousBackup) {
  if (supports_fs_checkpointing_) {
    GTEST_SKIP() << "Can't run if filesystem checkpointing is enabled";
  }
  PrepareTestApexForInstall installer1(GetTestFile("apex.apexd_test.apex"));
  PrepareTestApexForInstall installer2(
      GetTestFile("apex.apexd_test_different_app.apex"));
  PrepareTestApexForInstall installer3(GetTestFile("apex.apexd_test_v2.apex"),
                                       "/data/app-staging/session_43",
                                       "staging_data_file");

  if (!installer1.Prepare() || !installer2.Prepare() || !installer3.Prepare()) {
    return;
  }

  // Make sure /data/apex/backups exists.
  ASSERT_TRUE(IsOk(createDirIfNeeded(std::string(kApexBackupDir), 0700)));
  // Create some bogus files in /data/apex/backups.
  std::ofstream old_backup(StringPrintf("%s/file1", kApexBackupDir));
  ASSERT_TRUE(old_backup.good());
  old_backup.close();

  bool ret = false;
  std::vector<std::string> pkgs = {installer1.test_file, installer2.test_file};
  ASSERT_TRUE(IsOk(service_->stagePackages(pkgs, &ret)));
  ASSERT_TRUE(ret);

  // Make sure that /data/apex/active has activated packages.
  auto active_pkgs =
      ReadDir(kActiveApexPackagesDataDir, [](auto _) { return true; });
  ASSERT_TRUE(IsOk(active_pkgs));
  ASSERT_THAT(*active_pkgs,
              UnorderedElementsAre(installer1.test_installed_file,
                                   installer2.test_installed_file));

  ApexInfoList list;
  std::vector<int> empty_child_session_ids;
  ASSERT_TRUE(IsOk(
      service_->submitStagedSession(43, empty_child_session_ids, &list, &ret)));
  ASSERT_TRUE(ret);

  auto backups = ReadDir(kApexBackupDir, [](auto _) { return true; });
  ASSERT_TRUE(IsOk(backups));
  auto backup1 =
      StringPrintf("%s/com.android.apex.test_package@1.apex", kApexBackupDir);
  auto backup2 =
      StringPrintf("%s/com.android.apex.test_package_2@1.apex", kApexBackupDir);
  ASSERT_THAT(*backups, UnorderedElementsAre(backup1, backup2));
}

TEST_F(ApexServiceTest, BackupActivePackagesZeroActivePackages) {
  if (supports_fs_checkpointing_) {
    GTEST_SKIP() << "Can't run if filesystem checkpointing is enabled";
  }
  PrepareTestApexForInstall installer(GetTestFile("apex.apexd_test_v2.apex"),
                                      "/data/app-staging/session_41",
                                      "staging_data_file");

  if (!installer.Prepare()) {
    return;
  }

  // Make sure that /data/apex/active exists and is empty
  ASSERT_TRUE(
      IsOk(createDirIfNeeded(std::string(kActiveApexPackagesDataDir), 0750)));
  auto active_pkgs =
      ReadDir(kActiveApexPackagesDataDir, [](auto _) { return true; });
  ASSERT_TRUE(IsOk(active_pkgs));
  ASSERT_EQ(0u, active_pkgs->size());

  ApexInfoList list;
  std::vector<int> empty_child_session_ids;
  bool ret = false;
  ASSERT_TRUE(IsOk(
      service_->submitStagedSession(41, empty_child_session_ids, &list, &ret)));
  ASSERT_TRUE(ret);

  auto backups = ReadDir(kApexBackupDir, [](auto _) { return true; });
  ASSERT_TRUE(IsOk(backups));
  ASSERT_EQ(0u, backups->size());
}

TEST_F(ApexServiceTest, ActivePackagesFolderDoesNotExist) {
  PrepareTestApexForInstall installer(GetTestFile("apex.apexd_test_v2.apex"),
                                      "/data/app-staging/session_41",
                                      "staging_data_file");

  if (!installer.Prepare()) {
    return;
  }

  // Make sure that /data/apex/active does not exist
  std::error_code ec;
  fs::remove_all(fs::path(kActiveApexPackagesDataDir), ec);
  ASSERT_FALSE(ec) << "Failed to delete " << kActiveApexPackagesDataDir;

  ApexInfoList list;
  std::vector<int> empty_child_session_ids;
  bool ret = false;
  ASSERT_TRUE(IsOk(
      service_->submitStagedSession(41, empty_child_session_ids, &list, &ret)));
  ASSERT_TRUE(ret);

  if (!supports_fs_checkpointing_) {
    auto backups = ReadDir(kApexBackupDir, [](auto _) { return true; });
    ASSERT_TRUE(IsOk(backups));
    ASSERT_EQ(0u, backups->size());
  }
}

TEST_F(ApexServiceTest, UnstagePackagesSuccess) {
  PrepareTestApexForInstall installer1(GetTestFile("apex.apexd_test.apex"));
  PrepareTestApexForInstall installer2(
      GetTestFile("apex.apexd_test_different_app.apex"));

  if (!installer1.Prepare() || !installer2.Prepare()) {
    return;
  }

  bool ret = false;
  std::vector<std::string> pkgs = {installer1.test_file, installer2.test_file};
  ASSERT_TRUE(IsOk(service_->stagePackages(pkgs, &ret)));
  ASSERT_TRUE(ret);

  pkgs = {installer2.test_installed_file};
  ASSERT_TRUE(IsOk(service_->unstagePackages(pkgs)));

  auto active_packages =
      ReadDir(kActiveApexPackagesDataDir, [](auto _) { return true; });
  ASSERT_TRUE(IsOk(active_packages));
  ASSERT_THAT(*active_packages,
              UnorderedElementsAre(installer1.test_installed_file));
}

TEST_F(ApexServiceTest, UnstagePackagesFail) {
  PrepareTestApexForInstall installer1(GetTestFile("apex.apexd_test.apex"));
  PrepareTestApexForInstall installer2(
      GetTestFile("apex.apexd_test_different_app.apex"));

  if (!installer1.Prepare() || !installer2.Prepare()) {
    return;
  }

  bool ret = false;
  std::vector<std::string> pkgs = {installer1.test_file};
  ASSERT_TRUE(IsOk(service_->stagePackages(pkgs, &ret)));
  ASSERT_TRUE(ret);

  pkgs = {installer1.test_installed_file, installer2.test_installed_file};
  ASSERT_FALSE(IsOk(service_->unstagePackages(pkgs)));

  // Check that first package wasn't unstaged.
  auto active_packages =
      ReadDir(kActiveApexPackagesDataDir, [](auto _) { return true; });
  ASSERT_TRUE(IsOk(active_packages));
  ASSERT_THAT(*active_packages,
              UnorderedElementsAre(installer1.test_installed_file));
}

class ApexServiceRollbackTest : public ApexServiceTest {
 protected:
  void SetUp() override { ApexServiceTest::SetUp(); }

  void PrepareBackup(const std::vector<std::string>& pkgs) {
    ASSERT_TRUE(IsOk(createDirIfNeeded(std::string(kApexBackupDir), 0700)));
    for (const auto& pkg : pkgs) {
      PrepareTestApexForInstall installer(pkg);
      ASSERT_TRUE(installer.Prepare()) << " failed to prepare " << pkg;
      const std::string& from = installer.test_file;
      std::string to = std::string(kApexBackupDir) + "/" + installer.package +
                       "@" + std::to_string(installer.version) + ".apex";
      std::error_code ec;
      fs::copy(fs::path(from), fs::path(to),
               fs::copy_options::create_hard_links, ec);
      ASSERT_FALSE(ec) << "Failed to copy " << from << " to " << to << " : "
                       << ec;
    }
  }

  void CheckRollbackWasPerformed(
      const std::vector<std::string>& expected_pkgs) {
    // First check that /data/apex/active exists and has correct permissions.
    struct stat sd;
    ASSERT_EQ(0, stat(kActiveApexPackagesDataDir, &sd));
    ASSERT_EQ(0750u, sd.st_mode & ALLPERMS);

    // Now read content and check it contains expected values.
    auto active_pkgs =
        ReadDir(kActiveApexPackagesDataDir, [](auto _) { return true; });
    ASSERT_TRUE(IsOk(active_pkgs));
    ASSERT_THAT(*active_pkgs, UnorderedElementsAreArray(expected_pkgs));
  }
};

TEST_F(ApexServiceRollbackTest, AbortActiveSessionSuccessfulRollback) {
  if (supports_fs_checkpointing_) {
    GTEST_SKIP() << "Can't run if filesystem checkpointing is enabled";
  }
  PrepareTestApexForInstall installer(GetTestFile("apex.apexd_test_v2.apex"));
  if (!installer.Prepare()) {
    return;
  }

  auto session = ApexSession::CreateSession(239);
  ASSERT_TRUE(IsOk(session));
  ASSERT_TRUE(IsOk(session->UpdateStateAndCommit(SessionState::ACTIVATED)));

  // Make sure /data/apex/active is non-empty.
  bool ret;
  ASSERT_TRUE(IsOk(service_->stagePackage(installer.test_file, &ret)));
  ASSERT_TRUE(ret);

  PrepareBackup({GetTestFile("apex.apexd_test.apex"),
                 GetTestFile("apex.apexd_test_different_app.apex")});

  ASSERT_TRUE(IsOk(service_->abortActiveSession()));

  auto pkg1 = StringPrintf("%s/com.android.apex.test_package@1.apex",
                           kActiveApexPackagesDataDir);
  auto pkg2 = StringPrintf("%s/com.android.apex.test_package_2@1.apex",
                           kActiveApexPackagesDataDir);
  SCOPED_TRACE("");
  CheckRollbackWasPerformed({pkg1, pkg2});
  std::vector<ApexSessionInfo> sessions;
  ASSERT_TRUE(IsOk(service_->getSessions(&sessions)));
  ApexSessionInfo expected = CreateSessionInfo(239);
  expected.isRolledBack = true;
  ASSERT_THAT(sessions, UnorderedElementsAre(SessionInfoEq(expected)));
}

TEST_F(ApexServiceRollbackTest, RollbackLastSessionCalledSuccessfulRollback) {
  if (supports_fs_checkpointing_) {
    GTEST_SKIP() << "Can't run if filesystem checkpointing is enabled";
  }

  PrepareTestApexForInstall installer(GetTestFile("apex.apexd_test_v2.apex"));
  if (!installer.Prepare()) {
    return;
  }

  auto session = ApexSession::CreateSession(1543);
  ASSERT_TRUE(IsOk(session));
  ASSERT_TRUE(IsOk(session->UpdateStateAndCommit(SessionState::ACTIVATED)));

  // Make sure /data/apex/active is non-empty.
  bool ret;
  ASSERT_TRUE(IsOk(service_->stagePackage(installer.test_file, &ret)));
  ASSERT_TRUE(ret);

  PrepareBackup({GetTestFile("apex.apexd_test.apex")});

  ASSERT_TRUE(IsOk(service_->rollbackActiveSession()));

  auto pkg = StringPrintf("%s/com.android.apex.test_package@1.apex",
                          kActiveApexPackagesDataDir);
  SCOPED_TRACE("");
  CheckRollbackWasPerformed({pkg});
}

TEST_F(ApexServiceRollbackTest, RollbackLastSessionCalledNoActiveSession) {
  // This test simulates a situation that should never happen on user builds:
  // abortLastSession was called, but there are no active sessions.
  PrepareTestApexForInstall installer(GetTestFile("apex.apexd_test_v2.apex"));
  if (!installer.Prepare()) {
    return;
  }

  // Make sure /data/apex/active is non-empty.
  bool ret;
  ASSERT_TRUE(IsOk(service_->stagePackage(installer.test_file, &ret)));
  ASSERT_TRUE(ret);

  PrepareBackup({GetTestFile("apex.apexd_test.apex")});

  // Even though backup is there, no sessions are active, hence rollback request
  // should fail.
  ASSERT_FALSE(IsOk(service_->rollbackActiveSession()));
}

TEST_F(ApexServiceRollbackTest, RollbackFailsNoBackupFolder) {
  ASSERT_FALSE(IsOk(service_->rollbackActiveSession()));
}

TEST_F(ApexServiceRollbackTest, RollbackFailsNoActivePackagesFolder) {
  PrepareTestApexForInstall installer(GetTestFile("apex.apexd_test.apex"));
  ASSERT_FALSE(IsOk(service_->rollbackActiveSession()));
}

TEST_F(ApexServiceRollbackTest, MarkStagedSessionSuccessfulCleanupBackup) {
  PrepareBackup({GetTestFile("apex.apexd_test.apex"),
                 GetTestFile("apex.apexd_test_different_app.apex")});

  auto session = ApexSession::CreateSession(101);
  ASSERT_TRUE(IsOk(session));
  ASSERT_TRUE(IsOk(session->UpdateStateAndCommit(SessionState::ACTIVATED)));

  ASSERT_TRUE(IsOk(service_->markStagedSessionSuccessful(101)));

  ASSERT_TRUE(fs::is_empty(fs::path(kApexBackupDir)));
}

TEST_F(ApexServiceRollbackTest, ResumesRollback) {
  if (supports_fs_checkpointing_) {
    GTEST_SKIP() << "Can't run if filesystem checkpointing is enabled";
  }
  PrepareBackup({GetTestFile("apex.apexd_test.apex"),
                 GetTestFile("apex.apexd_test_different_app.apex")});

  PrepareTestApexForInstall installer(GetTestFile("apex.apexd_test_v2.apex"));
  if (!installer.Prepare()) {
    return;
  }

  // Make sure /data/apex/active is non-empty.
  bool ret;
  ASSERT_TRUE(IsOk(service_->stagePackage(installer.test_file, &ret)));
  ASSERT_TRUE(ret);

  auto session = ApexSession::CreateSession(17239);
  ASSERT_TRUE(IsOk(session));
  ASSERT_TRUE(
      IsOk(session->UpdateStateAndCommit(SessionState::ROLLBACK_IN_PROGRESS)));

  ASSERT_TRUE(IsOk(service_->resumeRollbackIfNeeded()));

  auto pkg1 = StringPrintf("%s/com.android.apex.test_package@1.apex",
                           kActiveApexPackagesDataDir);
  auto pkg2 = StringPrintf("%s/com.android.apex.test_package_2@1.apex",
                           kActiveApexPackagesDataDir);
  SCOPED_TRACE("");
  CheckRollbackWasPerformed({pkg1, pkg2});

  std::vector<ApexSessionInfo> sessions;
  ASSERT_TRUE(IsOk(service_->getSessions(&sessions)));
  ApexSessionInfo expected = CreateSessionInfo(17239);
  expected.isRolledBack = true;
  ASSERT_THAT(sessions, UnorderedElementsAre(SessionInfoEq(expected)));
}

TEST_F(ApexServiceRollbackTest, DoesNotResumeRollback) {
  if (supports_fs_checkpointing_) {
    GTEST_SKIP() << "Can't run if filesystem checkpointing is enabled";
  }
  PrepareTestApexForInstall installer(GetTestFile("apex.apexd_test_v2.apex"));
  if (!installer.Prepare()) {
    return;
  }

  // Make sure /data/apex/active is non-empty.
  bool ret;
  ASSERT_TRUE(IsOk(service_->stagePackage(installer.test_file, &ret)));
  ASSERT_TRUE(ret);

  auto session = ApexSession::CreateSession(53);
  ASSERT_TRUE(IsOk(session));
  ASSERT_TRUE(IsOk(session->UpdateStateAndCommit(SessionState::SUCCESS)));

  ASSERT_TRUE(IsOk(service_->resumeRollbackIfNeeded()));

  // Check that rollback wasn't resumed.
  auto active_pkgs =
      ReadDir(kActiveApexPackagesDataDir, [](auto _) { return true; });
  ASSERT_TRUE(IsOk(active_pkgs));
  ASSERT_THAT(*active_pkgs,
              UnorderedElementsAre(installer.test_installed_file));

  std::vector<ApexSessionInfo> sessions;
  ASSERT_TRUE(IsOk(service_->getSessions(&sessions)));
  ApexSessionInfo expected = CreateSessionInfo(53);
  expected.isSuccess = true;
  ASSERT_THAT(sessions, UnorderedElementsAre(SessionInfoEq(expected)));
}

TEST_F(ApexServiceRollbackTest, FailsRollback) {
  if (supports_fs_checkpointing_) {
    GTEST_SKIP() << "Can't run if filesystem checkpointing is enabled";
  }

  auto session = ApexSession::CreateSession(53);
  ASSERT_TRUE(IsOk(session));
  ASSERT_TRUE(IsOk(session->UpdateStateAndCommit(SessionState::ACTIVATED)));

  ASSERT_FALSE(IsOk(service_->rollbackActiveSession()));
  ApexSessionInfo session_info;
  ASSERT_TRUE(IsOk(service_->getStagedSessionInfo(53, &session_info)));
  ApexSessionInfo expected = CreateSessionInfo(53);
  expected.isRollbackFailed = true;
  ASSERT_THAT(session_info, SessionInfoEq(expected));
}

TEST_F(ApexServiceRollbackTest, RollbackFailedStateRollbackAttemptFails) {
  if (supports_fs_checkpointing_) {
    GTEST_SKIP() << "Can't run if filesystem checkpointing is enabled";
  }

  auto session = ApexSession::CreateSession(17239);
  ASSERT_TRUE(IsOk(session));
  ASSERT_TRUE(
      IsOk(session->UpdateStateAndCommit(SessionState::ROLLBACK_FAILED)));

  ASSERT_FALSE(IsOk(service_->rollbackActiveSession()));
  ApexSessionInfo session_info;
  ASSERT_TRUE(IsOk(service_->getStagedSessionInfo(17239, &session_info)));
  ApexSessionInfo expected = CreateSessionInfo(17239);
  expected.isRollbackFailed = true;
  ASSERT_THAT(session_info, SessionInfoEq(expected));
}

static pid_t GetPidOf(const std::string& name) {
  char buf[1024];
  const std::string cmd = std::string("pidof -s ") + name;
  FILE* cmd_pipe = popen(cmd.c_str(), "r");
  if (cmd_pipe == nullptr) {
    PLOG(ERROR) << "Cannot open pipe for " << cmd;
    return 0;
  }
  if (fgets(buf, 1024, cmd_pipe) == nullptr) {
    PLOG(ERROR) << "Cannot read pipe for " << cmd;
    pclose(cmd_pipe);
    return 0;
  }

  pclose(cmd_pipe);
  return strtoul(buf, nullptr, 10);
}

static void ExecInMountNamespaceOf(pid_t pid,
                                   const std::function<void(pid_t)>& func) {
  const std::string my_path = "/proc/self/ns/mnt";
  android::base::unique_fd my_fd(open(my_path.c_str(), O_RDONLY | O_CLOEXEC));
  ASSERT_TRUE(my_fd.get() >= 0);

  const std::string target_path =
      std::string("/proc/") + std::to_string(pid) + "/ns/mnt";
  android::base::unique_fd target_fd(
      open(target_path.c_str(), O_RDONLY | O_CLOEXEC));
  ASSERT_TRUE(target_fd.get() >= 0);

  int res = setns(target_fd.get(), CLONE_NEWNS);
  ASSERT_NE(-1, res);

  func(pid);

  res = setns(my_fd.get(), CLONE_NEWNS);
  ASSERT_NE(-1, res);
}

TEST(ApexdTest, ApexdIsInSameMountNamespaceAsInit) {
  std::string ns_apexd;
  std::string ns_init;

  ExecInMountNamespaceOf(GetPidOf("apexd"), [&](pid_t pid) {
    bool res = android::base::Readlink("/proc/self/ns/mnt", &ns_apexd);
    ASSERT_TRUE(res);
  });

  ExecInMountNamespaceOf(1, [&](pid_t pid) {
    bool res = android::base::Readlink("/proc/self/ns/mnt", &ns_init);
    ASSERT_TRUE(res);
  });

  ASSERT_EQ(ns_apexd, ns_init);
}

// These are NOT exhaustive list of early processes be should be enough
static const std::vector<const std::string> kEarlyProcesses = {
    "servicemanager",
    "hwservicemanager",
    "vold",
    "logd",
};

TEST(ApexdTest, EarlyProcessesAreInDifferentMountNamespace) {
  if (!android::base::GetBoolProperty("ro.apex.updatable", false)) {
    return;
  }

  std::string ns_apexd;

  ExecInMountNamespaceOf(GetPidOf("apexd"), [&](pid_t _) {
    bool res = android::base::Readlink("/proc/self/ns/mnt", &ns_apexd);
    ASSERT_TRUE(res);
  });

  for (const auto& name : kEarlyProcesses) {
    std::string ns_early_process;
    ExecInMountNamespaceOf(GetPidOf(name), [&](pid_t _) {
      bool res =
          android::base::Readlink("/proc/self/ns/mnt", &ns_early_process);
      ASSERT_TRUE(res);
    });
    ASSERT_NE(ns_apexd, ns_early_process);
  }
}

TEST(ApexdTest, ApexIsAPrivateMountPoint) {
  std::string mountinfo;
  ASSERT_TRUE(
      android::base::ReadFileToString("/proc/self/mountinfo", &mountinfo));
  bool found_apex_mountpoint = false;
  for (const auto& line : android::base::Split(mountinfo, "\n")) {
    std::vector<std::string> tokens = android::base::Split(line, " ");
    // line format:
    // mnt_id parent_mnt_id major:minor source target option propagation_type
    // ex) 33 260:19 / /apex rw,nosuid,nodev -
    if (tokens.size() >= 7 && tokens[4] == "/apex") {
      found_apex_mountpoint = true;
      // Make sure that propagation type is set to - which means private
      ASSERT_EQ("-", tokens[6]);
    }
  }
  ASSERT_TRUE(found_apex_mountpoint);
}

static const std::vector<const std::string> kEarlyApexes = {
    "/apex/com.android.runtime",
    "/apex/com.android.tzdata",
};

TEST(ApexdTest, ApexesAreActivatedForEarlyProcesses) {
  for (const auto& name : kEarlyProcesses) {
    pid_t pid = GetPidOf(name);
    const std::string path =
        std::string("/proc/") + std::to_string(pid) + "/mountinfo";
    std::string mountinfo;
    ASSERT_TRUE(android::base::ReadFileToString(path.c_str(), &mountinfo));

    std::unordered_set<std::string> mountpoints;
    for (const auto& line : android::base::Split(mountinfo, "\n")) {
      std::vector<std::string> tokens = android::base::Split(line, " ");
      // line format:
      // mnt_id parent_mnt_id major:minor source target option propagation_type
      // ex) 69 33 7:40 / /apex/com.android.conscrypt ro,nodev,noatime -
      if (tokens.size() >= 5) {
        // token[4] is the target mount point
        mountpoints.emplace(tokens[4]);
      }
    }
    for (const auto& apex_name : kEarlyApexes) {
      ASSERT_NE(mountpoints.end(), mountpoints.find(apex_name));
    }
  }
}

class ApexShimUpdateTest : public ApexServiceTest {
 protected:
  void SetUp() override {
    ApexServiceTest::SetUp();

    // Assert that shim apex is pre-installed.
    std::vector<ApexInfo> list;
    ASSERT_TRUE(IsOk(service_->getAllPackages(&list)));
    ApexInfo expected;
    expected.packageName = "com.android.apex.cts.shim";
    expected.packagePath = "/system/apex/com.android.apex.cts.shim.apex";
    expected.versionCode = 1;
    expected.isFactory = true;
    expected.isActive = true;
    ASSERT_THAT(list, Contains(ApexInfoEq(expected)));
  }
};

TEST_F(ApexShimUpdateTest, UpdateToV2Success) {
  PrepareTestApexForInstall installer(
      GetTestFile("com.android.apex.cts.shim.v2.apex"));

  if (!installer.Prepare()) {
    FAIL() << GetDebugStr(&installer);
  }

  bool success;
  ASSERT_TRUE(IsOk(service_->stagePackage(installer.test_file, &success)));
  ASSERT_TRUE(success);
}

TEST_F(ApexShimUpdateTest, UpdateToV2FailureWrongSHA512) {
  PrepareTestApexForInstall installer(
      GetTestFile("com.android.apex.cts.shim.v2_wrong_sha.apex"));

  if (!installer.Prepare()) {
    FAIL() << GetDebugStr(&installer);
  }

  bool success;
  const auto& status = service_->stagePackage(installer.test_file, &success);
  ASSERT_FALSE(IsOk(status));
  const std::string& error_message =
      std::string(status.exceptionMessage().c_str());
  ASSERT_THAT(error_message, HasSubstr("has unexpected SHA512 hash"));
}

TEST_F(ApexShimUpdateTest, SubmitStagedSesssionFailureHasPreInstallHook) {
  PrepareTestApexForInstall installer(
      GetTestFile("com.android.apex.cts.shim.v2_with_pre_install_hook.apex"),
      "/data/app-staging/session_23", "staging_data_file");

  if (!installer.Prepare()) {
    FAIL() << GetDebugStr(&installer);
  }

  ApexInfoList list;
  bool success;
  ASSERT_TRUE(IsOk(service_->submitStagedSession(23, {}, &list, &success)));
  ASSERT_FALSE(success);
}

TEST_F(ApexShimUpdateTest, SubmitStagedSessionFailureHasPostInstallHook) {
  PrepareTestApexForInstall installer(
      GetTestFile("com.android.apex.cts.shim.v2_with_post_install_hook.apex"),
      "/data/app-staging/session_43", "staging_data_file");

  if (!installer.Prepare()) {
    FAIL() << GetDebugStr(&installer);
  }

  ApexInfoList list;
  bool success;
  ASSERT_TRUE(IsOk(service_->submitStagedSession(43, {}, &list, &success)));
  ASSERT_FALSE(success);
}

TEST_F(ApexShimUpdateTest, SubmitStagedSessionFailureAdditionalFile) {
  PrepareTestApexForInstall installer(
      GetTestFile("com.android.apex.cts.shim.v2_additional_file.apex"),
      "/data/app-staging/session_41", "staging_data_file");
  if (!installer.Prepare()) {
    FAIL() << GetDebugStr(&installer);
  }

  ApexInfoList list;
  bool success;
  ASSERT_TRUE(IsOk(service_->submitStagedSession(41, {}, &list, &success)));
  ASSERT_FALSE(success);
}

TEST_F(ApexShimUpdateTest, SubmitStagedSessionFailureAdditionalFolder) {
  PrepareTestApexForInstall installer(
      GetTestFile("com.android.apex.cts.shim.v2_additional_folder.apex"),
      "/data/app-staging/session_42", "staging_data_file");
  if (!installer.Prepare()) {
    FAIL() << GetDebugStr(&installer);
  }

  ApexInfoList list;
  bool success;
  ASSERT_TRUE(IsOk(service_->submitStagedSession(42, {}, &list, &success)));
  ASSERT_FALSE(success);
}

TEST_F(ApexServiceTest, SubmitStagedSessionCorruptApexFails) {
  PrepareTestApexForInstall installer(
      GetTestFile("apex.apexd_test_corrupt_apex.apex"),
      "/data/app-staging/session_57", "staging_data_file");

  if (!installer.Prepare()) {
    FAIL() << GetDebugStr(&installer);
  }

  ApexInfoList list;
  bool success;
  ASSERT_TRUE(IsOk(service_->submitStagedSession(57, {}, &list, &success)));
  ASSERT_FALSE(success);
}

// Following test case piggybacks on logic in ApexServiceActivationSuccessTest
// in order to use mounted apex as flattened one.
TEST_F(ApexServiceActivationSuccessTest, StageFailsFlattenedApex) {
  ASSERT_TRUE(IsOk(service_->activatePackage(installer_->test_installed_file)))
      << GetDebugStr(installer_.get());

  StatusOr<ApexFile> flattened_apex =
      ApexFile::Open(StringPrintf("/apex/%s", installer_->package.c_str()));
  ASSERT_TRUE(IsOk(flattened_apex));
  ASSERT_TRUE(flattened_apex->IsFlattened());

  bool success;
  const auto& status =
      service_->stagePackage(flattened_apex->GetPath(), &success);
  ASSERT_FALSE(IsOk(status));
  const std::string& error_message =
      std::string(status.exceptionMessage().c_str());
  ASSERT_THAT(error_message, HasSubstr("Can't upgrade flattened apex"));
}

class LogTestToLogcat : public ::testing::EmptyTestEventListener {
  void OnTestStart(const ::testing::TestInfo& test_info) override {
#ifdef __ANDROID__
    using base::LogId;
    using base::LogSeverity;
    using base::StringPrintf;
    base::LogdLogger l;
    std::string msg =
        StringPrintf("=== %s::%s (%s:%d)", test_info.test_case_name(),
                     test_info.name(), test_info.file(), test_info.line());
    l(LogId::MAIN, LogSeverity::INFO, "apexservice_test", __FILE__, __LINE__,
      msg.c_str());
#else
    UNUSED(test_info);
#endif
  }
};

}  // namespace apex
}  // namespace android

int main(int argc, char** argv) {
  android::base::InitLogging(argv, &android::base::StderrLogger);
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::UnitTest::GetInstance()->listeners().Append(
      new android::apex::LogTestToLogcat());
  return RUN_ALL_TESTS();
}
