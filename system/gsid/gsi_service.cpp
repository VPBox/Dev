/*
 * Copyright (C) 2019 The Android Open Source Project
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

#include "gsi_service.h"

#include <errno.h>
#include <linux/fs.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <sys/types.h>
#include <sys/vfs.h>
#include <unistd.h>

#include <chrono>
#include <string>
#include <vector>

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/stringprintf.h>
#include <android-base/strings.h>
#include <android/gsi/IGsiService.h>
#include <ext4_utils/ext4_utils.h>
#include <fs_mgr.h>
#include <fs_mgr_dm_linear.h>
#include <fstab/fstab.h>
#include <libdm/dm.h>
#include <libfiemap_writer/fiemap_writer.h>
#include <logwrap/logwrap.h>
#include <private/android_filesystem_config.h>

#include "file_paths.h"
#include "libgsi_private.h"

namespace android {
namespace gsi {

using namespace std::literals;
using namespace android::dm;
using namespace android::fs_mgr;
using namespace android::fiemap_writer;
using android::base::StringPrintf;
using android::base::unique_fd;

static constexpr char kUserdataDevice[] = "/dev/block/by-name/userdata";

// The default size of userdata.img for GSI.
// We are looking for /data to have atleast 40% free space
static constexpr uint32_t kMinimumFreeSpaceThreshold = 40;
// We determine the fragmentation by making sure the files
// we create don't have more than 16 extents.
static constexpr uint32_t kMaximumExtents = 512;
// Default userdata image size.
static constexpr int64_t kDefaultUserdataSize = int64_t(8) * 1024 * 1024 * 1024;
static constexpr std::chrono::milliseconds kDmTimeout = 5000ms;

void GsiService::Register() {
    auto ret = android::BinderService<GsiService>::publish();
    if (ret != android::OK) {
        LOG(FATAL) << "Could not register gsi service: " << ret;
    }
}

GsiService::GsiService() {
    progress_ = {};
}

GsiService::~GsiService() {
    PostInstallCleanup();
}

#define ENFORCE_SYSTEM                          \
    do {                                        \
        binder::Status status = CheckUid();     \
        if (!status.isOk()) return status;      \
    } while (0)

#define ENFORCE_SYSTEM_OR_SHELL                                         \
    do {                                                                \
        binder::Status status = CheckUid(AccessLevel::SystemOrShell);   \
        if (!status.isOk()) return status;                              \
    } while (0)

binder::Status GsiService::startGsiInstall(int64_t gsiSize, int64_t userdataSize, bool wipeUserdata,
                                           int* _aidl_return) {
    GsiInstallParams params;
    params.gsiSize = gsiSize;
    params.userdataSize = userdataSize;
    params.wipeUserdata = wipeUserdata;
    return beginGsiInstall(params, _aidl_return);
}

binder::Status GsiService::beginGsiInstall(const GsiInstallParams& given_params, int* _aidl_return) {
    ENFORCE_SYSTEM;
    std::lock_guard<std::mutex> guard(main_lock_);

    // Make sure any interrupted installations are cleaned up.
    PostInstallCleanup();

    // Do some precursor validation on the arguments before diving into the
    // install process.
    GsiInstallParams params = given_params;
    if (int status = ValidateInstallParams(&params)) {
        *_aidl_return = status;
        return binder::Status::ok();
    }

    int status = StartInstall(params);
    if (status != INSTALL_OK) {
        // Perform local cleanup and delete any lingering files.
        PostInstallCleanup();
        RemoveGsiFiles(params.installDir, wipe_userdata_on_failure_);
    }
    *_aidl_return = status;

    // Clear the progress indicator.
    UpdateProgress(STATUS_NO_OPERATION, 0);
    return binder::Status::ok();
}

binder::Status GsiService::commitGsiChunkFromStream(const android::os::ParcelFileDescriptor& stream,
                                                    int64_t bytes, bool* _aidl_return) {
    ENFORCE_SYSTEM;
    std::lock_guard<std::mutex> guard(main_lock_);

    *_aidl_return = CommitGsiChunk(stream.get(), bytes);

    // Clear the progress indicator.
    UpdateProgress(STATUS_NO_OPERATION, 0);
    return binder::Status::ok();
}

void GsiService::StartAsyncOperation(const std::string& step, int64_t total_bytes) {
    std::lock_guard<std::mutex> guard(progress_lock_);

    progress_.step = step;
    progress_.status = STATUS_WORKING;
    progress_.bytes_processed = 0;
    progress_.total_bytes = total_bytes;
}

void GsiService::UpdateProgress(int status, int64_t bytes_processed) {
    std::lock_guard<std::mutex> guard(progress_lock_);

    progress_.status = status;
    if (status == STATUS_COMPLETE) {
        progress_.bytes_processed = progress_.total_bytes;
    } else {
        progress_.bytes_processed = bytes_processed;
    }
}

binder::Status GsiService::getInstallProgress(::android::gsi::GsiProgress* _aidl_return) {
    ENFORCE_SYSTEM;
    std::lock_guard<std::mutex> guard(progress_lock_);

    *_aidl_return = progress_;
    return binder::Status::ok();
}

binder::Status GsiService::commitGsiChunkFromMemory(const std::vector<uint8_t>& bytes,
                                                    bool* _aidl_return) {
    ENFORCE_SYSTEM;
    std::lock_guard<std::mutex> guard(main_lock_);

    *_aidl_return = CommitGsiChunk(bytes.data(), bytes.size());
    return binder::Status::ok();
}

binder::Status GsiService::setGsiBootable(bool one_shot, int* _aidl_return) {
    std::lock_guard<std::mutex> guard(main_lock_);

    if (installing_) {
        ENFORCE_SYSTEM;
        int error = SetGsiBootable(one_shot);
        PostInstallCleanup();
        if (error) {
            RemoveGsiFiles(install_dir_, wipe_userdata_on_failure_);
            *_aidl_return = error;
        } else {
            *_aidl_return = INSTALL_OK;
        }
    } else {
        ENFORCE_SYSTEM_OR_SHELL;
        *_aidl_return = ReenableGsi(one_shot);
        PostInstallCleanup();
    }

    return binder::Status::ok();
}

binder::Status GsiService::isGsiEnabled(bool* _aidl_return) {
    ENFORCE_SYSTEM_OR_SHELL;
    std::lock_guard<std::mutex> guard(main_lock_);
    std::string boot_key;
    if (!GetInstallStatus(&boot_key)) {
        *_aidl_return = false;
    } else {
        *_aidl_return = (boot_key == kInstallStatusOk);
    }
    return binder::Status::ok();
}

binder::Status GsiService::removeGsiInstall(bool* _aidl_return) {
    ENFORCE_SYSTEM_OR_SHELL;
    std::lock_guard<std::mutex> guard(main_lock_);

    // Just in case an install was left hanging.
    std::string install_dir;
    if (installing_) {
        install_dir = install_dir_;
        PostInstallCleanup();
    } else {
        install_dir = GetInstalledImageDir();
    }

    if (IsGsiRunning()) {
        // Can't remove gsi files while running.
        *_aidl_return = UninstallGsi();
    } else {
        *_aidl_return = RemoveGsiFiles(install_dir, true /* wipeUserdata */);
    }
    return binder::Status::ok();
}

