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

#include "apex_shim.h"

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/stringprintf.h>
#include <android-base/strings.h>
#include <openssl/sha.h>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <unordered_set>

#include "apex_file.h"
#include "status.h"
#include "status_or.h"
#include "string_log.h"

namespace android {
namespace apex {
namespace shim {

namespace fs = std::filesystem;

namespace {

static constexpr const char* kApexCtsShimPackage = "com.android.apex.cts.shim";
static constexpr const char* kHashFileName = "hash.txt";
static constexpr const int kBufSize = 1024;
static constexpr const char* kApexManifestFileName = "apex_manifest.json";
static constexpr const char* kEtcFolderName = "etc";
static constexpr const char* kLostFoundFolderName = "lost+found";
static constexpr const fs::perms kFordbiddenFilePermissions =
    fs::perms::owner_exec | fs::perms::group_exec | fs::perms::others_exec;

StatusOr<std::string> CalculateSha512(const std::string& path) {
  using StatusT = StatusOr<std::string>;
  LOG(DEBUG) << "Calculating SHA512 of " << path;
  SHA512_CTX ctx;
  SHA512_Init(&ctx);
  std::ifstream apex(path, std::ios::binary);
  if (apex.bad()) {
    return StatusT::MakeError(StringLog() << "Failed to open " << path);
  }
  char buf[kBufSize];
  while (!apex.eof()) {
    apex.read(buf, kBufSize);
    if (apex.bad()) {
      return StatusT::MakeError(StringLog() << "Failed to read " << path);
    }
    int bytes_read = apex.gcount();
    SHA512_Update(&ctx, buf, bytes_read);
  }
  uint8_t hash[SHA512_DIGEST_LENGTH];
  SHA512_Final(hash, &ctx);
  std::stringstream ss;
  ss << std::hex;
  for (int i = 0; i < SHA512_DIGEST_LENGTH; i++) {
    ss << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
  }
  return StatusT(ss.str());
}

StatusOr<std::vector<std::string>> ReadSha512(const std::string& path) {
  using android::base::ReadFileToString;
  using android::base::StringPrintf;
  using StatusT = StatusOr<std::vector<std::string>>;
  const std::string& file_path =
      StringPrintf("%s/%s/%s", path.c_str(), kEtcFolderName, kHashFileName);
  LOG(DEBUG) << "Reading SHA512 from " << file_path;
  std::string hash;
  if (!ReadFileToString(file_path, &hash, false /* follows symlinks */)) {
    return StatusT::MakeError(PStringLog() << "Failed to read " << file_path);
  }
  return StatusT(android::base::Split(hash, "\n"));
}

Status IsRegularFile(const fs::directory_entry& entry) {
  const fs::path& path = entry.path();
  std::error_code ec;
  fs::file_status status = entry.status(ec);
  if (ec) {
    return Status::Fail(StringLog()
                        << "Failed to stat " << path << " : " << ec);
  }
  if (!fs::is_regular_file(status)) {
    return Status::Fail(StringLog() << path << " is not a file");
  }
  if ((status.permissions() & kFordbiddenFilePermissions) != fs::perms::none) {
    return Status::Fail(StringLog() << path << " has illegal permissions");
  }
  // TODO: consider checking that file only contains ascii characters.
  return Status::Success();
}

Status IsHashTxt(const fs::directory_entry& entry) {
  LOG(DEBUG) << "Checking if " << entry.path() << " is an allowed file";
  const Status& status = IsRegularFile(entry);
  if (!status.Ok()) {
    return status;
  }
  if (entry.path().filename() != kHashFileName) {
    return Status::Fail(StringLog() << "Illegal file " << entry.path());
  }
  return Status::Success();
}

Status IsWhitelistedTopLevelEntry(const fs::directory_entry& entry) {
  LOG(DEBUG) << "Checking if " << entry.path() << " is an allowed directory";
  std::error_code ec;
  const fs::path& path = entry.path();
  if (path.filename() == kLostFoundFolderName) {
    bool is_empty = fs::is_empty(path, ec);
    if (ec) {
      return Status::Fail(StringLog()
                          << "Failed to scan " << path << " : " << ec);
    }
    if (is_empty) {
      return Status::Success();
    } else {
      return Status::Fail(StringLog() << path << " is not empty");
    }
  } else if (path.filename() == kEtcFolderName) {
    auto iter = fs::directory_iterator(path, ec);
    if (ec) {
      return Status::Fail(StringLog()
                          << "Failed to scan " << path << " : " << ec);
    }
    bool is_empty = fs::is_empty(path, ec);
    if (ec) {
      return Status::Fail(StringLog()
                          << "Failed to scan " << path << " : " << ec);
    }
    if (is_empty) {
      return Status::Fail(StringLog()
                          << path << " should contain " << kHashFileName);
    }
    // TODO: change to non-throwing iterator.
    while (iter != fs::end(iter)) {
      const Status& status = IsHashTxt(*iter);
      if (!status.Ok()) {
        return status;
      }
      iter = iter.increment(ec);
      if (ec) {
        return Status::Fail(StringLog()
                            << "Failed to scan " << path << " : " << ec);
      }
    }
    return Status::Success();
  } else if (path.filename() == kApexManifestFileName) {
    return IsRegularFile(entry);
  } else {
    return Status::Fail(StringLog() << "Illegal entry " << path);
  }
}

}  // namespace

bool IsShimApex(const ApexFile& apex_file) {
  return apex_file.GetManifest().name() == kApexCtsShimPackage;
}

Status ValidateShimApex(const std::string& mount_point,
                        const ApexFile& apex_file) {
  LOG(DEBUG) << "Validating shim apex " << mount_point;
  const ApexManifest& manifest = apex_file.GetManifest();
  if (!manifest.preinstallhook().empty() ||
      !manifest.postinstallhook().empty()) {
    return Status::Fail(
        "Shim apex is not allowed to have pre or post install hooks");
  }
  std::error_code ec;
  auto iter = fs::directory_iterator(mount_point, ec);
  if (ec) {
    return Status::Fail(StringLog()
                        << "Failed to scan " << mount_point << " : " << ec);
  }
  // Unfortunately fs::directory_iterator::operator++ can throw an exception,
  // which means that it's impossible to use range-based for loop here.
  // TODO: wrap into a non-throwing iterator to support range-based for loop.
  while (iter != fs::end(iter)) {
    const Status& status = IsWhitelistedTopLevelEntry(*iter);
    if (!status.Ok()) {
      return status;
    }
    iter = iter.increment(ec);
    if (ec) {
      return Status::Fail(StringLog()
                          << "Failed to scan " << mount_point << " : " << ec);
    }
  }
  return Status::Success();
}

Status ValidateUpdate(const std::string& system_apex_path,
                      const std::string& new_apex_path) {
  LOG(DEBUG) << "Validating update of shim apex to " << new_apex_path
             << " using system shim apex " << system_apex_path;
  auto allowed = ReadSha512(system_apex_path);
  if (!allowed.Ok()) {
    return allowed.ErrorStatus();
  }
  auto actual = CalculateSha512(new_apex_path);
  if (!actual.Ok()) {
    return actual.ErrorStatus();
  }
  auto it = std::find(allowed->begin(), allowed->end(), *actual);
  if (it == allowed->end()) {
    return Status::Fail(StringLog()
                        << new_apex_path << " has unexpected SHA512 hash "
                        << *actual);
  }
  return Status::Success();
}

}  // namespace shim
}  // namespace apex
}  // namespace android
