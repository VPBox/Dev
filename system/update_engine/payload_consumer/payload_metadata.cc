//
// Copyright (C) 2018 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include "update_engine/payload_consumer/payload_metadata.h"

#include <endian.h>

#include <brillo/data_encoding.h>

#include "update_engine/common/constants.h"
#include "update_engine/common/hash_calculator.h"
#include "update_engine/common/utils.h"
#include "update_engine/payload_consumer/payload_constants.h"
#include "update_engine/payload_consumer/payload_verifier.h"

using std::string;

namespace chromeos_update_engine {

const uint64_t PayloadMetadata::kDeltaVersionOffset = sizeof(kDeltaMagic);
const uint64_t PayloadMetadata::kDeltaVersionSize = 8;
const uint64_t PayloadMetadata::kDeltaManifestSizeOffset =
    kDeltaVersionOffset + kDeltaVersionSize;
const uint64_t PayloadMetadata::kDeltaManifestSizeSize = 8;
const uint64_t PayloadMetadata::kDeltaMetadataSignatureSizeSize = 4;

bool PayloadMetadata::GetMetadataSignatureSizeOffset(
    uint64_t* out_offset) const {
  if (GetMajorVersion() == kBrilloMajorPayloadVersion) {
    *out_offset = kDeltaManifestSizeOffset + kDeltaManifestSizeSize;
    return true;
  }
  return false;
}

bool PayloadMetadata::GetManifestOffset(uint64_t* out_offset) const {
  // Actual manifest begins right after the manifest size field or
  // metadata signature size field if major version >= 2.
  if (major_payload_version_ == kChromeOSMajorPayloadVersion) {
    *out_offset = kDeltaManifestSizeOffset + kDeltaManifestSizeSize;
    return true;
  }
  if (major_payload_version_ == kBrilloMajorPayloadVersion) {
    *out_offset = kDeltaManifestSizeOffset + kDeltaManifestSizeSize +
                  kDeltaMetadataSignatureSizeSize;
    return true;
  }
  LOG(ERROR) << "Unknown major payload version: " << major_payload_version_;
  return false;
}

MetadataParseResult PayloadMetadata::ParsePayloadHeader(
    const brillo::Blob& payload, ErrorCode* error) {
  uint64_t manifest_offset;
  // Ensure we have data to cover the major payload version.
  if (payload.size() < kDeltaManifestSizeOffset)
    return MetadataParseResult::kInsufficientData;

  // Validate the magic string.
  if (memcmp(payload.data(), kDeltaMagic, sizeof(kDeltaMagic)) != 0) {
    LOG(ERROR) << "Bad payload format -- invalid delta magic.";
    *error = ErrorCode::kDownloadInvalidMetadataMagicString;
    return MetadataParseResult::kError;
  }

  // Extract the payload version from the metadata.
  static_assert(sizeof(major_payload_version_) == kDeltaVersionSize,
                "Major payload version size mismatch");
  memcpy(&major_payload_version_,
         &payload[kDeltaVersionOffset],
         kDeltaVersionSize);
  // Switch big endian to host.
  major_payload_version_ = be64toh(major_payload_version_);

  if (major_payload_version_ < kMinSupportedMajorPayloadVersion ||
      major_payload_version_ > kMaxSupportedMajorPayloadVersion) {
    LOG(ERROR) << "Bad payload format -- unsupported payload version: "
               << major_payload_version_;
    *error = ErrorCode::kUnsupportedMajorPayloadVersion;
    return MetadataParseResult::kError;
  }

  // Get the manifest offset now that we have payload version.
  if (!GetManifestOffset(&manifest_offset)) {
    *error = ErrorCode::kUnsupportedMajorPayloadVersion;
    return MetadataParseResult::kError;
  }
  // Check again with the manifest offset.
  if (payload.size() < manifest_offset)
    return MetadataParseResult::kInsufficientData;

  // Next, parse the manifest size.
  static_assert(sizeof(manifest_size_) == kDeltaManifestSizeSize,
                "manifest_size size mismatch");
  memcpy(&manifest_size_,
         &payload[kDeltaManifestSizeOffset],
         kDeltaManifestSizeSize);
  manifest_size_ = be64toh(manifest_size_);  // switch big endian to host

  metadata_size_ = manifest_offset + manifest_size_;
  if (metadata_size_ < manifest_size_) {
    // Overflow detected.
    *error = ErrorCode::kDownloadInvalidMetadataSize;
    return MetadataParseResult::kError;
  }

  if (GetMajorVersion() == kBrilloMajorPayloadVersion) {
    // Parse the metadata signature size.
    static_assert(
        sizeof(metadata_signature_size_) == kDeltaMetadataSignatureSizeSize,
        "metadata_signature_size size mismatch");
    uint64_t metadata_signature_size_offset;
    if (!GetMetadataSignatureSizeOffset(&metadata_signature_size_offset)) {
      *error = ErrorCode::kError;
      return MetadataParseResult::kError;
    }
    memcpy(&metadata_signature_size_,
           &payload[metadata_signature_size_offset],
           kDeltaMetadataSignatureSizeSize);
    metadata_signature_size_ = be32toh(metadata_signature_size_);

    if (metadata_size_ + metadata_signature_size_ < metadata_size_) {
      // Overflow detected.
      *error = ErrorCode::kDownloadInvalidMetadataSize;
      return MetadataParseResult::kError;
    }
  }
  return MetadataParseResult::kSuccess;
}

bool PayloadMetadata::ParsePayloadHeader(const brillo::Blob& payload) {
  ErrorCode error;
  return ParsePayloadHeader(payload, &error) == MetadataParseResult::kSuccess;
}

bool PayloadMetadata::GetManifest(const brillo::Blob& payload,
                                  DeltaArchiveManifest* out_manifest) const {
  uint64_t manifest_offset;
  if (!GetManifestOffset(&manifest_offset))
    return false;
  CHECK_GE(payload.size(), manifest_offset + manifest_size_);
  return out_manifest->ParseFromArray(&payload[manifest_offset],
                                      manifest_size_);
}

ErrorCode PayloadMetadata::ValidateMetadataSignature(
    const brillo::Blob& payload,
    const string& metadata_signature,
    const string& pem_public_key) const {
  if (payload.size() < metadata_size_ + metadata_signature_size_)
    return ErrorCode::kDownloadMetadataSignatureError;

  // A single signature in raw bytes.
  brillo::Blob metadata_signature_blob;
  // The serialized Signatures protobuf message stored in major version >=2
  // payload, it may contain multiple signatures.
  string metadata_signature_protobuf;
  if (!metadata_signature.empty()) {
    // Convert base64-encoded signature to raw bytes.
    if (!brillo::data_encoding::Base64Decode(metadata_signature,
                                             &metadata_signature_blob)) {
      LOG(ERROR) << "Unable to decode base64 metadata signature: "
                 << metadata_signature;
      return ErrorCode::kDownloadMetadataSignatureError;
    }
  } else if (major_payload_version_ == kBrilloMajorPayloadVersion) {
    metadata_signature_protobuf.assign(
        payload.begin() + metadata_size_,
        payload.begin() + metadata_size_ + metadata_signature_size_);
  }

  if (metadata_signature_blob.empty() && metadata_signature_protobuf.empty()) {
    LOG(ERROR) << "Missing mandatory metadata signature in both Omaha "
               << "response and payload.";
    return ErrorCode::kDownloadMetadataSignatureMissingError;
  }

  brillo::Blob metadata_hash;
  if (!HashCalculator::RawHashOfBytes(
          payload.data(), metadata_size_, &metadata_hash)) {
    LOG(ERROR) << "Unable to compute actual hash of manifest";
    return ErrorCode::kDownloadMetadataSignatureVerificationError;
  }

  if (metadata_hash.size() != kSHA256Size) {
    LOG(ERROR) << "Computed actual hash of metadata has incorrect size: "
               << metadata_hash.size();
    return ErrorCode::kDownloadMetadataSignatureVerificationError;
  }

  if (!metadata_signature_blob.empty()) {
    brillo::Blob expected_metadata_hash;
    if (!PayloadVerifier::GetRawHashFromSignature(
            metadata_signature_blob, pem_public_key, &expected_metadata_hash)) {
      LOG(ERROR) << "Unable to compute expected hash from metadata signature";
      return ErrorCode::kDownloadMetadataSignatureError;
    }

    brillo::Blob padded_metadata_hash = metadata_hash;
    if (!PayloadVerifier::PadRSASHA256Hash(&padded_metadata_hash,
                                           expected_metadata_hash.size())) {
      LOG(ERROR) << "Failed to pad the SHA256 hash to "
                 << expected_metadata_hash.size() << " bytes.";
      return ErrorCode::kDownloadMetadataSignatureVerificationError;
    }

    if (padded_metadata_hash != expected_metadata_hash) {
      LOG(ERROR) << "Manifest hash verification failed. Expected hash = ";
      utils::HexDumpVector(expected_metadata_hash);
      LOG(ERROR) << "Calculated hash = ";
      utils::HexDumpVector(padded_metadata_hash);
      return ErrorCode::kDownloadMetadataSignatureMismatch;
    }
  } else {
    if (!PayloadVerifier::VerifySignature(
            metadata_signature_protobuf, pem_public_key, metadata_hash)) {
      LOG(ERROR) << "Manifest hash verification failed.";
      return ErrorCode::kDownloadMetadataSignatureMismatch;
    }
  }

  // The autoupdate_CatchBadSignatures test checks for this string in
  // log-files. Keep in sync.
  LOG(INFO) << "Metadata hash signature matches value in Omaha response.";
  return ErrorCode::kSuccess;
}

}  // namespace chromeos_update_engine