binder::Status GsiService::disableGsiInstall(bool* _aidl_return) {
    ENFORCE_SYSTEM_OR_SHELL;
    std::lock_guard<std::mutex> guard(main_lock_);

    *_aidl_return = DisableGsiInstall();
    return binder::Status::ok();
}

binder::Status GsiService::isGsiRunning(bool* _aidl_return) {
    ENFORCE_SYSTEM_OR_SHELL;
    std::lock_guard<std::mutex> guard(main_lock_);

    *_aidl_return = IsGsiRunning();
    return binder::Status::ok();
}

binder::Status GsiService::isGsiInstalled(bool* _aidl_return) {
    ENFORCE_SYSTEM_OR_SHELL;
    std::lock_guard<std::mutex> guard(main_lock_);

    *_aidl_return = IsGsiInstalled();
    return binder::Status::ok();
}

binder::Status GsiService::isGsiInstallInProgress(bool* _aidl_return) {
    ENFORCE_SYSTEM_OR_SHELL;
    std::lock_guard<std::mutex> guard(main_lock_);

    *_aidl_return = installing_;
    return binder::Status::ok();
}

binder::Status GsiService::cancelGsiInstall(bool* _aidl_return) {
    ENFORCE_SYSTEM;
    should_abort_ = true;
    std::lock_guard<std::mutex> guard(main_lock_);

    should_abort_ = false;
    if (installing_) {
        PostInstallCleanup();
        RemoveGsiFiles(install_dir_, wipe_userdata_on_failure_);
    }

    *_aidl_return = true;
    return binder::Status::ok();
}

binder::Status GsiService::getGsiBootStatus(int* _aidl_return) {
    ENFORCE_SYSTEM_OR_SHELL;
    std::lock_guard<std::mutex> guard(main_lock_);

    if (!IsGsiInstalled()) {
        *_aidl_return = BOOT_STATUS_NOT_INSTALLED;
        return binder::Status::ok();
    }

    std::string boot_key;
    if (!GetInstallStatus(&boot_key)) {
        PLOG(ERROR) << "read " << kGsiInstallStatusFile;
        *_aidl_return = BOOT_STATUS_NOT_INSTALLED;
        return binder::Status::ok();
    }

    bool single_boot = !access(kGsiOneShotBootFile, F_OK);

    if (boot_key == kInstallStatusWipe) {
        // This overrides all other statuses.
        *_aidl_return = BOOT_STATUS_WILL_WIPE;
    } else if (boot_key == kInstallStatusDisabled) {
        // A single-boot GSI will have a "disabled" status, because it's
        // disabled immediately upon reading the one_shot_boot file. However,
        // we still want to return SINGLE_BOOT, because it makes the
        // transition clearer to the user.
        if (single_boot) {
            *_aidl_return = BOOT_STATUS_SINGLE_BOOT;
        } else {
            *_aidl_return = BOOT_STATUS_DISABLED;
        }
    } else if (single_boot) {
        *_aidl_return = BOOT_STATUS_SINGLE_BOOT;
    } else {
        *_aidl_return = BOOT_STATUS_ENABLED;
    }
    return binder::Status::ok();
}

