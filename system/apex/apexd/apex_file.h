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

#ifndef ANDROID_APEXD_APEX_FILE_H_
#define ANDROID_APEXD_APEX_FILE_H_

#include <memory>
#include <string>
#include <vector>
#include <ziparchive/zip_archive.h>

#include "apex_constants.h"
#include "apex_manifest.h"
#include "status_or.h"

struct AvbHashtreeDescriptor;

namespace android {
namespace apex {

// Data needed to construct a valid VerityTable
struct ApexVerityData {
  std::unique_ptr<AvbHashtreeDescriptor> desc;
  std::string salt;
  std::string root_digest;
};

// Manages the content of an APEX package and provides utilities to navigate
// the content.
class ApexFile {
 public:
  static StatusOr<ApexFile> Open(const std::string& path);
  ApexFile() = delete;
  ApexFile(ApexFile&&) = default;

  const std::string& GetPath() const { return apex_path_; }
  int32_t GetImageOffset() const { return image_offset_; }
  size_t GetImageSize() const { return image_size_; }
  const ApexManifest& GetManifest() const { return manifest_; }
  bool IsFlattened() const { return flattened_; }
  const std::string& GetBundledPublicKey() const { return apex_pubkey_; }

  StatusOr<ApexVerityData> VerifyApexVerity() const;
  Status VerifyManifestMatches(const std::string& mount_path) const;

 private:
  ApexFile(const std::string& apex_path, bool flattened, int32_t image_offset,
           size_t image_size, ApexManifest& manifest,
           const std::string& apex_pubkey)
      : apex_path_(apex_path),
        flattened_(flattened),
        image_offset_(image_offset),
        image_size_(image_size),
        manifest_(std::move(manifest)),
        apex_pubkey_(apex_pubkey) {}

  std::string apex_path_;
  bool flattened_;
  int32_t image_offset_;
  size_t image_size_;
  ApexManifest manifest_;
  std::string apex_pubkey_;
};

StatusOr<std::vector<std::string>> FindApexes(
    const std::vector<std::string>& paths);
StatusOr<std::vector<std::string>> FindApexFilesByName(const std::string& path,
                                                       bool include_dirs);

bool isPathForBuiltinApexes(const std::string& path);
bool isFlattenedApex(const std::string& path);

}  // namespace apex
}  // namespace android

#endif  // ANDROID_APEXD_APEX_FILE_H_
