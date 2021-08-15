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

#define LOG_TAG "apexd"

#include "apexd.h"
#include "apexd_private.h"

#include "apex_database.h"
#include "apex_file.h"
#include "apex_key.h"
#include "apex_manifest.h"
#include "apex_shim.h"
#include "apexd_checkpoint.h"
#include "apexd_loop.h"
#include "apexd_prepostinstall.h"
#include "apexd_prop.h"
#include "apexd_session.h"
#include "apexd_utils.h"
#include "status_or.h"
#include "string_log.h"

#include <ApexProperties.sysprop.h>
#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/macros.h>
#include <android-base/properties.h>
#include <android-base/scopeguard.h>
#include <android-base/stringprintf.h>
#include <android-base/strings.h>
#include <android-base/unique_fd.h>
#include <libavb/libavb.h>
#include <libdm/dm.h>
#include <libdm/dm_table.h>
#include <libdm/dm_target.h>
#include <selinux/android.h>

#include <dirent.h>
#include <fcntl.h>
#include <linux/loop.h>
#include <sys/ioctl.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>
#include <array>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>

using android::base::EndsWith;
using android::base::Join;
using android::base::ReadFullyAtOffset;
using android::base::StartsWith;
using android::base::StringPrintf;
using android::base::unique_fd;
using android::dm::DeviceMapper;
using android::dm::DmDeviceState;
using android::dm::DmTable;
using android::dm::DmTargetVerity;

using apex::proto::SessionState;

namespace android {
namespace apex {

using MountedApexData = MountedApexDatabase::MountedApexData;

namespace {

// These should be in-sync with system/sepolicy/public/property_contexts
static constexpr const char* kApexStatusSysprop = "apexd.status";
static constexpr const char* kApexStatusStarting = "starting";
static constexpr const char* kApexStatusReady = "ready";

static constexpr const char* kBuildFingerprintSysprop = "ro.build.fingerprint";

static constexpr const char* kApexVerityOnSystemProp =
    "persist.apexd.verity_on_system";
static bool gForceDmVerityOnSystem =
    android::base::GetBoolProperty(kApexVerityOnSystemProp, false);

// This should be in UAPI, but it's not :-(
static constexpr const char* kDmVerityRestartOnCorruption =
    "restart_on_corruption";

MountedApexDatabase gMountedApexes;

CheckpointInterface* gVoldService;
bool gSupportsFsCheckpoints = false;
bool gInFsCheckpointMode = false;

static constexpr size_t kLoopDeviceSetupAttempts = 3u;

static const bool kUpdatable =
    android::sysprop::ApexProperties::updatable().value_or(false);

bool gBootstrap = false;
static const std::vector<const std::string> kBootstrapApexes = {
    "com.android.runtime",
    "com.android.tzdata",
};

static constexpr const int kNumRetriesWhenCheckpointingEnabled = 1;

bool isBootstrapApex(const ApexFile& apex) {
  return std::find(kBootstrapApexes.begin(), kBootstrapApexes.end(),
                   apex.GetManifest().name()) != kBootstrapApexes.end();
}

// Pre-allocate loop devices so that we don't have to wait for them
// later when actually activating APEXes.
Status preAllocateLoopDevices() {
  auto scan = FindApexes(kApexPackageBuiltinDirs);
  if (!scan.Ok()) {
    return scan.ErrorStatus();
  }

  auto size = 0;
  for (const auto& path : *scan) {
    auto apexFile = ApexFile::Open(path);
    if (!apexFile.Ok() || apexFile->IsFlattened()) {
      continue;
    }
    size++;
    // bootstrap Apexes may be activated on separate namespaces.
    if (isBootstrapApex(*apexFile)) {
      size++;
    }
  }

  // note: do not call preAllocateLoopDevices() if size == 0
  // or the device does not support updatable APEX.
  // For devices (e.g. ARC) which doesn't support loop-control
  // preAllocateLoopDevices() can cause problem when it tries
  // to access /dev/loop-control.
  if (size == 0 || !kUpdatable) {
    return Status::Success();
  }
  return loop::preAllocateLoopDevices(size);
}

std::unique_ptr<DmTable> createVerityTable(const ApexVerityData& verity_data,
                                           const std::string& loop,
                                           bool restart_on_corruption) {
  AvbHashtreeDescriptor* desc = verity_data.desc.get();
  auto table = std::make_unique<DmTable>();

  std::ostringstream hash_algorithm;
  hash_algorithm << desc->hash_algorithm;

  auto target = std::make_unique<DmTargetVerity>(
      0, desc->image_size / 512, desc->dm_verity_version, loop, loop,
      desc->data_block_size, desc->hash_block_size,
      desc->image_size / desc->data_block_size,
      desc->tree_offset / desc->hash_block_size, hash_algorithm.str(),
      verity_data.root_digest, verity_data.salt);

  target->IgnoreZeroBlocks();
  if (restart_on_corruption) {
    target->SetVerityMode(kDmVerityRestartOnCorruption);
  }
  table->AddTarget(std::move(target));

  table->set_readonly(true);

  return table;
}

enum WaitForDeviceMode {
  kWaitToBeCreated = 0,
  kWaitToBeDeleted,
};

Status waitForDevice(const std::string& device, const WaitForDeviceMode& mode) {
  // TODO(b/122059364): Make this more efficient
  // TODO: use std::chrono?

  // Deleting a device might take more time, so wait a little bit longer.
  size_t num_tries = mode == kWaitToBeCreated ? 10u : 15u;

  LOG(DEBUG) << "Waiting for " << device << " to be "
             << (mode == kWaitToBeCreated ? "created" : " deleted");
  for (size_t i = 0; i < num_tries; ++i) {
    StatusOr<bool> status = PathExists(device);
    if (status.Ok()) {
      if (mode == kWaitToBeCreated && *status) {
        return Status::Success();
      }
      if (mode == kWaitToBeDeleted && !*status) {
        return Status::Success();
      }
    }
    if (i + 1 < num_tries) {
      usleep(50000);
    }
  }

  return Status::Fail(StringLog()
                      << "Failed to wait for device " << device << " to be "
                      << (mode == kWaitToBeCreated ? " created" : " deleted"));
}

// Deletes a dm-verity device with a given name and path.
// Synchronizes on the device actually being deleted from userspace.
Status DeleteVerityDevice(const std::string& name, const std::string& path) {
  DeviceMapper& dm = DeviceMapper::Instance();
  if (!dm.DeleteDevice(name)) {
    return Status::Fail(StringLog() << "Failed to delete device " << name
                                    << " with path " << path);
  }
  // Block until device is deleted from userspace.
  return waitForDevice(path, kWaitToBeDeleted);
}

// Deletes dm-verity device with a given name.
// See function above.
Status DeleteVerityDevice(const std::string& name) {
  DeviceMapper& dm = DeviceMapper::Instance();
  std::string path;
  if (!dm.GetDmDevicePathByName(name, &path)) {
    return Status::Fail(StringLog()
                        << "Unable to get path for dm-verity device " << name);
  }
  return DeleteVerityDevice(name, path);
}

class DmVerityDevice {
 public:
  DmVerityDevice() : cleared_(true) {}
  explicit DmVerityDevice(const std::string& name)
      : name_(name), cleared_(false) {}
  DmVerityDevice(const std::string& name, const std::string& dev_path)
      : name_(name), dev_path_(dev_path), cleared_(false) {}

  DmVerityDevice(DmVerityDevice&& other) noexcept
      : name_(std::move(other.name_)),
        dev_path_(std::move(other.dev_path_)),
        cleared_(other.cleared_) {
    other.cleared_ = true;
  }

  DmVerityDevice& operator=(DmVerityDevice&& other) noexcept {
    name_ = other.name_;
    dev_path_ = other.dev_path_;
    cleared_ = other.cleared_;
    other.cleared_ = true;
    return *this;
  }

  ~DmVerityDevice() {
    if (!cleared_) {
      Status ret = DeleteVerityDevice(name_, dev_path_);
      if (!ret.Ok()) {
        LOG(ERROR) << ret.ErrorMessage();
      }
    }
  }

  const std::string& GetName() const { return name_; }
  const std::string& GetDevPath() const { return dev_path_; }
  void SetDevPath(const std::string& dev_path) { dev_path_ = dev_path; }

  void Release() { cleared_ = true; }