binder::Status GsiService::getUserdataImageSize(int64_t* _aidl_return) {
    ENFORCE_SYSTEM;
    std::lock_guard<std::mutex> guard(main_lock_);

    *_aidl_return = -1;

    if (installing_) {
        // Size has already been computed.
        *_aidl_return = userdata_size_;
    } else if (IsGsiRunning()) {
        // :TODO: libdm
        unique_fd fd(open(kUserdataDevice, O_RDONLY | O_NOFOLLOW | O_CLOEXEC));
        if (fd < 0) {
            PLOG(ERROR) << "open " << kUserdataDevice;
            return binder::Status::ok();
        }

        int64_t size;
        if (ioctl(fd, BLKGETSIZE64, &size)) {
            PLOG(ERROR) << "BLKGETSIZE64 " << kUserdataDevice;
            return binder::Status::ok();
        }
        *_aidl_return = size;
    } else {
        // Stat the size of the userdata file.
        auto userdata_gsi = GetInstalledImagePath("userdata_gsi");
        struct stat s;
        if (stat(userdata_gsi.c_str(), &s)) {
            if (errno != ENOENT) {
                PLOG(ERROR) << "open " << userdata_gsi;
                return binder::Status::ok();
            }
            *_aidl_return = 0;
        } else {
            *_aidl_return = s.st_size;
        }
    }
    return binder::Status::ok();
}

binder::Status GsiService::getInstalledGsiImageDir(std::string* _aidl_return) {
    ENFORCE_SYSTEM;
    std::lock_guard<std::mutex> guard(main_lock_);

    if (IsGsiInstalled()) {
        *_aidl_return = GetInstalledImageDir();
    }
    return binder::Status::ok();
}

binder::Status GsiService::wipeGsiUserdata(int* _aidl_return) {
    ENFORCE_SYSTEM_OR_SHELL;
    std::lock_guard<std::mutex> guard(main_lock_);

    if (IsGsiRunning() || !IsGsiInstalled()) {
        *_aidl_return = IGsiService::INSTALL_ERROR_GENERIC;
        return binder::Status::ok();
    }

    *_aidl_return = WipeUserdata();

    return binder::Status::ok();
}

binder::Status GsiService::CheckUid(AccessLevel level) {
    std::vector<uid_t> allowed_uids{AID_ROOT, AID_SYSTEM};
    if (level == AccessLevel::SystemOrShell) {
        allowed_uids.push_back(AID_SHELL);
    }

    uid_t uid = IPCThreadState::self()->getCallingUid();
    for (const auto& allowed_uid : allowed_uids) {
        if (allowed_uid == uid) {
            return binder::Status::ok();
        }
    }

    auto message = StringPrintf("UID %d is not allowed", uid);
    return binder::Status::fromExceptionCode(binder::Status::EX_SECURITY,
                                             String8(message.c_str()));
}

void GsiService::PostInstallCleanup() {
    // This must be closed before unmapping partitions.
    system_writer_ = nullptr;

    const auto& dm = DeviceMapper::Instance();
    if (dm.GetState("userdata_gsi") != DmDeviceState::INVALID) {
        DestroyLogicalPartition("userdata_gsi", kDmTimeout);
    }
    if (dm.GetState("system_gsi") != DmDeviceState::INVALID) {
        DestroyLogicalPartition("system_gsi", kDmTimeout);
    }

    installing_ = false;
    partitions_ .clear();
}

static bool IsExternalStoragePath(const std::string& path) {
    if (!android::base::StartsWith(path, "/mnt/media_rw/")) {
        return false;
    }
    unique_fd fd(open(path.c_str(), O_RDONLY | O_CLOEXEC | O_NOFOLLOW));
    if (fd < 0) {
        PLOG(ERROR) << "open failed: " << path;
        return false;
    }
    struct statfs info;
    if (fstatfs(fd, &info)) {
        PLOG(ERROR) << "statfs failed: " << path;
        return false;
    }
    LOG(ERROR) << "fs type: " << info.f_type;
    return info.f_type == MSDOS_SUPER_MAGIC;
}

int GsiService::ValidateInstallParams(GsiInstallParams* params) {
    // If no install path was specified, use the default path. We also allow
    // specifying the top-level folder, and then we choose the correct location
    // underneath.
    if (params->installDir.empty() || params->installDir == "/data/gsi") {
        params->installDir = kDefaultGsiImageFolder;
    }

    // Normalize the path and add a trailing slash.
    std::string origInstallDir = params->installDir;
    if (!android::base::Realpath(origInstallDir, &params->installDir)) {
        PLOG(ERROR) << "realpath failed: " << origInstallDir;
        return INSTALL_ERROR_GENERIC;
    }
    // Ensure the path ends in / for consistency. Even though GetImagePath()
    // does this already, we want it to appear this way in install_dir.
    if (!android::base::EndsWith(params->installDir, "/")) {
        params->installDir += "/";
    }

    // Currently, we can only install to /data/gsi/ or external storage.
    if (IsExternalStoragePath(params->installDir)) {
        Fstab fstab;
        if (!ReadDefaultFstab(&fstab)) {
            LOG(ERROR) << "cannot read default fstab";
            return INSTALL_ERROR_GENERIC;
        }
        FstabEntry* system = GetEntryForMountPoint(&fstab, "/system");
        if (!system) {
            LOG(ERROR) << "cannot find /system fstab entry";
            return INSTALL_ERROR_GENERIC;
        }
        if (fs_mgr_verity_is_check_at_most_once(*system)) {
            LOG(ERROR) << "cannot install GSIs to external media if verity uses check_at_most_once";
            return INSTALL_ERROR_GENERIC;
        }
    } else if (params->installDir != kDefaultGsiImageFolder) {
        LOG(ERROR) << "cannot install GSI to " << params->installDir;
        return INSTALL_ERROR_GENERIC;
    }

    if (params->gsiSize % LP_SECTOR_SIZE) {
        LOG(ERROR) << "GSI size " << params->gsiSize << " is not a multiple of " << LP_SECTOR_SIZE;
        return INSTALL_ERROR_GENERIC;
    }
    if (params->userdataSize % LP_SECTOR_SIZE) {
        LOG(ERROR) << "userdata size " << params->userdataSize << " is not a multiple of "
                   << LP_SECTOR_SIZE;
        return INSTALL_ERROR_GENERIC;
    }
    return INSTALL_OK;
}

