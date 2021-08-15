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

#include "apexd_loop.h"

#include <dirent.h>
#include <fcntl.h>
#include <linux/fs.h>
#include <linux/loop.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/stringprintf.h>
#include <android-base/strings.h>

#include "apexd_utils.h"
#include "string_log.h"

using android::base::StartsWith;
using android::base::StringPrintf;
using android::base::unique_fd;

namespace android {
namespace apex {
namespace loop {

static constexpr const char* kApexLoopIdPrefix = "apex:";

// 128 kB read-ahead, which we currently use for /system as well
static constexpr const char* kReadAheadKb = "128";

// TODO(b/122059364): Even though the kernel has created the loop
// device, we still depend on ueventd to run to actually create the
// device node in userspace. To solve this properly we should listen on
// the netlink socket for uevents, or use inotify. For now, this will
// have to do.
static constexpr size_t kLoopDeviceRetryAttempts = 3u;

void LoopbackDeviceUniqueFd::MaybeCloseBad() {
  if (device_fd.get() != -1) {
    // Disassociate any files.
    if (ioctl(device_fd.get(), LOOP_CLR_FD) == -1) {
      PLOG(ERROR) << "Unable to clear fd for loopback device";
    }
  }
}

Status configureReadAhead(const std::string& device_path) {
  auto pos = device_path.find("/dev/block/");
  if (pos != 0) {
    return Status::Fail(StringLog()
                        << "Device path does not start with /dev/block.");
  }
  pos = device_path.find_last_of('/');
  std::string device_name = device_path.substr(pos + 1, std::string::npos);

  std::string sysfs_device =
      StringPrintf("/sys/block/%s/queue/read_ahead_kb", device_name.c_str());
  unique_fd sysfs_fd(open(sysfs_device.c_str(), O_RDWR | O_CLOEXEC));
  if (sysfs_fd.get() == -1) {
    return Status::Fail(PStringLog() << "Failed to open " << sysfs_device);
  }

  int ret = TEMP_FAILURE_RETRY(
      write(sysfs_fd.get(), kReadAheadKb, strlen(kReadAheadKb) + 1));
  if (ret < 0) {
    return Status::Fail(PStringLog() << "Failed to write to " << sysfs_device);
  }

  return Status::Success();
}

Status preAllocateLoopDevices(size_t num) {
  Status loopReady = WaitForFile("/dev/loop-control", 20s);
  if (!loopReady.Ok()) {
    return loopReady;
  }
  unique_fd ctl_fd(
      TEMP_FAILURE_RETRY(open("/dev/loop-control", O_RDWR | O_CLOEXEC)));
  if (ctl_fd.get() == -1) {
    return Status::Fail(PStringLog() << "Failed to open loop-control");
  }

  // Assumption: loop device ID [0..num) is valid.
  // This is because pre-allocation happens during bootstrap.
  // Anyway Kernel pre-allocated loop devices
  // as many as CONFIG_BLK_DEV_LOOP_MIN_COUNT,
  // Within the amount of kernel-pre-allocation,
  // LOOP_CTL_ADD will fail with EEXIST
  for (size_t id = 0ul; id < num; ++id) {
    int ret = ioctl(ctl_fd.get(), LOOP_CTL_ADD, id);
    if (ret < 0 && errno != EEXIST) {
      return Status::Fail(PStringLog() << "Failed LOOP_CTL_ADD");
    }
  }

  // Don't wait until the dev nodes are actually created, which
  // will delay the boot. By simply returing here, the creation of the dev
  // nodes will be done in parallel with other boot processes, and we
  // just optimistally hope that they are all created when we actually
  // access them for activating APEXes. If the dev nodes are not ready
  // even then, we wait 50ms and warning message will be printed (see below
  // createLoopDevice()).
  LOG(INFO) << "Pre-allocated " << num << " loopback devices";
  return Status::Success();
}

StatusOr<LoopbackDeviceUniqueFd> createLoopDevice(const std::string& target,
                                                  const int32_t imageOffset,
                                                  const size_t imageSize) {
  using Failed = StatusOr<LoopbackDeviceUniqueFd>;
  unique_fd ctl_fd(open("/dev/loop-control", O_RDWR | O_CLOEXEC));
  if (ctl_fd.get() == -1) {
    return Failed::MakeError(PStringLog() << "Failed to open loop-control");
  }

  int num = ioctl(ctl_fd.get(), LOOP_CTL_GET_FREE);
  if (num == -1) {
    return Failed::MakeError(PStringLog() << "Failed LOOP_CTL_GET_FREE");
  }

  std::string device = StringPrintf("/dev/block/loop%d", num);

  unique_fd target_fd(open(target.c_str(), O_RDONLY | O_CLOEXEC));
  if (target_fd.get() == -1) {
    return Failed::MakeError(PStringLog() << "Failed to open " << target);
  }
  LoopbackDeviceUniqueFd device_fd;
  {
    // See comment on kLoopDeviceRetryAttempts.
    unique_fd sysfs_fd;
    for (size_t i = 0; i != kLoopDeviceRetryAttempts; ++i) {
      sysfs_fd.reset(open(device.c_str(), O_RDWR | O_CLOEXEC));
      if (sysfs_fd.get() != -1) {
        break;
      }
      PLOG(WARNING) << "Loopback device " << device
                    << " not ready. Waiting 50ms...";
      usleep(50000);
    }
    if (sysfs_fd.get() == -1) {
      return Failed::MakeError(PStringLog() << "Failed to open " << device);
    }
    device_fd = LoopbackDeviceUniqueFd(std::move(sysfs_fd), device);
    CHECK_NE(device_fd.get(), -1);
  }

  if (ioctl(device_fd.get(), LOOP_SET_FD, target_fd.get()) == -1) {
    return Failed::MakeError(PStringLog() << "Failed to LOOP_SET_FD");
  }

  struct loop_info64 li;
  memset(&li, 0, sizeof(li));
  strlcpy((char*)li.lo_crypt_name, kApexLoopIdPrefix, LO_NAME_SIZE);
  li.lo_offset = imageOffset;
  li.lo_sizelimit = imageSize;
  if (ioctl(device_fd.get(), LOOP_SET_STATUS64, &li) == -1) {
    return Failed::MakeError(PStringLog() << "Failed to LOOP_SET_STATUS64");
  }

  if (ioctl(device_fd.get(), BLKFLSBUF, 0) == -1) {
    // This works around a kernel bug where the following happens.
    // 1) The device runs with a value of loop.max_part > 0
    // 2) As part of LOOP_SET_FD above, we do a partition scan, which loads
    //    the first 2 pages of the underlying file into the buffer cache
    // 3) When we then change the offset with LOOP_SET_STATUS64, those pages
    //    are not invalidated from the cache.
    // 4) When we try to mount an ext4 filesystem on the loop device, the ext4
    //    code will try to find a superblock by reading 4k at offset 0; but,
    //    because we still have the old pages at offset 0 lying in the cache,
    //    those pages will be returned directly. However, those pages contain
    //    the data at offset 0 in the underlying file, not at the offset that
    //    we configured
    // 5) the ext4 driver fails to find a superblock in the (wrong) data, and
    //    fails to mount the filesystem.
    //
    // To work around this, explicitly flush the block device, which will flush
    // the buffer cache and make sure we actually read the data at the correct
    // offset.
    return Failed::MakeError(PStringLog()
                             << "Failed to flush buffers on the loop device");
  }

  // Direct-IO requires the loop device to have the same block size as the
  // underlying filesystem.
  if (ioctl(device_fd.get(), LOOP_SET_BLOCK_SIZE, 4096) == -1) {
    PLOG(WARNING) << "Failed to LOOP_SET_BLOCK_SIZE";
  } else {
    if (ioctl(device_fd.get(), LOOP_SET_DIRECT_IO, 1) == -1) {
      PLOG(WARNING) << "Failed to LOOP_SET_DIRECT_IO";
      // TODO Eventually we'll want to fail on this; right now we can't because
      // not all devices have the necessary kernel patches.
    }
  }

  Status readAheadStatus = configureReadAhead(device);
  if (!readAheadStatus.Ok()) {
    return Failed::MakeError(StringLog() << readAheadStatus.ErrorMessage());
  }
  return StatusOr<LoopbackDeviceUniqueFd>(std::move(device_fd));
}

void DestroyLoopDevice(const std::string& path, const DestroyLoopFn& extra) {
  unique_fd fd(open(path.c_str(), O_RDWR | O_CLOEXEC));
  if (fd.get() == -1) {
    if (errno != ENOENT) {
      PLOG(WARNING) << "Failed to open " << path;
    }
    return;
  }

  struct loop_info64 li;
  if (ioctl(fd.get(), LOOP_GET_STATUS64, &li) < 0) {
    if (errno != ENXIO) {
      PLOG(WARNING) << "Failed to LOOP_GET_STATUS64 " << path;
    }
    return;
  }

  auto id = std::string((char*)li.lo_crypt_name);
  if (StartsWith(id, kApexLoopIdPrefix)) {
    extra(path, id);

    if (ioctl(fd.get(), LOOP_CLR_FD, 0) < 0) {
      PLOG(WARNING) << "Failed to LOOP_CLR_FD " << path;
    }
  }
}

}  // namespace loop
}  // namespace apex
}  // namespace android