 private:
  std::string name_;
  std::string dev_path_;
  bool cleared_;
};

StatusOr<DmVerityDevice> createVerityDevice(const std::string& name,
                                            const DmTable& table) {
  DeviceMapper& dm = DeviceMapper::Instance();

  if (dm.GetState(name) != DmDeviceState::INVALID) {
    // TODO: since apexd tears down devices during unmount, can this happen?
    LOG(WARNING) << "Deleting existing dm device " << name;
    const Status& status = DeleteVerityDevice(name);
    if (!status.Ok()) {
      // TODO: should we fail instead?
      LOG(ERROR) << "Failed to delete device " << name << " : "
                 << status.ErrorMessage();
    }
  }

  if (!dm.CreateDevice(name, table)) {
    return StatusOr<DmVerityDevice>::MakeError(
        "Couldn't create verity device.");
  }
  DmVerityDevice dev(name);

  std::string dev_path;
  if (!dm.GetDmDevicePathByName(name, &dev_path)) {
    return StatusOr<DmVerityDevice>::MakeError(
        "Couldn't get verity device path!");
  }
  dev.SetDevPath(dev_path);

  return StatusOr<DmVerityDevice>(std::move(dev));
}

Status RemovePreviouslyActiveApexFiles(
    const std::unordered_set<std::string>& affected_packages,
    const std::unordered_set<std::string>& files_to_keep) {
  auto all_active_apex_files =
      FindApexFilesByName(kActiveApexPackagesDataDir, false /* include_dirs */);

  if (!all_active_apex_files.Ok()) {
    return all_active_apex_files.ErrorStatus();
  }

  for (const std::string& path : *all_active_apex_files) {
    StatusOr<ApexFile> apex_file = ApexFile::Open(path);
    if (!apex_file.Ok()) {
      return apex_file.ErrorStatus();
    }

    const std::string& package_name = apex_file->GetManifest().name();
    if (affected_packages.find(package_name) == affected_packages.end()) {
      // This apex belongs to a package that wasn't part of this stage sessions,
      // hence it should be kept.
      continue;
    }

    if (files_to_keep.find(apex_file->GetPath()) != files_to_keep.end()) {
      // This is a path that was staged and should be kept.
      continue;
    }

    LOG(DEBUG) << "Deleting previously active apex " << apex_file->GetPath();
    if (unlink(apex_file->GetPath().c_str()) != 0) {
      return Status::Fail(PStringLog()
                          << "Failed to unlink " << apex_file->GetPath());
    }
  }

  return Status::Success();
}

// Reads the entire device to verify the image is authenticatic
Status readVerityDevice(const std::string& verity_device,
                        uint64_t device_size) {
  static constexpr int kBlockSize = 4096;
  static constexpr size_t kBufSize = 1024 * kBlockSize;
  std::vector<uint8_t> buffer(kBufSize);

  unique_fd fd(TEMP_FAILURE_RETRY(open(verity_device.c_str(), O_RDONLY)));
  if (fd.get() == -1) {
    return Status::Fail(PStringLog() << "Can't open " << verity_device);
  }

  size_t bytes_left = device_size;
  while (bytes_left > 0) {
    size_t to_read = std::min(bytes_left, kBufSize);
    if (!android::base::ReadFully(fd.get(), buffer.data(), to_read)) {
      return Status::Fail(PStringLog() << "Can't verify " << verity_device
                                       << "; corrupted?");
    }
    bytes_left -= to_read;
  }

  return Status::Success();
}

Status VerifyMountedImage(const ApexFile& apex,
                          const std::string& mount_point) {
  auto status = apex.VerifyManifestMatches(mount_point);
  if (!status.Ok()) {
    return status;
  }
  if (shim::IsShimApex(apex)) {
    return shim::ValidateShimApex(mount_point, apex);
  }
  return Status::Success();
}

StatusOr<MountedApexData> mountNonFlattened(const ApexFile& apex,
                                            const std::string& mountPoint,
                                            const std::string& device_name,
                                            bool verifyImage) {
  using StatusM = StatusOr<MountedApexData>;
  const std::string& full_path = apex.GetPath();

  if (!kUpdatable) {
    return StatusM::Fail(StringLog()
                         << "Unable to mount non-flattened apex package "
                         << full_path << " because device doesn't support it");
  }

  loop::LoopbackDeviceUniqueFd loopbackDevice;
  for (size_t attempts = 1;; ++attempts) {
    StatusOr<loop::LoopbackDeviceUniqueFd> ret = loop::createLoopDevice(
        full_path, apex.GetImageOffset(), apex.GetImageSize());
    if (ret.Ok()) {
      loopbackDevice = std::move(*ret);
      break;
    }
    if (attempts >= kLoopDeviceSetupAttempts) {
      return StatusM::Fail(StringLog()
                           << "Could not create loop device for " << full_path
                           << ": " << ret.ErrorMessage());
    }
  }
  LOG(VERBOSE) << "Loopback device created: " << loopbackDevice.name;

  auto verityData = apex.VerifyApexVerity();
  if (!verityData.Ok()) {
    return StatusM::Fail(StringLog()
                         << "Failed to verify Apex Verity data for "
                         << full_path << ": " << verityData.ErrorMessage());
  }
  std::string blockDevice = loopbackDevice.name;
  MountedApexData apex_data(loopbackDevice.name, apex.GetPath(), mountPoint,
                            device_name);

  // for APEXes in immutable partitions, we don't need to mount them on
  // dm-verity because they are already in the dm-verity protected partition;
  // system. However, note that we don't skip verification to ensure that APEXes
  // are correctly signed.
  const bool mountOnVerity =
      gForceDmVerityOnSystem || !isPathForBuiltinApexes(full_path);
  DmVerityDevice verityDev;
  if (mountOnVerity) {
    auto verityTable =
        createVerityTable(*verityData, loopbackDevice.name,
                          /* restart_on_corruption = */ !verifyImage);
    StatusOr<DmVerityDevice> verityDevRes =
        createVerityDevice(device_name, *verityTable);
    if (!verityDevRes.Ok()) {
      return StatusM::Fail(StringLog()
                           << "Failed to create Apex Verity device "
                           << full_path << ": " << verityDevRes.ErrorMessage());
    }
    verityDev = std::move(*verityDevRes);
    blockDevice = verityDev.GetDevPath();

    Status readAheadStatus = loop::configureReadAhead(verityDev.GetDevPath());
    if (!readAheadStatus.Ok()) {
      return StatusM::MakeError(readAheadStatus);
    }
  }

  // TODO(b/122059364): Even though the kernel has created the verity
  // device, we still depend on ueventd to run to actually create the
  // device node in userspace. To solve this properly we should listen on
  // the netlink socket for uevents, or use inotify. For now, this will
  // have to do.
  Status deviceStatus = waitForDevice(blockDevice, kWaitToBeCreated);
  if (!deviceStatus.Ok()) {
    return StatusM::MakeError(deviceStatus);
  }

  // TODO: consider moving this inside RunVerifyFnInsideTempMount.
  if (mountOnVerity && verifyImage) {
    Status verityStatus =
        readVerityDevice(blockDevice, (*verityData).desc->image_size);
    if (!verityStatus.Ok()) {
      return StatusM::MakeError(verityStatus);
    }
  }

  if (mount(blockDevice.c_str(), mountPoint.c_str(), "ext4",
            MS_NOATIME | MS_NODEV | MS_DIRSYNC | MS_RDONLY, nullptr) == 0) {
    LOG(INFO) << "Successfully mounted package " << full_path << " on "
              << mountPoint;
    auto status = VerifyMountedImage(apex, mountPoint);
    if (!status.Ok()) {
      umount2(mountPoint.c_str(), UMOUNT_NOFOLLOW | MNT_DETACH);
      return StatusM::Fail(StringLog() << "Failed to verify " << full_path
                                       << ": " << status.ErrorMessage());
    }
    // Time to accept the temporaries as good.
    if (mountOnVerity) {
      verityDev.Release();
    }
    loopbackDevice.CloseGood();

    return StatusM(std::move(apex_data));
  } else {
    return StatusM::Fail(StringLog() << "Mounting failed for package "
                                     << full_path << " : " << strerror(errno));
  }
}

StatusOr<MountedApexData> mountFlattened(const ApexFile& apex,
                                         const std::string& mountPoint) {
  using StatusM = StatusOr<MountedApexData>;
  if (!isPathForBuiltinApexes(apex.GetPath())) {
    return StatusM::Fail(StringLog() << "Cannot activate flattened APEX "
                                     << apex.GetPath());
  }

  if (mount(apex.GetPath().c_str(), mountPoint.c_str(), nullptr, MS_BIND,
            nullptr) == 0) {
    LOG(INFO) << "Successfully bind-mounted flattened package "
              << apex.GetPath() << " on " << mountPoint;

    MountedApexData apex_data("" /* loop_name */, apex.GetPath(), mountPoint,
                              "" /* device_name */);
    return StatusM(std::move(apex_data));
  }
  return StatusM::Fail(PStringLog() << "Mounting failed for flattened package "
                                    << apex.GetPath());
}

StatusOr<MountedApexData> MountPackageImpl(const ApexFile& apex,
                                           const std::string& mountPoint,
                                           const std::string& device_name,
                                           bool verifyImage) {
  using StatusM = StatusOr<MountedApexData>;
  LOG(VERBOSE) << "Creating mount point: " << mountPoint;
  // Note: the mount point could exist in case when the APEX was activated
  // during the bootstrap phase (e.g., the runtime or tzdata APEX).
  // Although we have separate mount namespaces to separate the early activated
  // APEXes from the normally activate APEXes, the mount points themselves
  // are shared across the two mount namespaces because /apex (a tmpfs) itself
  // mounted at / which is (and has to be) a shared mount. Therefore, if apexd
  // finds an empty directory under /apex, it's not a problem and apexd can use
  // it.
  auto exists = PathExists(mountPoint);
  if (!exists.Ok()) {
    return StatusM::MakeError(exists.ErrorStatus());
  }
  if (!*exists && mkdir(mountPoint.c_str(), kMkdirMode) != 0) {
    return StatusM::Fail(PStringLog()
                         << "Could not create mount point " << mountPoint);
  }
  auto deleter = [&mountPoint]() {
    if (rmdir(mountPoint.c_str()) != 0) {
      PLOG(WARNING) << "Could not rmdir " << mountPoint;
    }
  };
  auto scope_guard = android::base::make_scope_guard(deleter);
  if (!IsEmptyDirectory(mountPoint)) {
    return StatusM::Fail(PStringLog() << mountPoint << " is not empty");
  }

  StatusOr<MountedApexData> ret;
  if (apex.IsFlattened()) {
    ret = mountFlattened(apex, mountPoint);
  } else {
    ret = mountNonFlattened(apex, mountPoint, device_name, verifyImage);
  }
  if (ret.Ok()) {
    scope_guard.Disable();  // Accept the mount.
  }
  return ret;
}

StatusOr<MountedApexData> VerifyAndTempMountPackage(
    const ApexFile& apex, const std::string& mount_point) {
  const std::string& package_id = GetPackageId(apex.GetManifest());
  LOG(DEBUG) << "Temp mounting " << package_id << " to " << mount_point;
  const std::string& temp_device_name = package_id + ".tmp";
  return MountPackageImpl(apex, mount_point, temp_device_name,
                          /* verifyImage = */ true);
}

Status Unmount(const MountedApexData& data) {
  // Lazily try to umount whatever is mounted.
  if (umount2(data.mount_point.c_str(), UMOUNT_NOFOLLOW | MNT_DETACH) != 0 &&
      errno != EINVAL && errno != ENOENT) {
    return Status::Fail(PStringLog()
                        << "Failed to unmount directory " << data.mount_point);
  }
  // Attempt to delete the folder. If the folder is retained, other
  // data may be incorrect.
  if (rmdir(data.mount_point.c_str()) != 0) {
    PLOG(ERROR) << "Failed to rmdir directory " << data.mount_point;
  }

  // Try to free up the device-mapper device.
  if (!data.device_name.empty()) {
    const auto& status = DeleteVerityDevice(data.device_name);
    if (!status.Ok()) {
      LOG(DEBUG) << "Failed to free device " << data.device_name << " : "
                 << status.ErrorMessage();
    }
  }

  // Try to free up the loop device.
  if (!data.loop_name.empty()) {
    auto log_fn = [](const std::string& path,
                     const std::string& id ATTRIBUTE_UNUSED) {
      LOG(VERBOSE) << "Freeing loop device " << path << "for unmount.";
    };
    loop::DestroyLoopDevice(data.loop_name, log_fn);
  }

  return Status::Success();
}

std::string GetPackageTempMountPoint(const ApexManifest& manifest) {
  return StringPrintf("%s.tmp",
                      apexd_private::GetPackageMountPoint(manifest).c_str());
}

template <typename VerifyFn>
Status RunVerifyFnInsideTempMount(const ApexFile& apex,
                                  const VerifyFn& verify_fn) {
  // Temp mount image of this apex to validate it was properly signed;
  // this will also read the entire block device through dm-verity, so
  // we can be sure there is no corruption.
  const std::string& temp_mount_point =
      GetPackageTempMountPoint(apex.GetManifest());

  StatusOr<MountedApexData> mount_status =
      VerifyAndTempMountPackage(apex, temp_mount_point);
  if (!mount_status.Ok()) {
    LOG(ERROR) << "Failed to temp mount to " << temp_mount_point << " : "
               << mount_status.ErrorMessage();
    return mount_status.ErrorStatus();
  }
  auto cleaner = [&]() {
    LOG(DEBUG) << "Unmounting " << temp_mount_point;
    Status status = Unmount(*mount_status);
    if (!status.Ok()) {
      LOG(WARNING) << "Failed to unmount " << temp_mount_point << " : "
                   << status.ErrorMessage();
    }
  };
  auto scope_guard = android::base::make_scope_guard(cleaner);
  return verify_fn(temp_mount_point);
}

template <typename HookFn, typename HookCall>
Status PrePostinstallPackages(const std::vector<ApexFile>& apexes, HookFn fn,
                              HookCall call) {
  if (apexes.empty()) {
    return Status::Fail("Empty set of inputs");
  }

  // 1) Check whether the APEXes have hooks.
  bool has_hooks = false;
  for (const ApexFile& apex_file : apexes) {
    if (!(apex_file.GetManifest().*fn)().empty()) {
      has_hooks = true;
      break;
    }
  }

  // 2) If we found hooks, run the pre/post-install.
  if (has_hooks) {
    Status install_status = (*call)(apexes);
    if (!install_status.Ok()) {
      return install_status;
    }
  }

  return Status::Success();
}

Status PreinstallPackages(const std::vector<ApexFile>& apexes) {
  return PrePostinstallPackages(apexes, &ApexManifest::preinstallhook,
                                &StagePreInstall);
}

Status PostinstallPackages(const std::vector<ApexFile>& apexes) {
  return PrePostinstallPackages(apexes, &ApexManifest::postinstallhook,
                                &StagePostInstall);
}

template <typename RetType, typename Fn>
RetType HandlePackages(const std::vector<std::string>& paths, Fn fn) {
  // 1) Open all APEXes.
  std::vector<ApexFile> apex_files;
  for (const std::string& path : paths) {
    StatusOr<ApexFile> apex_file = ApexFile::Open(path);
    if (!apex_file.Ok()) {
      return RetType::Fail(apex_file.ErrorMessage());
    }
    apex_files.emplace_back(std::move(*apex_file));
  }

  // 2) Dispatch.
  return fn(apex_files);
}

Status ValidateStagingShimApex(const ApexFile& to) {
  using android::base::StringPrintf;
  auto system_shim = ApexFile::Open(
      StringPrintf("%s/%s", kApexPackageSystemDir, shim::kSystemShimApexName));
  if (!system_shim.Ok()) {
    return system_shim.ErrorStatus();
  }
  auto verify_fn = [&](const std::string& system_apex_path) {
    return shim::ValidateUpdate(system_apex_path, to.GetPath());
  };
  return RunVerifyFnInsideTempMount(*system_shim, verify_fn);
}

// A version of apex verification that happens during boot.
// This function should only verification checks that are necessary to run on
// each boot. Try to avoid putting expensive checks inside this function.
Status VerifyPackageBoot(const ApexFile& apex_file) {
  if (apex_file.IsFlattened()) {
    return Status::Fail("Can't upgrade flattened apex");
  }
  StatusOr<ApexVerityData> verity_or = apex_file.VerifyApexVerity();
  if (!verity_or.Ok()) {
    return Status::Fail(verity_or.ErrorMessage());
  }

  if (shim::IsShimApex(apex_file)) {
    // Validating shim is not a very cheap operation, but it's fine to perform
    // it here since it only runs during CTS tests and will never be triggered
    // during normal flow.
    const auto& status = ValidateStagingShimApex(apex_file);
    if (!status.Ok()) {
      return status;
    }
  }
  return Status::Success();
}

// A version of apex verification that happens on submitStagedSession.
// This function contains checks that might be expensive to perform, e.g. temp
// mounting a package and reading entire dm-verity device, and shouldn't be run
// during boot.
Status VerifyPackageInstall(const ApexFile& apex_file) {
  const auto& verify_package_boot_status = VerifyPackageBoot(apex_file);
  if (!verify_package_boot_status.Ok()) {
    return verify_package_boot_status;
  }
  if (!kUpdatable) {
    return Status::Fail(StringLog() << "Attempted to upgrade apex package "
                                    << apex_file.GetPath()
                                    << " on a device that doesn't support it");
  }
  StatusOr<ApexVerityData> verity_or = apex_file.VerifyApexVerity();

  constexpr const auto kSuccessFn = [](const std::string& _) {
    return Status::Success();
  };
  return RunVerifyFnInsideTempMount(apex_file, kSuccessFn);
}

template <typename VerifyApexFn>
StatusOr<std::vector<ApexFile>> verifyPackages(
    const std::vector<std::string>& paths, const VerifyApexFn& verify_apex_fn) {
  if (paths.empty()) {
    return StatusOr<std::vector<ApexFile>>::MakeError("Empty set of inputs");
  }
  LOG(DEBUG) << "verifyPackages() for " << Join(paths, ',');

  using StatusT = StatusOr<std::vector<ApexFile>>;
  auto verify_fn = [&](std::vector<ApexFile>& apexes) {
    for (const ApexFile& apex_file : apexes) {
      Status status = verify_apex_fn(apex_file);
      if (!status.Ok()) {
        return StatusT::MakeError(status);
      }
    }
    return StatusT(std::move(apexes));
  };
  return HandlePackages<StatusT>(paths, verify_fn);
}

StatusOr<ApexFile> verifySessionDir(const int session_id) {
  std::string sessionDirPath = std::string(kStagedSessionsDir) + "/session_" +
                               std::to_string(session_id);
  LOG(INFO) << "Scanning " << sessionDirPath
            << " looking for packages to be validated";
  StatusOr<std::vector<std::string>> scan =
      FindApexFilesByName(sessionDirPath, /* include_dirs=*/false);
  if (!scan.Ok()) {
    LOG(WARNING) << scan.ErrorMessage();
    return StatusOr<ApexFile>::MakeError(scan.ErrorMessage());
  }

  if (scan->size() > 1) {
    return StatusOr<ApexFile>::MakeError(
        "More than one APEX package found in the same session directory.");
  }

  auto verified = verifyPackages(*scan, VerifyPackageInstall);
  if (!verified.Ok()) {
    return StatusOr<ApexFile>::MakeError(verified.ErrorStatus());
  }
  return StatusOr<ApexFile>(std::move((*verified)[0]));
}

Status ClearSessions() {
  auto sessions = ApexSession::GetSessions();
  int cnt = 0;
  for (ApexSession& session : sessions) {
    Status status = session.DeleteSession();
    if (!status.Ok()) {
      return status;
    }
    cnt++;
  }
  if (cnt > 0) {
    LOG(DEBUG) << "Deleted " << cnt << " sessions";
  }
  return Status::Success();
}

Status DeleteBackup() {
  auto exists = PathExists(std::string(kApexBackupDir));
  if (!exists.Ok()) {
    return Status::Fail(StringLog() << "Can't clean " << kApexBackupDir << " : "
                                    << exists.ErrorMessage());
  }
  if (!*exists) {
    LOG(DEBUG) << kApexBackupDir << " does not exist. Nothing to clean";
    return Status::Success();
  }
  return DeleteDirContent(std::string(kApexBackupDir));
}

Status BackupActivePackages() {
  LOG(DEBUG) << "Initializing  backup of " << kActiveApexPackagesDataDir;

  // Previous restore might've delete backups folder.
  auto create_status = createDirIfNeeded(kApexBackupDir, 0700);
  if (!create_status.Ok()) {
    return Status::Fail(StringLog()
                        << "Backup failed : " << create_status.ErrorMessage());
  }

  auto apex_active_exists = PathExists(std::string(kActiveApexPackagesDataDir));
  if (!apex_active_exists.Ok()) {
    return Status::Fail("Backup failed : " + apex_active_exists.ErrorMessage());
  }
  if (!*apex_active_exists) {
    LOG(DEBUG) << kActiveApexPackagesDataDir
               << " does not exist. Nothing to backup";
    return Status::Success();
  }

  auto active_packages =
      FindApexFilesByName(kActiveApexPackagesDataDir, false /* include_dirs */);
  if (!active_packages.Ok()) {
    return Status::Fail(StringLog() << "Backup failed : "
                                    << active_packages.ErrorMessage());
  }

  auto cleanup_status = DeleteBackup();
  if (!cleanup_status.Ok()) {
    return Status::Fail(StringLog()
                        << "Backup failed : " << cleanup_status.ErrorMessage());
  }

  auto backup_path_fn = [](const ApexFile& apex_file) {
    return StringPrintf("%s/%s%s", kApexBackupDir,
                        GetPackageId(apex_file.GetManifest()).c_str(),
                        kApexPackageSuffix);
  };

  auto deleter = []() {
    auto status = DeleteDirContent(std::string(kApexBackupDir));
    if (!status.Ok()) {
      LOG(ERROR) << "Failed to cleanup " << kApexBackupDir << " : "
                 << status.ErrorMessage();
    }
  };
  auto scope_guard = android::base::make_scope_guard(deleter);

  for (const std::string& path : *active_packages) {
    StatusOr<ApexFile> apex_file = ApexFile::Open(path);
    if (!apex_file.Ok()) {
      return Status::Fail("Backup failed : " + apex_file.ErrorMessage());
    }
    const auto& dest_path = backup_path_fn(*apex_file);
    if (link(apex_file->GetPath().c_str(), dest_path.c_str()) != 0) {
      return Status::Fail(PStringLog()
                          << "Failed to backup " << apex_file->GetPath());
    }
  }

  scope_guard.Disable();  // Accept the backup.
  return Status::Success();
}

Status DoRollback(ApexSession& session) {
  if (gInFsCheckpointMode) {
    // We will roll back automatically when we reboot
    return Status::Success();
  }
  auto scope_guard = android::base::make_scope_guard([&]() {
    auto st = session.UpdateStateAndCommit(SessionState::ROLLBACK_FAILED);
    LOG(DEBUG) << "Marking " << session << " as failed to rollback";
    if (!st.Ok()) {
      LOG(WARNING) << "Failed to mark session " << session
                   << " as failed to rollback : " << st.ErrorMessage();
    }
  });

  auto backup_exists = PathExists(std::string(kApexBackupDir));
  if (!backup_exists.Ok()) {
    return backup_exists.ErrorStatus();
  }
  if (!*backup_exists) {
    return Status::Fail(StringLog() << kApexBackupDir << " does not exist");
  }

  struct stat stat_data;
  if (stat(kActiveApexPackagesDataDir, &stat_data) != 0) {
    return Status::Fail(PStringLog()
                        << "Failed to access " << kActiveApexPackagesDataDir);
  }

  LOG(DEBUG) << "Deleting existing packages in " << kActiveApexPackagesDataDir;
  auto delete_status =
      DeleteDirContent(std::string(kActiveApexPackagesDataDir));
  if (!delete_status.Ok()) {
    return delete_status;
  }

  LOG(DEBUG) << "Renaming " << kApexBackupDir << " to "
             << kActiveApexPackagesDataDir;
  if (rename(kApexBackupDir, kActiveApexPackagesDataDir) != 0) {
    return Status::Fail(PStringLog() << "Failed to rename " << kApexBackupDir
                                     << " to " << kActiveApexPackagesDataDir);
  }

  LOG(DEBUG) << "Restoring original permissions for "
             << kActiveApexPackagesDataDir;
  if (chmod(kActiveApexPackagesDataDir, stat_data.st_mode & ALLPERMS) != 0) {
    // TODO: should we wipe out /data/apex/active if chmod fails?
    return Status::Fail(PStringLog()
                        << "Failed to restore original permissions for "
                        << kActiveApexPackagesDataDir);
  }

  scope_guard.Disable();  // Rollback succeeded. Accept state.
  return Status::Success();
}

Status RollbackStagedSession(ApexSession& session) {
  // If the session is staged, it hasn't been activated yet, and we just need
  // to update its state to prevent it from being activated later.
  return session.UpdateStateAndCommit(SessionState::ROLLED_BACK);
}

Status RollbackActivatedSession(ApexSession& session) {
  if (gInFsCheckpointMode) {
    LOG(DEBUG) << "Checkpoint mode is enabled";
    // On checkpointing devices, our modifications on /data will be
    // automatically rolled back when we abort changes. Updating the session
    // state is pointless here, as it will be rolled back as well.
    return Status::Success();
  }

  auto status =
      session.UpdateStateAndCommit(SessionState::ROLLBACK_IN_PROGRESS);
  if (!status.Ok()) {
    // TODO: should we continue with a rollback?
    return Status::Fail(StringLog() << "Rollback of session " << session
                                    << " failed : " << status.ErrorMessage());
  }

  status = DoRollback(session);
  if (!status.Ok()) {
    return Status::Fail(StringLog() << "Rollback of session " << session
                                    << " failed : " << status.ErrorMessage());
  }

  status = session.UpdateStateAndCommit(SessionState::ROLLED_BACK);
  if (!status.Ok()) {
    LOG(WARNING) << "Failed to mark session " << session
                 << " as rolled back : " << status.ErrorMessage();
  }

  return Status::Success();
}

Status RollbackSession(ApexSession& session) {
  LOG(DEBUG) << "Initializing rollback of " << session;

  switch (session.GetState()) {
    case SessionState::ROLLBACK_IN_PROGRESS:
      [[clang::fallthrough]];
    case SessionState::ROLLED_BACK:
      return Status::Success();
    case SessionState::STAGED:
      return RollbackStagedSession(session);
    case SessionState::ACTIVATED:
      return RollbackActivatedSession(session);
    default:
      return Status::Fail(StringLog() << "Can't restore session " << session
                                      << " : session is in a wrong state");
  }
}

Status ResumeRollback(ApexSession& session) {
  auto backup_exists = PathExists(std::string(kApexBackupDir));
  if (!backup_exists.Ok()) {
    return backup_exists.ErrorStatus();
  }
  if (*backup_exists) {
    auto rollback_status = DoRollback(session);
    if (!rollback_status.Ok()) {
      return rollback_status;
    }
  }
  auto status = session.UpdateStateAndCommit(SessionState::ROLLED_BACK);
  if (!status.Ok()) {
    LOG(WARNING) << "Failed to mark session " << session
                 << " as rolled back : " << status.ErrorMessage();
  }
  return Status::Success();
}

Status UnmountPackage(const ApexFile& apex, bool allow_latest) {
  LOG(VERBOSE) << "Unmounting " << GetPackageId(apex.GetManifest());

  const ApexManifest& manifest = apex.GetManifest();

  std::optional<MountedApexData> data;
  bool latest = false;

  auto fn = [&](const MountedApexData& d, bool l) {
    if (d.full_path == apex.GetPath()) {
      data.emplace(d);
      latest = l;
    }
  };
  gMountedApexes.ForallMountedApexes(manifest.name(), fn);

  if (!data.has_value()) {
    return Status::Fail(StringLog() << "Did not find " << apex.GetPath());
  }

  if (latest) {
    if (!allow_latest) {
      return Status::Fail(StringLog()
                          << "Package " << apex.GetPath() << " is active");
    }
    std::string mount_point = apexd_private::GetActiveMountPoint(manifest);
    LOG(VERBOSE) << "Unmounting and deleting " << mount_point;
    if (umount2(mount_point.c_str(), UMOUNT_NOFOLLOW | MNT_DETACH) != 0) {
      return Status::Fail(PStringLog() << "Failed to unmount " << mount_point);
    }
    if (rmdir(mount_point.c_str()) != 0) {
      PLOG(ERROR) << "Could not rmdir " << mount_point;
      // Continue here.
    }
  }

  // Clean up gMountedApexes now, even though we're not fully done.
  gMountedApexes.RemoveMountedApex(manifest.name(), apex.GetPath());
  return Unmount(*data);
}

}  // namespace

namespace apexd_private {

Status MountPackage(const ApexFile& apex, const std::string& mountPoint) {
  auto ret =
      MountPackageImpl(apex, mountPoint, GetPackageId(apex.GetManifest()),
                       /* verifyImage = */ false);
  if (!ret.Ok()) {
    return ret.ErrorStatus();
  }

  gMountedApexes.AddMountedApex(apex.GetManifest().name(), false,
                                std::move(*ret));
  return Status::Success();
}

Status UnmountPackage(const ApexFile& apex) {
  return android::apex::UnmountPackage(apex, /* allow_latest= */ false);
}

bool IsMounted(const std::string& name, const std::string& full_path) {
  bool found_mounted = false;
  gMountedApexes.ForallMountedApexes(
      name, [&](const MountedApexData& data, bool latest ATTRIBUTE_UNUSED) {
        if (full_path == data.full_path) {
          found_mounted = true;
        }
      });
  return found_mounted;
}

std::string GetPackageMountPoint(const ApexManifest& manifest) {
  return StringPrintf("%s/%s", kApexRoot, GetPackageId(manifest).c_str());
}

std::string GetActiveMountPoint(const ApexManifest& manifest) {
  return StringPrintf("%s/%s", kApexRoot, manifest.name().c_str());
}

}  // namespace apexd_private

Status resumeRollbackIfNeeded() {
  auto session = ApexSession::GetActiveSession();
  if (!session.Ok()) {
    return session.ErrorStatus();
  }
  if (!session->has_value()) {
    return Status::Success();
  }
  if ((**session).GetState() == SessionState::ROLLBACK_IN_PROGRESS) {
    // This means that phone was rebooted during the rollback. Resuming it.
    return ResumeRollback(**session);
  }
  return Status::Success();
}

Status activatePackageImpl(const ApexFile& apex_file) {
  const ApexManifest& manifest = apex_file.GetManifest();

  if (gBootstrap && !isBootstrapApex(apex_file)) {
    LOG(INFO) << "Skipped when bootstrapping";
    return Status::Success();
  } else if (!kUpdatable && !gBootstrap && isBootstrapApex(apex_file)) {
    LOG(INFO) << "Package already activated in bootstrap";
    return Status::Success();
  }

  // See whether we think it's active, and do not allow to activate the same
  // version. Also detect whether this is the highest version.
  // We roll this into a single check.
  bool is_newest_version = true;
  bool found_other_version = false;
  bool version_found_mounted = false;
  {
    uint64_t new_version = manifest.version();
    bool version_found_active = false;
    gMountedApexes.ForallMountedApexes(
        manifest.name(), [&](const MountedApexData& data, bool latest) {
          StatusOr<ApexFile> otherApex = ApexFile::Open(data.full_path);
          if (!otherApex.Ok()) {
            return;
          }
          found_other_version = true;
          if (static_cast<uint64_t>(otherApex->GetManifest().version()) ==
              new_version) {
            version_found_mounted = true;
            version_found_active = latest;
          }
          if (static_cast<uint64_t>(otherApex->GetManifest().version()) >
              new_version) {
            is_newest_version = false;
          }
        });
    if (version_found_active) {
      LOG(DEBUG) << "Package " << manifest.name() << " with version "
                 << manifest.version() << " already active";
      return Status::Success();
    }
  }

  const std::string& mountPoint = apexd_private::GetPackageMountPoint(manifest);

  if (!version_found_mounted) {
    Status mountStatus = apexd_private::MountPackage(apex_file, mountPoint);
    if (!mountStatus.Ok()) {
      return mountStatus;
    }
  }

  bool mounted_latest = false;
  if (is_newest_version) {
    const Status& update_st = apexd_private::BindMount(
        apexd_private::GetActiveMountPoint(manifest), mountPoint);
    mounted_latest = update_st.Ok();
    if (!update_st.Ok()) {
      return Status::Fail(StringLog()
                          << "Failed to update package " << manifest.name()
                          << " to version " << manifest.version() << " : "
                          << update_st.ErrorMessage());
    }
  }
  if (mounted_latest) {
    gMountedApexes.SetLatest(manifest.name(), apex_file.GetPath());
  }

  LOG(DEBUG) << "Successfully activated " << apex_file.GetPath()
             << " package_name: " << manifest.name()
             << " version: " << manifest.version();
  return Status::Success();
}

Status activatePackage(const std::string& full_path) {
  LOG(INFO) << "Trying to activate " << full_path;

  StatusOr<ApexFile> apex_file = ApexFile::Open(full_path);
  if (!apex_file.Ok()) {
    return apex_file.ErrorStatus();
  }
  return activatePackageImpl(*apex_file);
}

Status deactivatePackage(const std::string& full_path) {
  LOG(INFO) << "Trying to deactivate " << full_path;

  StatusOr<ApexFile> apexFile = ApexFile::Open(full_path);
  if (!apexFile.Ok()) {
    return apexFile.ErrorStatus();
  }

  return UnmountPackage(*apexFile, /* allow_latest= */ true);
}

std::vector<ApexFile> getActivePackages() {
  std::vector<ApexFile> ret;
  gMountedApexes.ForallMountedApexes(
      [&](const std::string&, const MountedApexData& data, bool latest) {
        if (!latest) {
          return;
        }

        StatusOr<ApexFile> apexFile = ApexFile::Open(data.full_path);
        if (!apexFile.Ok()) {
          // TODO: Fail?
          return;
        }
        ret.emplace_back(std::move(*apexFile));
      });

  return ret;
}

namespace {
std::unordered_map<std::string, uint64_t> GetActivePackagesMap() {
  std::vector<ApexFile> active_packages = getActivePackages();
  std::unordered_map<std::string, uint64_t> ret;
  for (const auto& package : active_packages) {
    const ApexManifest& manifest = package.GetManifest();
    ret.insert({manifest.name(), manifest.version()});
  }
  return ret;
}

}  // namespace

std::vector<ApexFile> getFactoryPackages() {
  std::vector<ApexFile> ret;
  for (const auto& dir : kApexPackageBuiltinDirs) {
    auto apex_files = FindApexFilesByName(dir, /* include_dirs=*/false);
    if (!apex_files.Ok()) {
      LOG(ERROR) << apex_files.ErrorMessage();
      continue;
    }
    for (const std::string& path : *apex_files) {
      StatusOr<ApexFile> apex_file = ApexFile::Open(path);
      if (!apex_file.Ok()) {
        LOG(ERROR) << apex_file.ErrorMessage();
      } else {
        ret.emplace_back(std::move(*apex_file));
      }
    }
  }
  return ret;
}

StatusOr<ApexFile> getActivePackage(const std::string& packageName) {
  std::vector<ApexFile> packages = getActivePackages();
  for (ApexFile& apex : packages) {
    if (apex.GetManifest().name() == packageName) {
      return StatusOr<ApexFile>(std::move(apex));
    }
  }

  return StatusOr<ApexFile>::MakeError(
      PStringLog() << "Cannot find matching package for: " << packageName);
}

Status abortActiveSession() {
  auto session_or_none = ApexSession::GetActiveSession();
  if (!session_or_none.Ok()) {
    return session_or_none.ErrorStatus();
  }
  if (session_or_none->has_value()) {
    auto& session = session_or_none->value();
    LOG(DEBUG) << "Aborting active session " << session;
    switch (session.GetState()) {
      case SessionState::VERIFIED:
        [[clang::fallthrough]];
      case SessionState::STAGED:
        return session.DeleteSession();
      case SessionState::ACTIVATED:
        return RollbackActivatedSession(session);
      default:
        return Status::Fail(StringLog()
                            << "Session " << session << " can't be aborted");
    }
  } else {
    LOG(DEBUG) << "There are no active sessions";
    return Status::Success();
  }
}

Status scanPackagesDirAndActivate(const char* apex_package_dir) {
  LOG(INFO) << "Scanning " << apex_package_dir << " looking for APEX packages.";

  const bool scanBuiltinApexes = isPathForBuiltinApexes(apex_package_dir);
  StatusOr<std::vector<std::string>> scan =
      FindApexFilesByName(apex_package_dir, scanBuiltinApexes);
  if (!scan.Ok()) {
    return Status::Fail(StringLog() << "Failed to scan " << apex_package_dir
                                    << " : " << scan.ErrorMessage());
  }

  const auto& packages_with_code = GetActivePackagesMap();

  std::vector<std::string> failed_pkgs;
  size_t activated_cnt = 0;
  size_t skipped_cnt = 0;
  for (const std::string& name : *scan) {
    LOG(INFO) << "Found " << name;

    StatusOr<ApexFile> apex_file = ApexFile::Open(name);
    if (!apex_file.Ok()) {
      LOG(ERROR) << "Failed to activate " << name << " : "
                 << apex_file.ErrorMessage();
      failed_pkgs.push_back(name);
      continue;
    }

    uint64_t new_version =
        static_cast<uint64_t>(apex_file->GetManifest().version());
    const auto& it = packages_with_code.find(apex_file->GetManifest().name());
    if (it != packages_with_code.end() && it->second >= new_version) {
      LOG(INFO) << "Skipping activation of " << name
                << " same package with higher version " << it->second
                << " is already active";
      skipped_cnt++;
      continue;
    }

    if (!kUpdatable && !apex_file->IsFlattened()) {
      LOG(INFO) << "Skipping activation of non-flattened apex package " << name
                << " because device doesn't support it";
      skipped_cnt++;
      continue;
    }

    Status res = activatePackageImpl(*apex_file);
    if (!res.Ok()) {
      LOG(ERROR) << "Failed to activate " << name << " : "
                 << res.ErrorMessage();
      failed_pkgs.push_back(name);
    } else {
      activated_cnt++;
    }
  }

  if (!failed_pkgs.empty()) {
    return Status::Fail(StringLog()
                        << "Failed to activate following packages : "
                        << Join(failed_pkgs, ','));
  }

  LOG(INFO) << "Activated " << activated_cnt
            << " packages. Skipped: " << skipped_cnt;
  return Status::Success();
}

void scanStagedSessionsDirAndStage() {
  using android::base::GetProperty;
  LOG(INFO) << "Scanning " << kApexSessionsDir
            << " looking for sessions to be activated.";

  auto stagedSessions = ApexSession::GetSessionsInState(SessionState::STAGED);
  for (auto& session : stagedSessions) {
    auto sessionId = session.GetId();

    auto session_failed_fn = [&]() {
      LOG(WARNING) << "Marking session " << sessionId << " as failed.";
      auto st = session.UpdateStateAndCommit(SessionState::ACTIVATION_FAILED);
      if (!st.Ok()) {
        LOG(WARNING) << "Failed to mark session " << sessionId
                     << " as failed : " << st.ErrorMessage();
      }
    };
    auto scope_guard = android::base::make_scope_guard(session_failed_fn);

    std::string build_fingerprint = GetProperty(kBuildFingerprintSysprop, "");
    if (session.GetBuildFingerprint().compare(build_fingerprint) != 0) {
      LOG(ERROR) << "APEX build fingerprint has changed";
      continue;
    }

    std::vector<std::string> dirsToScan;
    if (session.GetChildSessionIds().empty()) {
      dirsToScan.push_back(std::string(kStagedSessionsDir) + "/session_" +
                           std::to_string(sessionId));
    } else {
      for (auto childSessionId : session.GetChildSessionIds()) {
        dirsToScan.push_back(std::string(kStagedSessionsDir) + "/session_" +
                             std::to_string(childSessionId));
      }
    }

    std::vector<std::string> apexes;
    bool scanSuccessful = true;
    for (const auto& dirToScan : dirsToScan) {
      StatusOr<std::vector<std::string>> scan =
          FindApexFilesByName(dirToScan, /* include_dirs=*/false);
      if (!scan.Ok()) {
        LOG(WARNING) << scan.ErrorMessage();
        scanSuccessful = false;
        break;
      }

      if (scan->size() > 1) {
        LOG(WARNING) << "More than one APEX package found in the same session "
                     << "directory " << dirToScan << ", skipping activation.";
        scanSuccessful = false;
        break;
      }

      if (scan->empty()) {
        LOG(WARNING) << "No APEX packages found while scanning " << dirToScan
                     << " session id: " << sessionId << ".";
        scanSuccessful = false;
        break;
      }
      apexes.push_back(std::move((*scan)[0]));
    }

    if (!scanSuccessful) {
      continue;
    }

    // Run postinstall, if necessary.
    Status postinstall_status = postinstallPackages(apexes);
    if (!postinstall_status.Ok()) {
      LOG(ERROR) << "Postinstall failed for session "
                 << std::to_string(sessionId) << ": "
                 << postinstall_status.ErrorMessage();
      continue;
    }

    const Status result = stagePackages(apexes);
    if (!result.Ok()) {
      LOG(ERROR) << "Activation failed for packages " << Join(apexes, ',')
                 << ": " << result.ErrorMessage();
      continue;
    }

    // Session was OK, release scopeguard.
    scope_guard.Disable();

    auto st = session.UpdateStateAndCommit(SessionState::ACTIVATED);
    if (!st.Ok()) {
      LOG(ERROR) << "Failed to mark " << session
                 << " as activated : " << st.ErrorMessage();
    }
  }
}

Status preinstallPackages(const std::vector<std::string>& paths) {
  if (paths.empty()) {
    return Status::Fail("Empty set of inputs");
  }
  LOG(DEBUG) << "preinstallPackages() for " << Join(paths, ',');
  return HandlePackages<Status>(paths, PreinstallPackages);
}

Status postinstallPackages(const std::vector<std::string>& paths) {
  if (paths.empty()) {
    return Status::Fail("Empty set of inputs");
  }
  LOG(DEBUG) << "postinstallPackages() for " << Join(paths, ',');
  return HandlePackages<Status>(paths, PostinstallPackages);
}

namespace {
std::string StageDestPath(const ApexFile& apex_file) {
  return StringPrintf("%s/%s%s", kActiveApexPackagesDataDir,
                      GetPackageId(apex_file.GetManifest()).c_str(),
                      kApexPackageSuffix);
}

std::vector<std::string> FilterUnnecessaryStagingPaths(
    const std::vector<std::string>& tmp_paths) {
  const auto& packages_with_code = GetActivePackagesMap();

  auto filter_fn = [&packages_with_code](const std::string& path) {
    auto apex_file = ApexFile::Open(path);
    if (!apex_file.Ok()) {
      // Pretend that apex should be staged, so that stagePackages will fail
      // trying to open it.
      return true;
    }
    std::string dest_path = StageDestPath(*apex_file);
    if (access(dest_path.c_str(), F_OK) == 0) {
      LOG(DEBUG) << dest_path << " already exists. Skipping";
      return false;
    }
    const ApexManifest& manifest = apex_file->GetManifest();
    const auto& it = packages_with_code.find(manifest.name());
    uint64_t new_version = static_cast<uint64_t>(manifest.version());
    if (it != packages_with_code.end() && it->second == new_version) {
      LOG(DEBUG) << GetPackageId(manifest) << " is already active. Skipping";
      return false;
    }
    return true;
  };

  std::vector<std::string> ret;
  std::copy_if(tmp_paths.begin(), tmp_paths.end(), std::back_inserter(ret),
               filter_fn);
  return ret;
}

}  // namespace

Status stagePackages(const std::vector<std::string>& tmpPaths) {
  if (tmpPaths.empty()) {
    return Status::Fail("Empty set of inputs");
  }
  LOG(DEBUG) << "stagePackages() for " << Join(tmpPaths, ',');

  // Note: this function is temporary. As such the code is not optimized, e.g.,
  //       it will open ApexFiles multiple times.

  // 1) Verify all packages.
  auto verify_status = verifyPackages(tmpPaths, VerifyPackageBoot);
  if (!verify_status.Ok()) {
    return Status::Fail(verify_status.ErrorMessage());
  }

  // 2) Now stage all of them.

  // Make sure that kActiveApexPackagesDataDir exists.
  auto create_dir_status =
      createDirIfNeeded(std::string(kActiveApexPackagesDataDir), 0750);
  if (!create_dir_status.Ok()) {
    return Status::Fail(create_dir_status.ErrorMessage());
  }

  // 2) Filter out packages that do not require staging, e.g.:
  //    a) Their /data/apex/active/package.apex@version already exists.
  //    b) Such package is already active
  std::vector<std::string> paths_to_stage =
      FilterUnnecessaryStagingPaths(tmpPaths);
  if (paths_to_stage.empty()) {
    // Finish early if nothing to stage. Since stagePackages fails in case
    // tmpPaths is empty, it's fine to return Success here.
    return Status::Success();
  }

  // 3) Now stage all of them.

  // Ensure the APEX gets removed on failure.
  std::unordered_set<std::string> staged_files;
  auto deleter = [&staged_files]() {
    for (const std::string& staged_path : staged_files) {
      if (TEMP_FAILURE_RETRY(unlink(staged_path.c_str())) != 0) {
        PLOG(ERROR) << "Unable to unlink " << staged_path;
      }
    }
  };
  auto scope_guard = android::base::make_scope_guard(deleter);

  std::unordered_set<std::string> staged_packages;
  for (const std::string& path : paths_to_stage) {
    StatusOr<ApexFile> apex_file = ApexFile::Open(path);
    if (!apex_file.Ok()) {
      return apex_file.ErrorStatus();
    }
    std::string dest_path = StageDestPath(*apex_file);

    if (link(apex_file->GetPath().c_str(), dest_path.c_str()) != 0) {
      // TODO: Get correct binder error status.
      return Status::Fail(PStringLog()
                          << "Unable to link " << apex_file->GetPath() << " to "
                          << dest_path);
    }
    staged_files.insert(dest_path);
    staged_packages.insert(apex_file->GetManifest().name());

    LOG(DEBUG) << "Success linking " << apex_file->GetPath() << " to "
               << dest_path;
  }

  scope_guard.Disable();  // Accept the state.

  return RemovePreviouslyActiveApexFiles(staged_packages, staged_files);
}

Status unstagePackages(const std::vector<std::string>& paths) {
  if (paths.empty()) {
    return Status::Fail("Empty set of inputs");
  }
  LOG(DEBUG) << "unstagePackages() for " << Join(paths, ',');

  // TODO: to make unstage safer, we can copy to be unstaged packages to a
  // temporary folder and restore state from it in case unstagePackages fails.

  for (const std::string& path : paths) {
    if (access(path.c_str(), F_OK) != 0) {
      return Status::Fail(PStringLog() << "Can't access " << path);
    }
  }

  for (const std::string& path : paths) {
    if (unlink(path.c_str()) != 0) {
      return Status::Fail(PStringLog() << "Can't unlink " << path);
    }
  }

  return Status::Success();
}

Status rollbackStagedSessionIfAny() {
  auto session = ApexSession::GetActiveSession();
  if (!session.Ok()) {
    return session.ErrorStatus();
  }
  if (!session->has_value()) {
    LOG(WARNING) << "No session to rollback";
    return Status::Success();
  }
  if ((*session)->GetState() == SessionState::STAGED) {
    LOG(INFO) << "Rolling back session " << **session;
    return RollbackStagedSession(**session);
  }
  return Status::Fail(StringLog() << "Can't rollback " << **session
                                  << " because it is not in STAGED state");
}

Status rollbackActiveSession() {
  auto session = ApexSession::GetActiveSession();
  if (!session.Ok()) {
    return Status::Fail(StringLog() << "Failed to get active session : "
                                    << session.ErrorMessage());
  } else if (!session->has_value()) {
    return Status::Fail(
        "Rollback requested, when there are no active sessions.");
  } else {
    return RollbackSession(*(*session));
  }
}

Status rollbackActiveSessionAndReboot() {
  auto status = rollbackActiveSession();
  if (!status.Ok()) {
    return status;
  }
  LOG(ERROR) << "Successfully rolled back. Time to reboot device.";
  if (gInFsCheckpointMode) {
    Status res = gVoldService->AbortChanges("apexd_initiated" /* message */,
                                            false /* retry */);
    if (!res.Ok()) {
      LOG(ERROR) << res.ErrorMessage();
    }
  }
  Reboot();
  return Status::Success();
}

int onBootstrap() {
  gBootstrap = true;

  Status preAllocate = preAllocateLoopDevices();
  if (!preAllocate.Ok()) {
    LOG(ERROR) << "Failed to pre-allocate loop devices : "
               << preAllocate.ErrorMessage();
  }

  Status status = collectApexKeys();
  if (!status.Ok()) {
    LOG(ERROR) << "Failed to collect APEX keys : " << status.ErrorMessage();
    return 1;
  }

  // Activate built-in APEXes for processes launched before /data is mounted.
  status = scanPackagesDirAndActivate(kApexPackageSystemDir);
  if (!status.Ok()) {
    LOG(ERROR) << "Failed to activate APEX files in " << kApexPackageSystemDir
               << " : " << status.ErrorMessage();
    return 1;
  }
  LOG(INFO) << "Bootstrapping done";
  return 0;
}

void onStart(CheckpointInterface* checkpoint_service) {
  LOG(INFO) << "Marking APEXd as starting";
  if (!android::base::SetProperty(kApexStatusSysprop, kApexStatusStarting)) {
    PLOG(ERROR) << "Failed to set " << kApexStatusSysprop << " to "
                << kApexStatusStarting;
  }

  if (checkpoint_service != nullptr) {
    gVoldService = checkpoint_service;
    StatusOr<bool> supports_fs_checkpoints =
        gVoldService->SupportsFsCheckpoints();
    if (supports_fs_checkpoints.Ok()) {
      gSupportsFsCheckpoints = *supports_fs_checkpoints;
    } else {
      LOG(ERROR) << "Failed to check if filesystem checkpoints are supported: "
                 << supports_fs_checkpoints.ErrorMessage();
    }
    if (gSupportsFsCheckpoints) {
      StatusOr<bool> needs_checkpoint = gVoldService->NeedsCheckpoint();
      if (needs_checkpoint.Ok()) {
        gInFsCheckpointMode = *needs_checkpoint;
      } else {
        LOG(ERROR) << "Failed to check if we're in filesystem checkpoint mode: "
                   << needs_checkpoint.ErrorMessage();
      }
    }
  }

  // Ask whether we should roll back any staged sessions; this can happen if
  // we've exceeded the retry count on a device that supports filesystem
  // checkpointing.
  if (gSupportsFsCheckpoints) {
    StatusOr<bool> needs_rollback = gVoldService->NeedsRollback();
    if (!needs_rollback.Ok()) {
      LOG(ERROR) << "Failed to check if we need a rollback: "
                 << needs_rollback.ErrorMessage();
    } else if (*needs_rollback) {
      LOG(INFO) << "Exceeded number of session retries ("
                << kNumRetriesWhenCheckpointingEnabled
                << "). Starting a rollback";
      Status status = rollbackStagedSessionIfAny();
      if (!status.Ok()) {
        LOG(ERROR)
            << "Failed to roll back (as requested by fs checkpointing) : "
            << status.ErrorMessage();
      }
    }
  }

  Status status = collectApexKeys();
  if (!status.Ok()) {
    LOG(ERROR) << "Failed to collect APEX keys : " << status.ErrorMessage();
    return;
  }

  gMountedApexes.PopulateFromMounts();

  // Activate APEXes from /data/apex. If one in the directory is newer than the
  // system one, the new one will eclipse the old one.
  scanStagedSessionsDirAndStage();
  status = resumeRollbackIfNeeded();
  if (!status.Ok()) {
    LOG(ERROR) << "Failed to resume rollback : " << status.ErrorMessage();
  }

  status = scanPackagesDirAndActivate(kActiveApexPackagesDataDir);
  if (!status.Ok()) {
    LOG(ERROR) << "Failed to activate packages from "
               << kActiveApexPackagesDataDir << " : " << status.ErrorMessage();
    Status rollback_status = rollbackActiveSessionAndReboot();
    if (!rollback_status.Ok()) {
      // TODO: should we kill apexd in this case?
      LOG(ERROR) << "Failed to rollback : " << rollback_status.ErrorMessage();
    }
  }

  for (const auto& dir : kApexPackageBuiltinDirs) {
    // TODO(b/123622800): if activation failed, rollback and reboot.
    status = scanPackagesDirAndActivate(dir.c_str());
    if (!status.Ok()) {
      // This should never happen. Like **really** never.
      // TODO: should we kill apexd in this case?
      LOG(ERROR) << "Failed to activate packages from " << dir << " : "
                 << status.ErrorMessage();
    }
  }
}

void onAllPackagesReady() {
  // Set a system property to let other components to know that APEXs are
  // correctly mounted and ready to be used. Before using any file from APEXs,
  // they can query this system property to ensure that they are okay to
  // access. Or they may have a on-property trigger to delay a task until
  // APEXs become ready.
  LOG(INFO) << "Marking APEXd as ready";
  if (!android::base::SetProperty(kApexStatusSysprop, kApexStatusReady)) {
    PLOG(ERROR) << "Failed to set " << kApexStatusSysprop << " to "
                << kApexStatusReady;
  }
}

StatusOr<std::vector<ApexFile>> submitStagedSession(
    const int session_id, const std::vector<int>& child_session_ids) {
  using android::base::GetProperty;
  bool needsBackup = true;
  Status cleanup_status = ClearSessions();
  if (!cleanup_status.Ok()) {
    return StatusOr<std::vector<ApexFile>>::MakeError(cleanup_status);
  }

  if (gSupportsFsCheckpoints) {
    Status checkpoint_status =
        gVoldService->StartCheckpoint(kNumRetriesWhenCheckpointingEnabled);
    if (!checkpoint_status.Ok()) {
      // The device supports checkpointing, but we could not start it;
      // log a warning, but do continue, since we can live without it.
      LOG(WARNING) << "Failed to start filesystem checkpoint on device that "
                      "should support it: "
                   << checkpoint_status.ErrorMessage();
    } else {
      needsBackup = false;
    }
  }

  if (needsBackup) {
    Status backup_status = BackupActivePackages();
    if (!backup_status.Ok()) {
      return StatusOr<std::vector<ApexFile>>::MakeError(backup_status);
    }
  }

  std::vector<int> ids_to_scan;
  if (!child_session_ids.empty()) {
    ids_to_scan = child_session_ids;
  } else {
    ids_to_scan = {session_id};
  }

  std::vector<ApexFile> ret;
  for (int id_to_scan : ids_to_scan) {
    auto verified = verifySessionDir(id_to_scan);
    if (!verified.Ok()) {
      return StatusOr<std::vector<ApexFile>>::MakeError(verified.ErrorStatus());
    }
    ret.push_back(std::move(*verified));
  }

  // Run preinstall, if necessary.
  Status preinstall_status = PreinstallPackages(ret);
  if (!preinstall_status.Ok()) {
    return StatusOr<std::vector<ApexFile>>::MakeError(preinstall_status);
  }

  auto session = ApexSession::CreateSession(session_id);
  if (!session.Ok()) {
    return StatusOr<std::vector<ApexFile>>::MakeError(session.ErrorMessage());
  }
  (*session).SetChildSessionIds(child_session_ids);
  std::string build_fingerprint = GetProperty(kBuildFingerprintSysprop, "");
  (*session).SetBuildFingerprint(build_fingerprint);
  Status commit_status =
      (*session).UpdateStateAndCommit(SessionState::VERIFIED);
  if (!commit_status.Ok()) {
    return StatusOr<std::vector<ApexFile>>::MakeError(commit_status);
  }

  return StatusOr<std::vector<ApexFile>>(std::move(ret));
}

Status markStagedSessionReady(const int session_id) {
  auto session = ApexSession::GetSession(session_id);
  if (!session.Ok()) {
    return session.ErrorStatus();
  }
  // We should only accept sessions in SessionState::VERIFIED or
  // SessionState::STAGED state. In the SessionState::STAGED case, this
  // function is effectively a no-op.
  auto session_state = (*session).GetState();
  if (session_state == SessionState::STAGED) {
    return Status::Success();
  }
  if (session_state == SessionState::VERIFIED) {
    return (*session).UpdateStateAndCommit(SessionState::STAGED);
  }
  return Status::Fail(StringLog() << "Invalid state for session " << session_id
                                  << ". Cannot mark it as ready.");
}

Status markStagedSessionSuccessful(const int session_id) {
  auto session = ApexSession::GetSession(session_id);
  if (!session.Ok()) {
    return session.ErrorStatus();
  }
  // Only SessionState::ACTIVATED or SessionState::SUCCESS states are accepted.
  // In the SessionState::SUCCESS state, this function is a no-op.
  if (session->GetState() == SessionState::SUCCESS) {
    return Status::Success();
  } else if (session->GetState() == SessionState::ACTIVATED) {
    auto cleanup_status = DeleteBackup();
    if (!cleanup_status.Ok()) {
      return Status::Fail(StringLog() << "Failed to mark session " << *session
                                      << " as successful : "
                                      << cleanup_status.ErrorMessage());
    }
    return session->UpdateStateAndCommit(SessionState::SUCCESS);
  } else {
    return Status::Fail(StringLog() << "Session " << *session
                                    << " can not be marked successful");
  }
}

// Find dangling mounts and unmount them.
// If one is on /data/apex/active, remove it.
void unmountDanglingMounts() {
  std::multimap<std::string, MountedApexData> danglings;
  gMountedApexes.ForallMountedApexes([&](const std::string& package,
                                         const MountedApexData& data,
                                         bool latest) {
    if (!latest) {
      danglings.insert({package, data});
    }
  });

  for (const auto& [package, data] : danglings) {
    const std::string& path = data.full_path;
    LOG(VERBOSE) << "Unmounting " << data.mount_point;
    gMountedApexes.RemoveMountedApex(package, path);
    if (auto st = Unmount(data); !st.Ok()) {
      LOG(ERROR) << st.ErrorMessage();
    }
    if (StartsWith(path, kActiveApexPackagesDataDir)) {
      LOG(VERBOSE) << "Deleting old APEX " << path;
      if (unlink(path.c_str()) != 0) {
        PLOG(ERROR) << "Failed to delete " << path;
      }
    }
  }
}

}  // namespace apex
}  // namespace android