int GsiService::StartInstall(const GsiInstallParams& params) {
    installing_ = true;
    userdata_block_size_ = 0;
    system_block_size_ = 0;
    gsi_size_ = params.gsiSize;
    userdata_size_ = (params.userdataSize) ? params.userdataSize : kDefaultUserdataSize;
    wipe_userdata_ = params.wipeUserdata;
    can_use_devicemapper_ = false;
    gsi_bytes_written_ = 0;
    install_dir_ = params.installDir;

    userdata_gsi_path_ = GetImagePath(install_dir_, "userdata_gsi");
    system_gsi_path_ = GetImagePath(install_dir_, "system_gsi");

    // Only rm userdata_gsi if one didn't already exist.
    wipe_userdata_on_failure_ = wipe_userdata_ || access(userdata_gsi_path_.c_str(), F_OK);

    if (int status = PerformSanityChecks()) {
        return status;
    }
    if (int status = PreallocateFiles()) {
        return status;
    }
    if (int status = DetermineReadWriteMethod()) {
        return status;
    }
    if (!FormatUserdata()) {
        return INSTALL_ERROR_GENERIC;
    }

    // Map system_gsi so we can write to it.
    system_writer_ = OpenPartition("system_gsi");
    if (!system_writer_) {
        return INSTALL_ERROR_GENERIC;
    }
    return INSTALL_OK;
}

int GsiService::DetermineReadWriteMethod() {
    // If there is a device-mapper node wrapping the block device, then we're
    // able to create another node around it; the dm layer does not carry the
    // exclusion lock down the stack when a mount occurs.
    //
    // If there is no intermediate device-mapper node, then partitions cannot be
    // opened writable due to sepolicy and exclusivity of having a mounted
    // filesystem. This should only happen on devices with no encryption, or
    // devices with FBE and no metadata encryption. For these cases it suffices
    // to perform normal file writes to /data/gsi (which is unencrypted).
    std::string block_device;
    if (!FiemapWriter::GetBlockDeviceForFile(system_gsi_path_.c_str(), &block_device,
                                             &can_use_devicemapper_)) {
        return INSTALL_ERROR_GENERIC;
    }
    if (install_dir_ != kDefaultGsiImageFolder && can_use_devicemapper_) {
        // Never use device-mapper on external media. We don't support adopted
        // storage yet, and accidentally using device-mapper could be dangerous
        // as we hardcode the userdata device as backing storage.
        LOG(ERROR) << "unexpected device-mapper node used to mount external media";
        return INSTALL_ERROR_GENERIC;
    }
    return INSTALL_OK;
}

std::string GsiService::GetImagePath(const std::string& image_dir, const std::string& name) {
    std::string dir = image_dir;
    if (!android::base::EndsWith(dir, "/")) {
        dir += "/";
    }
    return dir + name + ".img";
}

std::string GsiService::GetInstalledImageDir() {
    // If there's no install left, just return /data/gsi since that's where
    // installs go by default.
    std::string dir;
    if (android::base::ReadFileToString(kGsiInstallDirFile, &dir)) {
        return dir;
    }
    return kDefaultGsiImageFolder;
}

std::string GsiService::GetInstalledImagePath(const std::string& name) {
    return GetImagePath(GetInstalledImageDir(), name);
}

int GsiService::PerformSanityChecks() {
    if (gsi_size_ < 0) {
        LOG(ERROR) << "image size " << gsi_size_ << " is negative";
        return INSTALL_ERROR_GENERIC;
    }
    if (android::gsi::IsGsiRunning()) {
        LOG(ERROR) << "cannot install gsi inside a live gsi";
        return INSTALL_ERROR_GENERIC;
    }

    struct statvfs sb;
    if (statvfs(install_dir_.c_str(), &sb)) {
        PLOG(ERROR) << "failed to read file system stats";
        return INSTALL_ERROR_GENERIC;
    }

    // This is the same as android::vold::GetFreebytes() but we also
    // need the total file system size so we open code it here.
    uint64_t free_space = 1ULL * sb.f_bavail * sb.f_frsize;
    uint64_t fs_size = sb.f_blocks * sb.f_frsize;
    if (free_space <= (gsi_size_ + userdata_size_)) {
        LOG(ERROR) << "not enough free space (only " << free_space << " bytes available)";
        return INSTALL_ERROR_NO_SPACE;
    }
    // We are asking for 40% of the /data to be empty.
    // TODO: may be not hard code it like this
    double free_space_percent = ((1.0 * free_space) / fs_size) * 100;
    if (free_space_percent < kMinimumFreeSpaceThreshold) {
        LOG(ERROR) << "free space " << static_cast<uint64_t>(free_space_percent)
                   << "% is below the minimum threshold of " << kMinimumFreeSpaceThreshold << "%";
        return INSTALL_ERROR_FILE_SYSTEM_CLUTTERED;
    }
    return INSTALL_OK;
}

