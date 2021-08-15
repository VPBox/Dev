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

#define LOG_TAG "apexd"

#include "apex_database.h"
#include "apex_constants.h"
#include "apex_file.h"
#include "apexd_utils.h"
#include "status_or.h"
#include "string_log.h"

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/parseint.h>
#include <android-base/strings.h>

#include <filesystem>
#include <fstream>
#include <string>
#include <unordered_map>
#include <utility>

using android::base::EndsWith;
using android::base::ParseInt;
using android::base::ReadFileToString;
using android::base::Split;
using android::base::StartsWith;
using android::base::Trim;

namespace fs = std::filesystem;

namespace android {
namespace apex {

namespace {

using MountedApexData = MountedApexDatabase::MountedApexData;

// from art/runtime/class_linker.cc
inline size_t hash_combine(size_t seed, size_t val) {
  return seed ^ (val + 0x9e3779b9 + (seed << 6) + (seed >> 2));
}

typedef std::pair<dev_t, ino_t> inode_t;
struct inode_hash {
  size_t operator()(const inode_t& inode) const {
    auto h1 = std::hash<dev_t>{}(inode.first);
    auto h2 = std::hash<ino_t>{}(inode.second);
    return hash_combine(h1, h2);
  }
};
typedef std::unordered_map<inode_t, std::string, inode_hash> inode_map;

enum BlockDeviceType {
  UnknownDevice,
  LoopDevice,
  DeviceMapperDevice,
};

const fs::path kDevBlock = "/dev/block";
const fs::path kSysBlock = "/sys/block";

class BlockDevice {
  std::string name;  // loopN, dm-N, ...
 public:
  explicit BlockDevice(const fs::path& path) { name = path.filename(); }

  BlockDeviceType GetType() const {
    if (StartsWith(name, "loop")) return LoopDevice;
    if (StartsWith(name, "dm-")) return DeviceMapperDevice;
    return UnknownDevice;
  }

  fs::path SysPath() const { return kSysBlock / name; }

  fs::path DevPath() const { return kDevBlock / name; }

  StatusOr<std::string> GetProperty(const std::string& property) const {
    auto propertyFile = SysPath() / property;
    std::string propertyValue;
    if (!ReadFileToString(propertyFile, &propertyValue)) {
      return StatusOr<std::string>::MakeError(PStringLog() << "Fail to read");
    }
    return StatusOr<std::string>(Trim(propertyValue));
  }

