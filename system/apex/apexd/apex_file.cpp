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

#include "apex_file.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <filesystem>
#include <fstream>

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/scopeguard.h>
#include <android-base/strings.h>
#include <android-base/unique_fd.h>
#include <google/protobuf/util/message_differencer.h>
#include <libavb/libavb.h>

#include "apex_key.h"
#include "apexd_utils.h"
#include "string_log.h"

using android::base::EndsWith;
using android::base::ReadFullyAtOffset;
using android::base::StartsWith;
using android::base::unique_fd;
using google::protobuf::util::MessageDifferencer;

namespace android {
namespace apex {
namespace {

constexpr const char* kImageFilename = "apex_payload.img";
constexpr const char* kManifestFilename = "apex_manifest.json";
constexpr const char* kBundledPublicKeyFilename = "apex_pubkey";
#ifdef DEBUG_ALLOW_BUNDLED_KEY
constexpr const bool kDebugAllowBundledKey = true;
#else
constexpr const bool kDebugAllowBundledKey = false;
#endif

}  // namespace

// Tests if <path>/manifest.json file exists.
bool isFlattenedApex(const std::string& path) {
  struct stat buf;
  const std::string manifest = path + "/" + kManifestFilename;
  if (stat(manifest.c_str(), &buf) != 0) {
    if (errno == ENOENT) {
      return false;
    }
    // If the APEX is there but not a flatttened apex, the final component
    // of path will be a file, and stat will complain that it's not a directory.
    // We are OK with that to avoid two stat calls.
    if (errno != ENOTDIR) {
      PLOG(ERROR) << "Failed to stat " << path;
    }
    return false;
  }

  if (!S_ISREG(buf.st_mode)) {
    return false;
  }
  return true;
}

StatusOr<ApexFile> ApexFile::Open(const std::string& path) {
  bool flattened;
  int32_t image_offset;
  size_t image_size;
  std::string manifest_content;
  std::string pubkey;

  if (isFlattenedApex(path)) {
    flattened = true;
    image_offset = 0;
    image_size = 0;
    const std::string manifest_path = path + "/" + kManifestFilename;
    if (!android::base::ReadFileToString(manifest_path, &manifest_content)) {
      std::string err = StringLog()
                        << "Failed to read manifest file: " << manifest_path;
      return StatusOr<ApexFile>::MakeError(err);
    }
    // TODO(b/124115379) don't read public key from flattened APEX when
    // we no longer have APEX tests on devices with flattened APEXes.
    const std::string pubkey_path = path + "/" + kBundledPublicKeyFilename;
    if (access(pubkey_path.c_str(), F_OK) == 0) {
      if (!android::base::ReadFileToString(pubkey_path, &pubkey)) {
        std::string err = StringLog()
                          << "Failed to read pubkey file: " << pubkey_path;
        return StatusOr<ApexFile>::MakeError(err);
      }
    }
  } else {
    flattened = false;

    ZipArchiveHandle handle;
    auto handle_guard =
        android::base::make_scope_guard([&handle] { CloseArchive(handle); });
    int ret = OpenArchive(path.c_str(), &handle);
    if (ret < 0) {
      std::string err = StringLog() << "Failed to open package " << path << ": "
                                    << ErrorCodeString(ret);
      return StatusOr<ApexFile>::MakeError(err);
    }

    // Locate the mountable image within the zipfile and store offset and size.
    ZipEntry entry;
    ret = FindEntry(handle, ZipString(kImageFilename), &entry);
    if (ret < 0) {
      std::string err = StringLog() << "Could not find entry \""
                                    << kImageFilename << "\" in package "
                                    << path << ": " << ErrorCodeString(ret);
      return StatusOr<ApexFile>::MakeError(err);
    }
    image_offset = entry.offset;
    image_size = entry.uncompressed_length;

    ret = FindEntry(handle, ZipString(kManifestFilename), &entry);
    if (ret < 0) {
      std::string err = StringLog() << "Could not find entry \""
                                    << kManifestFilename << "\" in package "
                                    << path << ": " << ErrorCodeString(ret);
      return StatusOr<ApexFile>::MakeError(err);
    }

    uint32_t length = entry.uncompressed_length;
    manifest_content.resize(length, '\0');
    ret = ExtractToMemory(handle, &entry,
                          reinterpret_cast<uint8_t*>(&(manifest_content)[0]),
                          length);
    if (ret != 0) {
      std::string err = StringLog()
                        << "Failed to extract manifest from package " << path
                        << ": " << ErrorCodeString(ret);
      return StatusOr<ApexFile>::MakeError(err);
    }

    ret = FindEntry(handle, ZipString(kBundledPublicKeyFilename), &entry);
    if (ret >= 0) {
      LOG(VERBOSE) << "Found bundled key in package " << path;
      length = entry.uncompressed_length;
      pubkey.resize(length, '\0');
      ret = ExtractToMemory(handle, &entry,
                            reinterpret_cast<uint8_t*>(&(pubkey)[0]), length);
      if (ret != 0) {
        std::string err = StringLog()
                          << "Failed to extract public key from package "
                          << path << ": " << ErrorCodeString(ret);
        return StatusOr<ApexFile>::MakeError(err);
      }
    }
  }

  StatusOr<ApexManifest> manifest = ParseManifest(manifest_content);
  if (!manifest.Ok()) {
    return StatusOr<ApexFile>::MakeError(manifest.ErrorMessage());
  }

  ApexFile apexFile(path, flattened, image_offset, image_size, *manifest,
                    pubkey);
  return StatusOr<ApexFile>(std::move(apexFile));
}

// AVB-related code.

namespace {

static constexpr const char* kApexKeyProp = "apex.key";

static constexpr int kVbMetaMaxSize = 64 * 1024;

std::string bytes_to_hex(const uint8_t* bytes, size_t bytes_len) {
  std::ostringstream s;

  s << std::hex << std::setfill('0');
  for (size_t i = 0; i < bytes_len; i++) {
    s << std::setw(2) << static_cast<int>(bytes[i]);
  }
  return s.str();
}

std::string getSalt(const AvbHashtreeDescriptor& desc,
                    const uint8_t* trailingData) {
  const uint8_t* desc_salt = trailingData + desc.partition_name_len;

  return bytes_to_hex(desc_salt, desc.salt_len);
}

std::string getDigest(const AvbHashtreeDescriptor& desc,
                      const uint8_t* trailingData) {
  const uint8_t* desc_digest =
      trailingData + desc.partition_name_len + desc.salt_len;

  return bytes_to_hex(desc_digest, desc.root_digest_len);
}

StatusOr<std::unique_ptr<AvbFooter>> getAvbFooter(const ApexFile& apex,
                                                  const unique_fd& fd) {
  std::array<uint8_t, AVB_FOOTER_SIZE> footer_data;
  auto footer = std::make_unique<AvbFooter>();

  // The AVB footer is located in the last part of the image
  off_t offset = apex.GetImageSize() + apex.GetImageOffset() - AVB_FOOTER_SIZE;
  int ret = lseek(fd, offset, SEEK_SET);
  if (ret == -1) {
    return StatusOr<std::unique_ptr<AvbFooter>>::MakeError(
        PStringLog() << "Couldn't seek to AVB footer");
  }

  ret = read(fd, footer_data.data(), AVB_FOOTER_SIZE);
  if (ret != AVB_FOOTER_SIZE) {
    return StatusOr<std::unique_ptr<AvbFooter>>::MakeError(
        PStringLog() << "Couldn't read AVB footer");
  }

  if (!avb_footer_validate_and_byteswap((const AvbFooter*)footer_data.data(),
                                        footer.get())) {
    return StatusOr<std::unique_ptr<AvbFooter>>::MakeError(
        StringLog() << "AVB footer verification failed.");
  }

  LOG(VERBOSE) << "AVB footer verification successful.";
  return StatusOr<std::unique_ptr<AvbFooter>>(std::move(footer));
}

Status verifyPublicKey(const uint8_t* key, size_t length,
                       std::string public_key_content) {
  if (public_key_content.length() != length ||
      memcmp(&public_key_content[0], key, length) != 0) {
    return Status::Fail("Failed to compare the bundled public key with key");
  }
  return Status::Success();
}

StatusOr<std::string> getPublicKeyName(const ApexFile& apex,
                                       const uint8_t* data, size_t length) {
  size_t keyNameLen;
  const char* keyName = avb_property_lookup(data, length, kApexKeyProp,
                                            strlen(kApexKeyProp), &keyNameLen);
  if (keyName == nullptr || keyNameLen == 0) {
    return StatusOr<std::string>::MakeError(
        StringLog() << "Cannot find prop '" << kApexKeyProp << "' from "
                    << apex.GetPath());
  }

  if (keyName != apex.GetManifest().name()) {
    return StatusOr<std::string>::MakeError(
        StringLog() << "Key mismatch: apex name is '"
                    << apex.GetManifest().name() << "'"
                    << " but key name is '" << keyName << "'");
  }
  return StatusOr<std::string>(keyName);
}

Status verifyVbMetaSignature(const ApexFile& apex, const uint8_t* data,
                             size_t length) {
  const uint8_t* pk;
  size_t pk_len;
  AvbVBMetaVerifyResult res;

  res = avb_vbmeta_image_verify(data, length, &pk, &pk_len);
  switch (res) {
    case AVB_VBMETA_VERIFY_RESULT_OK:
      break;
    case AVB_VBMETA_VERIFY_RESULT_OK_NOT_SIGNED:
    case AVB_VBMETA_VERIFY_RESULT_HASH_MISMATCH:
    case AVB_VBMETA_VERIFY_RESULT_SIGNATURE_MISMATCH:
      return Status::Fail(StringLog()
                          << "Error verifying " << apex.GetPath() << ": "
                          << avb_vbmeta_verify_result_to_string(res));
    case AVB_VBMETA_VERIFY_RESULT_INVALID_VBMETA_HEADER:
      return Status::Fail(StringLog()
                          << "Error verifying " << apex.GetPath() << ": "
                          << "invalid vbmeta header");
    case AVB_VBMETA_VERIFY_RESULT_UNSUPPORTED_VERSION:
      return Status::Fail(StringLog()
                          << "Error verifying " << apex.GetPath() << ": "
                          << "unsupported version");
    default:
      return Status::Fail("Unknown vmbeta_image_verify return value");
  }

  StatusOr<std::string> key_name = getPublicKeyName(apex, data, length);
  if (!key_name.Ok()) {
    return key_name.ErrorStatus();
  }

  StatusOr<const std::string> public_key = getApexKey(*key_name);
  Status st;
  if (public_key.Ok()) {
    // TODO(b/115718846)
    // We need to decide whether we need rollback protection, and whether
    // we can use the rollback protection provided by libavb.
    st = verifyPublicKey(pk, pk_len, *public_key);
  } else if (kDebugAllowBundledKey) {
    // Failing to find the matching public key in the built-in partitions
    // is a hard error for non-debuggable build. For debuggable builds,
    // the public key bundled in the APEX itself is used as a fallback.
    LOG(WARNING) << "Verifying " << apex.GetPath() << " with the bundled key";
    st = verifyPublicKey(pk, pk_len, apex.GetBundledPublicKey());
  } else {
    return public_key.ErrorStatus();
  }

  if (st.Ok()) {
    LOG(VERBOSE) << apex.GetPath() << ": public key matches.";
    return st;
  }

  return Status::Fail(StringLog()
                      << "Error verifying " << apex.GetPath() << ": "
                      << "couldn't verify public key: " << st.ErrorMessage());
}

StatusOr<std::unique_ptr<uint8_t[]>> verifyVbMeta(const ApexFile& apex,
                                                  const unique_fd& fd,
                                                  const AvbFooter& footer) {
  if (footer.vbmeta_size > kVbMetaMaxSize) {
    return StatusOr<std::unique_ptr<uint8_t[]>>::MakeError(
        "VbMeta size in footer exceeds kVbMetaMaxSize.");
  }

  off_t offset = apex.GetImageOffset() + footer.vbmeta_offset;
  std::unique_ptr<uint8_t[]> vbmeta_buf(new uint8_t[footer.vbmeta_size]);

  if (!ReadFullyAtOffset(fd, vbmeta_buf.get(), footer.vbmeta_size, offset)) {
    return StatusOr<std::unique_ptr<uint8_t[]>>::MakeError(
        PStringLog() << "Couldn't read AVB meta-data");
  }

  Status st = verifyVbMetaSignature(apex, vbmeta_buf.get(), footer.vbmeta_size);
  if (!st.Ok()) {
    return StatusOr<std::unique_ptr<uint8_t[]>>::MakeError(st.ErrorMessage());
  }

  return StatusOr<std::unique_ptr<uint8_t[]>>(std::move(vbmeta_buf));
}

StatusOr<const AvbHashtreeDescriptor*> findDescriptor(uint8_t* vbmeta_data,
                                                      size_t vbmeta_size) {
  const AvbDescriptor** descriptors;
  size_t num_descriptors;

  descriptors =
      avb_descriptor_get_all(vbmeta_data, vbmeta_size, &num_descriptors);

  // avb_descriptor_get_all() returns an internally allocated array
  // of pointers and it needs to be avb_free()ed after using it.
  auto guard = android::base::ScopeGuard(std::bind(avb_free, descriptors));

  for (size_t i = 0; i < num_descriptors; i++) {
    AvbDescriptor desc;
    if (!avb_descriptor_validate_and_byteswap(descriptors[i], &desc)) {
      return StatusOr<const AvbHashtreeDescriptor*>::MakeError(
          "Couldn't validate AvbDescriptor.");
    }

    if (desc.tag != AVB_DESCRIPTOR_TAG_HASHTREE) {
      // Ignore other descriptors
      continue;
    }

    return StatusOr<const AvbHashtreeDescriptor*>(
        (const AvbHashtreeDescriptor*)descriptors[i]);
  }

  return StatusOr<const AvbHashtreeDescriptor*>::MakeError(
      "Couldn't find any AVB hashtree descriptors.");
}

StatusOr<std::unique_ptr<AvbHashtreeDescriptor>> verifyDescriptor(
    const AvbHashtreeDescriptor* desc) {
  auto verifiedDesc = std::make_unique<AvbHashtreeDescriptor>();

  if (!avb_hashtree_descriptor_validate_and_byteswap(desc,
                                                     verifiedDesc.get())) {
    return StatusOr<std::unique_ptr<AvbHashtreeDescriptor>>::MakeError(
        "Couldn't validate AvbDescriptor.");
  }

  return StatusOr<std::unique_ptr<AvbHashtreeDescriptor>>(
      std::move(verifiedDesc));
}

}  // namespace

StatusOr<ApexVerityData> ApexFile::VerifyApexVerity() const {
  ApexVerityData verityData;

  unique_fd fd(open(GetPath().c_str(), O_RDONLY | O_CLOEXEC));
  if (fd.get() == -1) {
    return StatusOr<ApexVerityData>::MakeError(PStringLog() << "Failed to open "
                                                            << GetPath());
  }

  StatusOr<std::unique_ptr<AvbFooter>> footer = getAvbFooter(*this, fd);
  if (!footer.Ok()) {
    return StatusOr<ApexVerityData>::MakeError(footer.ErrorMessage());
  }

  StatusOr<std::unique_ptr<uint8_t[]>> vbmeta_data =
      verifyVbMeta(*this, fd, **footer);
  if (!vbmeta_data.Ok()) {
    return StatusOr<ApexVerityData>::MakeError(vbmeta_data.ErrorMessage());
  }

  StatusOr<const AvbHashtreeDescriptor*> descriptor =
      findDescriptor(vbmeta_data->get(), (*footer)->vbmeta_size);
  if (!descriptor.Ok()) {
    return StatusOr<ApexVerityData>::MakeError(descriptor.ErrorMessage());
  }

  StatusOr<std::unique_ptr<AvbHashtreeDescriptor>> verifiedDescriptor =
      verifyDescriptor(*descriptor);
  if (!verifiedDescriptor.Ok()) {
    return StatusOr<ApexVerityData>::MakeError(
        verifiedDescriptor.ErrorMessage());
  }
  verityData.desc = std::move(*verifiedDescriptor);

  // This area is now safe to access, because we just verified it
  const uint8_t* trailingData =
      (const uint8_t*)*descriptor + sizeof(AvbHashtreeDescriptor);
  verityData.salt = getSalt(*verityData.desc, trailingData);
  verityData.root_digest = getDigest(*verityData.desc, trailingData);

  return StatusOr<ApexVerityData>(std::move(verityData));
}

Status ApexFile::VerifyManifestMatches(const std::string& mount_path) const {
  std::string manifest_content;
  const std::string manifest_path = mount_path + "/" + kManifestFilename;

  if (!android::base::ReadFileToString(manifest_path, &manifest_content)) {
    std::string err = StringLog()
                      << "Failed to read manifest file: " << manifest_path;
    return Status::Fail(err);
  }

  StatusOr<ApexManifest> verifiedManifest = ParseManifest(manifest_content);
  if (!verifiedManifest.Ok()) {
    return Status::Fail(verifiedManifest.ErrorMessage());
  }

  if (!MessageDifferencer::Equals(manifest_, *verifiedManifest)) {
    return Status::Fail(
        "Manifest inside filesystem does not match manifest outside it");
  }

  return Status::Success();
}

StatusOr<std::vector<std::string>> FindApexes(
    const std::vector<std::string>& paths) {
  using StatusT = StatusOr<std::vector<std::string>>;
  std::vector<std::string> result;
  for (const auto& path : paths) {
    auto exist = PathExists(path);
    if (!exist.Ok()) {
      return StatusT::MakeError(exist.ErrorStatus());
    }
    if (!*exist) continue;

    const auto& apexes =
        FindApexFilesByName(path, isPathForBuiltinApexes(path));
    if (!apexes.Ok()) {
      return apexes;
    }

    result.insert(result.end(), apexes->begin(), apexes->end());
  }
  return StatusOr<std::vector<std::string>>(result);
}

StatusOr<std::vector<std::string>> FindApexFilesByName(const std::string& path,
                                                       bool include_dirs) {
  auto filter_fn =
      [include_dirs](const std::filesystem::directory_entry& entry) {
        std::error_code ec;
        if (entry.is_regular_file(ec) &&
            EndsWith(entry.path().filename().string(), kApexPackageSuffix)) {
          return true;  // APEX file, take.
        }
        // Directory and asked to scan for flattened.
        return entry.is_directory(ec) && include_dirs;
      };
  return ReadDir(path, filter_fn);
}

bool isPathForBuiltinApexes(const std::string& path) {
  for (const auto& dir : kApexPackageBuiltinDirs) {
    if (StartsWith(path, dir)) {
      return true;
    }
  }
  return false;
}

}  // namespace apex
}  // namespace android