int GsiService::PreallocateFiles() {
    if (wipe_userdata_) {
        SplitFiemap::RemoveSplitFiles(userdata_gsi_path_);
    }
    SplitFiemap::RemoveSplitFiles(system_gsi_path_);

    // TODO: trigger GC from fiemap writer.

    // Create fallocated files.
    if (int status = PreallocateUserdata()) {
        return status;
    }
    if (int status = PreallocateSystem()) {
        return status;
    }

    // Save the extent information in liblp.
    metadata_ = CreateMetadata();
    if (!metadata_) {
        return INSTALL_ERROR_GENERIC;
    }

    UpdateProgress(STATUS_COMPLETE, 0);
    return INSTALL_OK;
}

int GsiService::PreallocateUserdata() {
    int error;
    std::unique_ptr<SplitFiemap> userdata_image;
    if (wipe_userdata_ || access(userdata_gsi_path_.c_str(), F_OK)) {
        StartAsyncOperation("create userdata", userdata_size_);
        userdata_image = CreateFiemapWriter(userdata_gsi_path_, userdata_size_, &error);
        if (!userdata_image) {
            LOG(ERROR) << "Could not create userdata image: " << userdata_gsi_path_;
            return error;
        }
        // Signal that we need to reformat userdata.
        wipe_userdata_ = true;
    } else {
        userdata_image = CreateFiemapWriter(userdata_gsi_path_, 0, &error);
        if (!userdata_image) {
            LOG(ERROR) << "Could not open userdata image: " << userdata_gsi_path_;
            return error;
        }
        if (userdata_size_ && userdata_image->size() < userdata_size_) {
            // :TODO: need to fallocate more blocks and resizefs.
        }
        userdata_size_ = userdata_image->size();
    }

    userdata_block_size_ = userdata_image->block_size();

    Image image = {
            .writer = std::move(userdata_image),
            .actual_size = userdata_size_,
    };
    partitions_.emplace(std::make_pair("userdata_gsi", std::move(image)));
    return INSTALL_OK;
}

int GsiService::PreallocateSystem() {
    StartAsyncOperation("create system", gsi_size_);

    int error;
    auto system_image = CreateFiemapWriter(system_gsi_path_, gsi_size_, &error);
    if (!system_image) {
        return error;
    }

    system_block_size_ = system_image->block_size();

    Image image = {
            .writer = std::move(system_image),
            .actual_size = gsi_size_,
    };
    partitions_.emplace(std::make_pair("system_gsi", std::move(image)));
    return INSTALL_OK;
}

std::unique_ptr<SplitFiemap> GsiService::CreateFiemapWriter(const std::string& path,
                                                            uint64_t size, int* error) {
    bool create = (size != 0);

    std::function<bool(uint64_t, uint64_t)> progress;
    if (create) {
        // TODO: allow cancelling inside cancelGsiInstall.
        progress = [this](uint64_t bytes, uint64_t /* total */) -> bool {
            UpdateProgress(STATUS_WORKING, bytes);
            if (should_abort_) return false;
            return true;
        };
    }

    std::unique_ptr<SplitFiemap> file;
    if (!size) {
        file = SplitFiemap::Open(path);
    } else {
        file = SplitFiemap::Create(path, size, 0, std::move(progress));
    }
    if (!file) {
        LOG(ERROR) << "failed to create or open " << path;
        *error = INSTALL_ERROR_GENERIC;
        return nullptr;
    }

    uint64_t extents = file->extents().size();
    if (extents > kMaximumExtents) {
        LOG(ERROR) << "file " << path << " has too many extents: " << extents;
        *error = INSTALL_ERROR_FILE_SYSTEM_CLUTTERED;
        return nullptr;
    }
    return file;
}

// Write data through an fd.
class FdWriter final : public GsiService::WriteHelper {
  public:
    FdWriter(const std::string& path, unique_fd&& fd) : path_(path), fd_(std::move(fd)) {}

    bool Write(const void* data, uint64_t bytes) override {
        return android::base::WriteFully(fd_, data, bytes);
    }
    bool Flush() override {
        if (fsync(fd_)) {
            PLOG(ERROR) << "fsync failed: " << path_;
            return false;
        }
        return true;
    }
    uint64_t Size() override { return get_block_device_size(fd_); }

  private:
    std::string path_;
    unique_fd fd_;
};

// Write data through a SplitFiemap.
class SplitFiemapWriter final : public GsiService::WriteHelper {
  public:
    explicit SplitFiemapWriter(SplitFiemap* writer) : writer_(writer) {}

    bool Write(const void* data, uint64_t bytes) override {
        return writer_->Write(data, bytes);
    }
    bool Flush() override {
        return writer_->Flush();
    }
    uint64_t Size() override { return writer_->size(); }

  private:
    SplitFiemap* writer_;
};

std::unique_ptr<GsiService::WriteHelper> GsiService::OpenPartition(const std::string& name) {
    if (can_use_devicemapper_) {
        std::string path;
        if (!CreateLogicalPartition(kUserdataDevice, *metadata_.get(), name, true, kDmTimeout,
                                    &path)) {
            LOG(ERROR) << "Error creating device-mapper node for " << name;
            return {};
        }

        static const int kOpenFlags = O_RDWR | O_NOFOLLOW | O_CLOEXEC;
        unique_fd fd(open(path.c_str(), kOpenFlags));
        if (fd < 0) {
            PLOG(ERROR) << "could not open " << path;
        }
        return std::make_unique<FdWriter>(GetImagePath(install_dir_, name), std::move(fd));
    }

    auto iter = partitions_.find(name);
    if (iter == partitions_.end()) {
        LOG(ERROR) << "could not find partition " << name;
        return {};
    }
    return std::make_unique<SplitFiemapWriter>(iter->second.writer.get());
}