  std::vector<BlockDevice> GetSlaves() const {
    std::vector<BlockDevice> slaves;
    std::error_code ec;
    auto status = WalkDir(SysPath() / "slaves", [&](const auto& entry) {
      BlockDevice dev(entry);
      if (fs::is_block_file(dev.DevPath(), ec)) {
        slaves.push_back(dev);
      }
    });
    if (!status.Ok()) {
      LOG(WARNING) << status.ErrorMessage();
    }
    return slaves;
  }
};

std::pair<fs::path, fs::path> parseMountInfo(const std::string& mountInfo) {
  const auto& tokens = Split(mountInfo, " ");
  if (tokens.size() < 2) {
    return std::make_pair("", "");
  }
  return std::make_pair(tokens[0], tokens[1]);
}

std::pair<std::string, int> parseMountPoint(const std::string& mountPoint) {
  auto packageId = fs::path(mountPoint).filename();
  auto split = Split(packageId, "@");
  if (split.size() == 2) {
    int version;
    if (!ParseInt(split[1], &version)) {
      version = -1;
    }
    return std::make_pair(split[0], version);
  }
  return std::make_pair(packageId, -1);
}

bool isActiveMountPoint(const std::string& mountPoint) {
  return (mountPoint.find('@') == std::string::npos);
}

StatusOr<inode_t> inodeFor(const std::string& path) {
  struct stat buf;
  if (stat(path.c_str(), &buf)) {
    return StatusOr<inode_t>::MakeError(PStringLog() << "stat failed");
  }
  return StatusOr<inode_t>(buf.st_dev, buf.st_ino);
}

// Flattened packages from builtin APEX dirs(/system/apex, /product/apex, ...)
inode_map scanFlattendedPackages() {
  inode_map map;

  for (const auto& dir : kApexPackageBuiltinDirs) {
    auto status = WalkDir(dir, [&](const fs::directory_entry& entry) {
      const auto& path = entry.path();
      if (isFlattenedApex(path)) {
        auto inode = inodeFor(path);
        if (inode.Ok()) {
          map[*inode] = path;
        }
      }
    });
    if (!status.Ok()) {
      LOG(ERROR) << "Failed to walk " << dir << " : " << status.ErrorMessage();
    }
  }

  return map;
}

StatusOr<MountedApexData> resolveMountInfo(const BlockDevice& block,
                                           const std::string& mountPoint,
                                           const inode_map& inodeMap) {
  auto Error = [](auto e) { return StatusOr<MountedApexData>::MakeError(e); };

  // First, see if it is bind-mount'ed to a flattened APEX
  // This is checked first since flattened APEXes can be located in any stacked
  // filesystem. (e.g. if / is mounted via /dev/loop1, then /proc/mounts shows
  // that loop device as associated block device. But it is not related to APEX
  // activation.) In any cases, comparing (dev,inode) pair with scanned
  // flattened APEXes must identify bind-mounted APEX properly.
  // See b/131924899.
  auto inode = inodeFor(mountPoint);
  if (inode.Ok()) {
    auto iter = inodeMap.find(*inode);
    if (iter != inodeMap.end()) {
      return StatusOr<MountedApexData>("", iter->second, mountPoint, "");
    }
  }

  // Now, see if it is dm-verity or loop mounted
  switch (block.GetType()) {
    case LoopDevice: {
      auto backingFile = block.GetProperty("loop/backing_file");
      if (!backingFile.Ok()) {
        return Error(backingFile.ErrorStatus());
      }
      return StatusOr<MountedApexData>(block.DevPath(), *backingFile,
                                       mountPoint, "");
    }
    case DeviceMapperDevice: {
      auto name = block.GetProperty("dm/name");
      if (!name.Ok()) {
        return Error(name.ErrorStatus());
      }
      auto slaves = block.GetSlaves();
      if (slaves.empty() || slaves[0].GetType() != LoopDevice) {
        return Error("DeviceMapper device with no loop devices");
      }
      // TODO(jooyung): handle multiple loop devices when hash tree is
      // externalized
      auto slave = slaves[0];
      auto backingFile = slave.GetProperty("loop/backing_file");
      if (!backingFile.Ok()) {
        return Error(backingFile.ErrorStatus());
      }
      return StatusOr<MountedApexData>(slave.DevPath(), *backingFile,
                                       mountPoint, *name);
    }
    case UnknownDevice: {
      return Error("Can't resolve " + block.DevPath().string());
    }
  }
}

}  // namespace

// On startup, APEX database is populated from /proc/mounts.

// /apex/<package-id> can be mounted from
// - /dev/block/loopX : loop device
// - /dev/block/dm-X : dm-verity
// - <flattened> : bind-mount

// In case of loop device, it is from a non-flattened
// APEX file. This original APEX file can be tracked
// by /sys/block/loopX/loop/backing_file.

// In case of dm-verity, it is mapped to a loop device.
// This mapped loop device can be traced by
// /sys/block/dm-X/slaves/ directory which contains
// a symlink to /sys/block/loopY, which leads to
// the original APEX file.
// Device name can be retrieved from
// /sys/block/dm-Y/dm/name.

// In case of <flattened>, it is --bind mounted to a flattened
// APEX directory. This is allowed only for system/product
// partitions. So, original APEX directory can be found
// by comparing dev/inode pair with candidates.

// By synchronizing the mounts info with Database on startup,
// Apexd serves the correct package list even on the devices
// which are not ro.apex.updatable.
void MountedApexDatabase::PopulateFromMounts() {
  LOG(INFO) << "Populating APEX database from mounts...";

  std::unordered_map<std::string, int> activeVersions;
  inode_map inodeToFlattendApexMap = scanFlattendedPackages();

  std::ifstream mounts("/proc/mounts");
  std::string line;
  while (std::getline(mounts, line)) {
    auto [block, mountPoint] = parseMountInfo(line);
    // TODO(jooyung): ignore tmp mount?
    if (fs::path(mountPoint).parent_path() != kApexRoot) {
      continue;
    }
    if (isActiveMountPoint(mountPoint)) {
      continue;
    }

    auto mountData = resolveMountInfo(BlockDevice(block), mountPoint,
                                      inodeToFlattendApexMap);
    if (!mountData.Ok()) {
      LOG(WARNING) << "Can't resolve mount info " << mountData.ErrorMessage();
      continue;
    }

    auto [package, version] = parseMountPoint(mountPoint);
    AddMountedApex(package, false, *mountData);

    auto active = activeVersions[package] < version;
    if (active) {
      activeVersions[package] = version;
      SetLatest(package, mountData->full_path);
    }
    LOG(INFO) << "Found " << mountPoint;
  }

  LOG(INFO) << mounted_apexes_.size() << " packages restored.";
}

}  // namespace apex
}  // namespace android