bool GsiService::CommitGsiChunk(int stream_fd, int64_t bytes) {
    StartAsyncOperation("write gsi", gsi_size_);

    if (bytes < 0) {
        LOG(ERROR) << "chunk size " << bytes << " is negative";
        return false;
    }

    auto buffer = std::make_unique<char[]>(system_block_size_);

    int progress = -1;
    uint64_t remaining = bytes;
    while (remaining) {
        // :TODO: check file pin status!
        size_t max_to_read = std::min(system_block_size_, remaining);
        ssize_t rv = TEMP_FAILURE_RETRY(read(stream_fd, buffer.get(), max_to_read));
        if (rv < 0) {
            PLOG(ERROR) << "read gsi chunk";
            return false;
        }
        if (rv == 0) {
            LOG(ERROR) << "no bytes left in stream";
            return false;
        }
        if (!CommitGsiChunk(buffer.get(), rv)) {
            return false;
        }
        CHECK(static_cast<uint64_t>(rv) <= remaining);
        remaining -= rv;

        // Only update the progress when the % (or permille, in this case)
        // significantly changes.
        int new_progress = ((gsi_size_ - remaining) * 1000) / gsi_size_;
        if (new_progress != progress) {
            UpdateProgress(STATUS_WORKING, gsi_size_ - remaining);
        }
    }

    UpdateProgress(STATUS_COMPLETE, gsi_size_);
    return true;
}

bool GsiService::CommitGsiChunk(const void* data, size_t bytes) {
    if (!installing_) {
        LOG(ERROR) << "no gsi installation in progress";
        return false;
    }
    if (static_cast<uint64_t>(bytes) > gsi_size_ - gsi_bytes_written_) {
        // We cannot write past the end of the image file.
        LOG(ERROR) << "chunk size " << bytes << " exceeds remaining image size (" << gsi_size_
                   << " expected, " << gsi_bytes_written_ << " written)";
        return false;
    }

    if (!system_writer_->Write(data, bytes)) {
        PLOG(ERROR) << "write failed";
        return false;
    }
    gsi_bytes_written_ += bytes;
    return true;
}

int GsiService::SetGsiBootable(bool one_shot) {
    if (gsi_bytes_written_ != gsi_size_) {
        // We cannot boot if the image is incomplete.
        LOG(ERROR) << "image incomplete; expected " << gsi_size_ << " bytes, waiting for "
                   << (gsi_size_ - gsi_bytes_written_) << " bytes";
        return INSTALL_ERROR_GENERIC;
    }

    if (!system_writer_->Flush()) {
        return INSTALL_ERROR_GENERIC;
    }

    // If files moved (are no longer pinned), the metadata file will be invalid.
    for (const auto& [name, image] : partitions_) {
        if (!image.writer->HasPinnedExtents()) {
            LOG(ERROR) << name << " no longer has pinned extents";
            return INSTALL_ERROR_GENERIC;
        }
    }

    // Remember the installation directory.
    if (!android::base::WriteStringToFile(install_dir_, kGsiInstallDirFile)) {
        PLOG(ERROR) << "write failed: " << kGsiInstallDirFile;
        return INSTALL_ERROR_GENERIC;
    }

    // Note: create the install status file last, since this is the actual boot
    // indicator.
    if (!CreateMetadataFile() || !SetBootMode(one_shot) || !CreateInstallStatusFile()) {
        return INSTALL_ERROR_GENERIC;
    }
    return INSTALL_OK;
}

int GsiService::ReenableGsi(bool one_shot) {
    if (!android::gsi::IsGsiInstalled()) {
        LOG(ERROR) << "no gsi installed - cannot re-enable";
        return INSTALL_ERROR_GENERIC;
    }

    std::string boot_key;
    if (!GetInstallStatus(&boot_key)) {
        PLOG(ERROR) << "read " << kGsiInstallStatusFile;
        return INSTALL_ERROR_GENERIC;
    }
    if (boot_key != kInstallStatusDisabled) {
        LOG(ERROR) << "GSI is not currently disabled";
        return INSTALL_ERROR_GENERIC;
    }

    if (IsGsiRunning()) {
        if (!SetBootMode(one_shot) || !CreateInstallStatusFile()) {
            return INSTALL_ERROR_GENERIC;
        }
        return INSTALL_OK;
    }
    // Note: this metadata is only used to recover the original partition sizes.
    // We do not trust the extent information, which will get rebuilt later.
    auto old_metadata = ReadFromImageFile(kGsiLpMetadataFile);
    if (!old_metadata) {
        LOG(ERROR) << "GSI install is incomplete";
        return INSTALL_ERROR_GENERIC;
    }

    // Set up enough installer state so that we can use various helper
    // methods.
    //
    // TODO(dvander) Extract all of the installer state into a separate
    // class so this is more manageable.
    install_dir_ = GetInstalledImageDir();
    system_gsi_path_ = GetImagePath(install_dir_, "system_gsi");
    if (int error = DetermineReadWriteMethod()) {
        return error;
    }

    // Recover parition information.
    Image userdata_image;
    if (int error = GetExistingImage(*old_metadata.get(), "userdata_gsi", &userdata_image)) {
        return error;
    }
    partitions_.emplace(std::make_pair("userdata_gsi", std::move(userdata_image)));

    Image system_image;
    if (int error = GetExistingImage(*old_metadata.get(), "system_gsi", &system_image)) {
        return error;
    }
    partitions_.emplace(std::make_pair("system_gsi", std::move(system_image)));

    metadata_ = CreateMetadata();
    if (!metadata_) {
        return INSTALL_ERROR_GENERIC;
    }
    if (!CreateMetadataFile() || !SetBootMode(one_shot) || !CreateInstallStatusFile()) {
        return INSTALL_ERROR_GENERIC;
    }
    return INSTALL_OK;
}

int GsiService::WipeUserdata() {
    // Note: this metadata is only used to recover the original partition sizes.
    // We do not trust the extent information, which will get rebuilt later.
    auto old_metadata = ReadFromImageFile(kGsiLpMetadataFile);
    if (!old_metadata) {
        LOG(ERROR) << "GSI install is incomplete";
        return INSTALL_ERROR_GENERIC;
    }

    install_dir_ = GetInstalledImageDir();
    system_gsi_path_ = GetImagePath(install_dir_, "system_gsi");
    if (int error = DetermineReadWriteMethod()) {
        return error;
    }

    // Recover parition information.
    Image userdata_image;
    if (int error = GetExistingImage(*old_metadata.get(), "userdata_gsi", &userdata_image)) {
        return error;
    }
    partitions_.emplace(std::make_pair("userdata_gsi", std::move(userdata_image)));

    metadata_ = CreateMetadata();
    if (!metadata_) {
        return INSTALL_ERROR_GENERIC;
    }

    auto writer = OpenPartition("userdata_gsi");
    if (!writer) {
        return IGsiService::INSTALL_ERROR_GENERIC;
    }

    // Wipe the first 1MiB of the device, ensuring both the first block and
    // the superblock are destroyed.
    static constexpr uint64_t kEraseSize = 1024 * 1024;

    std::string zeroes(4096, 0);
    uint64_t erase_size = std::min(kEraseSize, writer->Size());
    for (uint64_t i = 0; i < erase_size; i += zeroes.size()) {
        if (!writer->Write(zeroes.data(), zeroes.size())) {
            PLOG(ERROR) << "write userdata_gsi";
            return IGsiService::INSTALL_ERROR_GENERIC;
        }
    }
    return INSTALL_OK;
}

static uint64_t GetPartitionSize(const LpMetadata& metadata, const LpMetadataPartition& partition) {
    uint64_t total = 0;
    for (size_t i = 0; i < partition.num_extents; i++) {
        const auto& extent = metadata.extents[partition.first_extent_index + i];
        if (extent.target_type != LP_TARGET_TYPE_LINEAR) {
            LOG(ERROR) << "non-linear extent detected";
            return 0;
        }
        total += extent.num_sectors * LP_SECTOR_SIZE;
    }
    return total;
}

static uint64_t GetPartitionSize(const LpMetadata& metadata, const std::string& name) {
    for (const auto& partition : metadata.partitions) {
        if (GetPartitionName(partition) == name) {
            return GetPartitionSize(metadata, partition);
        }
    }
    return 0;
}

int GsiService::GetExistingImage(const LpMetadata& metadata, const std::string& name,
                                 Image* image) {
    int error;
    std::string path = GetInstalledImagePath(name);
    auto writer = CreateFiemapWriter(path.c_str(), 0, &error);
    if (!writer) {
        return error;
    }

    // Even after recovering the FIEMAP, we also need to know the exact intended
    // size of the image, since FiemapWriter may have extended the final block.
    uint64_t actual_size = GetPartitionSize(metadata, name);
    if (!actual_size) {
        LOG(ERROR) << "Could not determine the pre-existing size of " << name;
        return INSTALL_ERROR_GENERIC;
    }
    image->writer = std::move(writer);
    image->actual_size = actual_size;
    return INSTALL_OK;
}

bool GsiService::RemoveGsiFiles(const std::string& install_dir, bool wipeUserdata) {
    bool ok = true;
    std::string message;
    if (!SplitFiemap::RemoveSplitFiles(GetImagePath(install_dir, "system_gsi"), &message)) {
        LOG(ERROR) << message;
        ok = false;
    }
    if (wipeUserdata &&
        !SplitFiemap::RemoveSplitFiles(GetImagePath(install_dir, "userdata_gsi"), &message)) {
        LOG(ERROR) << message;
        ok = false;
    }

    std::vector<std::string> files{
            kGsiInstallStatusFile,
            kGsiLpMetadataFile,
            kGsiOneShotBootFile,
            kGsiInstallDirFile,
    };
    for (const auto& file : files) {
        if (!android::base::RemoveFileIfExists(file, &message)) {
            LOG(ERROR) << message;
            ok = false;
        }
    }
    return ok;
}

bool GsiService::DisableGsiInstall() {
    if (!android::gsi::IsGsiInstalled()) {
        LOG(ERROR) << "cannot disable gsi install - no install detected";
        return false;
    }
    if (installing_) {
        LOG(ERROR) << "cannot disable gsi during GSI installation";
        return false;
    }
    if (!DisableGsi()) {
        PLOG(ERROR) << "could not write gsi status";
        return false;
    }
    return true;
}

std::unique_ptr<LpMetadata> GsiService::CreateMetadata() {
    std::string data_device_path;
    if (install_dir_ == kDefaultGsiImageFolder && !access(kUserdataDevice, F_OK)) {
        data_device_path = kUserdataDevice;
    } else {
        auto writer = partitions_["system_gsi"].writer.get();
        data_device_path = writer->bdev_path();
    }
    auto data_device_name = android::base::Basename(data_device_path);

    PartitionOpener opener;
    BlockDeviceInfo data_device_info;
    if (!opener.GetInfo(data_device_path, &data_device_info)) {
        LOG(ERROR) << "Error reading userdata partition";
        return nullptr;
    }

    std::vector<BlockDeviceInfo> block_devices = {data_device_info};
    auto builder = MetadataBuilder::New(block_devices, data_device_name, 128 * 1024, 1);
    if (!builder) {
        LOG(ERROR) << "Error creating metadata builder";
        return nullptr;
    }
    builder->IgnoreSlotSuffixing();

    for (const auto& [name, image] : partitions_) {
        uint32_t flags = LP_PARTITION_ATTR_NONE;
        if (name == "system_gsi") {
            flags |= LP_PARTITION_ATTR_READONLY;
        }
        Partition* partition = builder->AddPartition(name, flags);
        if (!partition) {
            LOG(ERROR) << "Error adding " << name << " to partition table";
            return nullptr;
        }
        if (!AddPartitionFiemap(builder.get(), partition, image, data_device_name)) {
            return nullptr;
        }
    }

    auto metadata = builder->Export();
    if (!metadata) {
        LOG(ERROR) << "Error exporting partition table";
        return nullptr;
    }
    return metadata;
}

bool GsiService::CreateMetadataFile() {
    if (!WriteToImageFile(kGsiLpMetadataFile, *metadata_.get())) {
        LOG(ERROR) << "Error writing GSI partition table image";
        return false;
    }
    return true;
}

bool GsiService::FormatUserdata() {
    auto writer = OpenPartition("userdata_gsi");
    if (!writer) {
        return false;
    }

    // libcutils checks the first 4K, no matter the block size.
    std::string zeroes(4096, 0);
    if (!writer->Write(zeroes.data(), zeroes.size())) {
        PLOG(ERROR) << "write userdata_gsi";
        return false;
    }
    return true;
}

bool GsiService::AddPartitionFiemap(MetadataBuilder* builder, Partition* partition,
                                    const Image& image, const std::string& block_device) {
    uint64_t sectors_needed = image.actual_size / LP_SECTOR_SIZE;
    for (const auto& extent : image.writer->extents()) {
        // :TODO: block size check for length, not sector size
        if (extent.fe_length % LP_SECTOR_SIZE != 0) {
            LOG(ERROR) << "Extent is not sector-aligned: " << extent.fe_length;
            return false;
        }
        if (extent.fe_physical % LP_SECTOR_SIZE != 0) {
            LOG(ERROR) << "Extent physical sector is not sector-aligned: " << extent.fe_physical;
            return false;
        }

        uint64_t num_sectors =
                std::min(static_cast<uint64_t>(extent.fe_length / LP_SECTOR_SIZE), sectors_needed);
        if (!num_sectors || !sectors_needed) {
            // This should never happen, but we include it just in case. It would
            // indicate that the last filesystem block had multiple extents.
            LOG(WARNING) << "FiemapWriter allocated extra blocks";
            break;
        }

        uint64_t physical_sector = extent.fe_physical / LP_SECTOR_SIZE;
        if (!builder->AddLinearExtent(partition, block_device, num_sectors, physical_sector)) {
            LOG(ERROR) << "Could not add extent to lp metadata";
            return false;
        }

        sectors_needed -= num_sectors;
    }
    return true;
}

bool GsiService::SetBootMode(bool one_shot) {
    if (one_shot) {
        if (!android::base::WriteStringToFile("1", kGsiOneShotBootFile)) {
            PLOG(ERROR) << "write " << kGsiOneShotBootFile;
            return false;
        }
    } else if (!access(kGsiOneShotBootFile, F_OK)) {
        std::string error;
        if (!android::base::RemoveFileIfExists(kGsiOneShotBootFile, &error)) {
            LOG(ERROR) << error;
            return false;
        }
    }
    return true;
}

bool GsiService::CreateInstallStatusFile() {
    if (!android::base::WriteStringToFile("0", kGsiInstallStatusFile)) {
        PLOG(ERROR) << "write " << kGsiInstallStatusFile;
        return false;
    }
    return true;
}

void GsiService::RunStartupTasks() {
    if (!IsGsiInstalled()) {
        return;
    }

    std::string boot_key;
    if (!GetInstallStatus(&boot_key)) {
        PLOG(ERROR) << "read " << kGsiInstallStatusFile;
        return;
    }

    if (!IsGsiRunning()) {
        // Check if a wipe was requested from fastboot or adb-in-gsi.
        if (boot_key == kInstallStatusWipe) {
            RemoveGsiFiles(GetInstalledImageDir(), true /* wipeUserdata */);
        }
    } else {
        // NB: When single-boot is enabled, init will write "disabled" into the
        // install_status file, which will cause GetBootAttempts to return
        // false. Thus, we won't write "ok" here.
        int ignore;
        if (GetBootAttempts(boot_key, &ignore)) {
            // Mark the GSI as having successfully booted.
            if (!android::base::WriteStringToFile(kInstallStatusOk, kGsiInstallStatusFile)) {
                PLOG(ERROR) << "write " << kGsiInstallStatusFile;
            }
        }
    }
}

}  // namespace gsi
}  // namespace android
