//
// Copyright (C) 2012 The Android Open Source Project
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

#include "update_engine/payload_consumer/delta_performer.h"

#include <endian.h>
#include <inttypes.h>
#include <time.h>

#include <memory>
#include <string>
#include <vector>

#include <base/files/file_path.h>
#include <base/files/file_util.h>
#include <base/files/scoped_temp_dir.h>
#include <base/strings/string_number_conversions.h>
#include <base/strings/string_util.h>
#include <base/strings/stringprintf.h>
#include <gmock/gmock.h>
#include <google/protobuf/repeated_field.h>
#include <gtest/gtest.h>

#include "update_engine/common/constants.h"
#include "update_engine/common/fake_boot_control.h"
#include "update_engine/common/fake_hardware.h"
#include "update_engine/common/fake_prefs.h"
#include "update_engine/common/test_utils.h"
#include "update_engine/common/utils.h"
#include "update_engine/payload_consumer/fake_file_descriptor.h"
#include "update_engine/payload_consumer/mock_download_action.h"
#include "update_engine/payload_consumer/payload_constants.h"
#include "update_engine/payload_generator/bzip.h"
#include "update_engine/payload_generator/extent_ranges.h"
#include "update_engine/payload_generator/payload_file.h"
#include "update_engine/payload_generator/payload_signer.h"
#include "update_engine/update_metadata.pb.h"

namespace chromeos_update_engine {

using std::string;
using std::vector;
using test_utils::GetBuildArtifactsPath;
using test_utils::kRandomString;
using test_utils::System;
using testing::_;

extern const char* kUnittestPrivateKeyPath;
extern const char* kUnittestPublicKeyPath;

namespace {

const char kBogusMetadataSignature1[] =
    "awSFIUdUZz2VWFiR+ku0Pj00V7bPQPQFYQSXjEXr3vaw3TE4xHV5CraY3/YrZpBv"
    "J5z4dSBskoeuaO1TNC/S6E05t+yt36tE4Fh79tMnJ/z9fogBDXWgXLEUyG78IEQr"
    "YH6/eBsQGT2RJtBgXIXbZ9W+5G9KmGDoPOoiaeNsDuqHiBc/58OFsrxskH8E6vMS"
    "BmMGGk82mvgzic7ApcoURbCGey1b3Mwne/hPZ/bb9CIyky8Og9IfFMdL2uAweOIR"
    "fjoTeLYZpt+WN65Vu7jJ0cQN8e1y+2yka5112wpRf/LLtPgiAjEZnsoYpLUd7CoV"
    "pLRtClp97kN2+tXGNBQqkA==";

// Different options that determine what we should fill into the
// install_plan.metadata_signature to simulate the contents received in the
// Omaha response.
enum MetadataSignatureTest {
  kEmptyMetadataSignature,
  kInvalidMetadataSignature,
  kValidMetadataSignature,
};

// Compressed data without checksum, generated with:
// echo -n "a$(head -c 4095 /dev/zero)" | xz -9 --check=none |
//     hexdump -v -e '"    " 12/1 "0x%02x, " "\n"'
const uint8_t kXzCompressedData[] = {
    0xfd, 0x37, 0x7a, 0x58, 0x5a, 0x00, 0x00, 0x00, 0xff, 0x12, 0xd9, 0x41,
    0x02, 0x00, 0x21, 0x01, 0x1c, 0x00, 0x00, 0x00, 0x10, 0xcf, 0x58, 0xcc,
    0xe0, 0x0f, 0xff, 0x00, 0x1b, 0x5d, 0x00, 0x30, 0x80, 0x33, 0xff, 0xdf,
    0xff, 0x51, 0xd6, 0xaf, 0x90, 0x1c, 0x1b, 0x4c, 0xaa, 0x3d, 0x7b, 0x28,
    0xe4, 0x7a, 0x74, 0xbc, 0xe5, 0xa7, 0x33, 0x4e, 0xcf, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x2f, 0x80, 0x20, 0x00, 0x00, 0x00, 0x92, 0x7c, 0x7b, 0x24,
    0xa8, 0x00, 0x0a, 0xfc, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x59, 0x5a,
};

// clang-format off
const uint8_t src_deflates[] = {
  /* raw      0  */ 0x11, 0x22,
  /* deflate  2  */ 0x63, 0x64, 0x62, 0x66, 0x61, 0x05, 0x00,
  /* raw      9  */ 0x33,
  /* deflate  10 */ 0x03, 0x00,
  /* raw      12 */
  /* deflate  12 */ 0x63, 0x04, 0x00,
  /* raw      15 */ 0x44, 0x55
};

const uint8_t dst_deflates[] = {
  /* deflate  0  */ 0x63, 0x64, 0x62, 0x66, 0x61, 0x05, 0x00,
  /* raw      7  */ 0x33, 0x66,
  /* deflate  9  */ 0x01, 0x05, 0x00, 0xFA, 0xFF, 0x01, 0x02, 0x03, 0x04, 0x05,
  /* deflate  19 */ 0x63, 0x04, 0x00
};
// clang-format on

// To generate this patch either:
// - Use puffin/src/patching_unittest.cc:TestPatching
// Or
// - Use the following approach:
// * Make src_deflate a string of hex with only spaces. (e.g. "0XTE 0xST")
// * echo "0XTE 0xST" | xxd -r -p > src.bin
// * Find the location of deflates in src_deflates (in bytes) in the format of
//   "offset:length,...". (e.g. "2:7,10:2,12:3")
// * Do previous three steps for dst_deflates.
// * puffin --operation=puffdiff --src_file=src.bin --dst_file=dst.bin \
//   --src_deflates_byte="2:7,10:2,12:3" --dst_deflates_byte="0:7,9:10,19:3" \
//   --patch_file=patch.bin
// * hexdump -ve '"  " 12/1 "0x%02x, " "\n"' patch.bin
const uint8_t puffdiff_patch[] = {
    0x50, 0x55, 0x46, 0x31, 0x00, 0x00, 0x00, 0x51, 0x08, 0x01, 0x12, 0x27,
    0x0A, 0x04, 0x08, 0x10, 0x10, 0x32, 0x0A, 0x04, 0x08, 0x50, 0x10, 0x0A,
    0x0A, 0x04, 0x08, 0x60, 0x10, 0x12, 0x12, 0x04, 0x08, 0x10, 0x10, 0x58,
    0x12, 0x04, 0x08, 0x78, 0x10, 0x28, 0x12, 0x05, 0x08, 0xA8, 0x01, 0x10,
    0x38, 0x18, 0x1F, 0x1A, 0x24, 0x0A, 0x02, 0x10, 0x32, 0x0A, 0x04, 0x08,
    0x48, 0x10, 0x50, 0x0A, 0x05, 0x08, 0x98, 0x01, 0x10, 0x12, 0x12, 0x02,
    0x10, 0x58, 0x12, 0x04, 0x08, 0x70, 0x10, 0x58, 0x12, 0x05, 0x08, 0xC8,
    0x01, 0x10, 0x38, 0x18, 0x21, 0x42, 0x53, 0x44, 0x49, 0x46, 0x46, 0x34,
    0x30, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x42, 0x5A, 0x68, 0x39, 0x31, 0x41, 0x59, 0x26, 0x53, 0x59, 0x65,
    0x29, 0x8C, 0x9B, 0x00, 0x00, 0x03, 0x60, 0x40, 0x7A, 0x0E, 0x08, 0x00,
    0x40, 0x00, 0x20, 0x00, 0x21, 0x22, 0x9A, 0x3D, 0x4F, 0x50, 0x40, 0x0C,
    0x3B, 0xC7, 0x9B, 0xB2, 0x21, 0x0E, 0xE9, 0x15, 0x98, 0x7A, 0x7C, 0x5D,
    0xC9, 0x14, 0xE1, 0x42, 0x41, 0x94, 0xA6, 0x32, 0x6C, 0x42, 0x5A, 0x68,
    0x39, 0x31, 0x41, 0x59, 0x26, 0x53, 0x59, 0xF1, 0x20, 0x5F, 0x0D, 0x00,
    0x00, 0x02, 0x41, 0x15, 0x42, 0x08, 0x20, 0x00, 0x40, 0x00, 0x00, 0x02,
    0x40, 0x00, 0x20, 0x00, 0x22, 0x3D, 0x23, 0x10, 0x86, 0x03, 0x96, 0x54,
    0x11, 0x16, 0x5F, 0x17, 0x72, 0x45, 0x38, 0x50, 0x90, 0xF1, 0x20, 0x5F,
    0x0D, 0x42, 0x5A, 0x68, 0x39, 0x31, 0x41, 0x59, 0x26, 0x53, 0x59, 0x07,
    0xD4, 0xCB, 0x6E, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x20, 0x00,
    0x21, 0x18, 0x46, 0x82, 0xEE, 0x48, 0xA7, 0x0A, 0x12, 0x00, 0xFA, 0x99,
    0x6D, 0xC0};

}  // namespace

class DeltaPerformerTest : public ::testing::Test {
 protected:
  void SetUp() override {
    install_plan_.source_slot = 0;
    install_plan_.target_slot = 1;
    EXPECT_CALL(mock_delegate_, ShouldCancel(_))
        .WillRepeatedly(testing::Return(false));
  }

  // Test helper placed where it can easily be friended from DeltaPerformer.
  void RunManifestValidation(const DeltaArchiveManifest& manifest,
                             uint64_t major_version,
                             InstallPayloadType payload_type,
                             ErrorCode expected) {
    payload_.type = payload_type;

    // The Manifest we are validating.
    performer_.manifest_.CopyFrom(manifest);
    performer_.major_payload_version_ = major_version;

    EXPECT_EQ(expected, performer_.ValidateManifest());
  }

  brillo::Blob GeneratePayload(const brillo::Blob& blob_data,
                               const vector<AnnotatedOperation>& aops,
                               bool sign_payload,
                               PartitionConfig* old_part = nullptr) {
    return GeneratePayload(blob_data,
                           aops,
                           sign_payload,
                           kMaxSupportedMajorPayloadVersion,
                           kMaxSupportedMinorPayloadVersion,
                           old_part);
  }

  brillo::Blob GeneratePayload(const brillo::Blob& blob_data,
                               const vector<AnnotatedOperation>& aops,
                               bool sign_payload,
                               uint64_t major_version,
                               uint32_t minor_version,
                               PartitionConfig* old_part = nullptr) {
    test_utils::ScopedTempFile blob_file("Blob-XXXXXX");
    EXPECT_TRUE(test_utils::WriteFileVector(blob_file.path(), blob_data));

    PayloadGenerationConfig config;
    config.version.major = major_version;
    config.version.minor = minor_version;

    PayloadFile payload;
    EXPECT_TRUE(payload.Init(config));

    std::unique_ptr<PartitionConfig> old_part_uptr;
    if (!old_part) {
      old_part_uptr = std::make_unique<PartitionConfig>(kPartitionNameRoot);
      old_part = old_part_uptr.get();
    }
    if (minor_version != kFullPayloadMinorVersion) {
      // When generating a delta payload we need to include the old partition
      // information to mark it as a delta payload.
      if (old_part->path.empty()) {
        old_part->path = "/dev/null";
      }
    }
    PartitionConfig new_part(kPartitionNameRoot);
    new_part.path = "/dev/zero";
    new_part.size = 1234;

    payload.AddPartition(*old_part, new_part, aops);

    // We include a kernel partition without operations.
    old_part->name = kPartitionNameKernel;
    new_part.name = kPartitionNameKernel;
    new_part.size = 0;
    payload.AddPartition(*old_part, new_part, {});

    test_utils::ScopedTempFile payload_file("Payload-XXXXXX");
    string private_key =
        sign_payload ? GetBuildArtifactsPath(kUnittestPrivateKeyPath) : "";
    EXPECT_TRUE(payload.WritePayload(payload_file.path(),
                                     blob_file.path(),
                                     private_key,
                                     &payload_.metadata_size));

    brillo::Blob payload_data;
    EXPECT_TRUE(utils::ReadFile(payload_file.path(), &payload_data));
    return payload_data;
  }

  brillo::Blob GenerateSourceCopyPayload(const brillo::Blob& copied_data,
                                         bool add_hash,
                                         PartitionConfig* old_part = nullptr) {
    PayloadGenerationConfig config;
    const uint64_t kDefaultBlockSize = config.block_size;
    EXPECT_EQ(0U, copied_data.size() % kDefaultBlockSize);
    uint64_t num_blocks = copied_data.size() / kDefaultBlockSize;
    AnnotatedOperation aop;
    *(aop.op.add_src_extents()) = ExtentForRange(0, num_blocks);
    *(aop.op.add_dst_extents()) = ExtentForRange(0, num_blocks);
    aop.op.set_type(InstallOperation::SOURCE_COPY);
    brillo::Blob src_hash;
    EXPECT_TRUE(HashCalculator::RawHashOfData(copied_data, &src_hash));
    if (add_hash)
      aop.op.set_src_sha256_hash(src_hash.data(), src_hash.size());

    return GeneratePayload(brillo::Blob(), {aop}, false, old_part);
  }

  // Apply |payload_data| on partition specified in |source_path|.
  // Expect result of performer_.Write() to be |expect_success|.
  // Returns the result of the payload application.
  brillo::Blob ApplyPayload(const brillo::Blob& payload_data,
                            const string& source_path,
                            bool expect_success) {
    return ApplyPayloadToData(
        payload_data, source_path, brillo::Blob(), expect_success);
  }

  // Apply the payload provided in |payload_data| reading from the |source_path|
  // file and writing the contents to a new partition. The existing data in the
  // new target file are set to |target_data| before applying the payload.
  // Expect result of performer_.Write() to be |expect_success|.
  // Returns the result of the payload application.
  brillo::Blob ApplyPayloadToData(const brillo::Blob& payload_data,
                                  const string& source_path,
                                  const brillo::Blob& target_data,
                                  bool expect_success) {
    test_utils::ScopedTempFile new_part("Partition-XXXXXX");
    EXPECT_TRUE(test_utils::WriteFileVector(new_part.path(), target_data));

    // We installed the operations only in the rootfs partition, but the
    // delta performer needs to access all the partitions.
    fake_boot_control_.SetPartitionDevice(
        kPartitionNameRoot, install_plan_.target_slot, new_part.path());
    fake_boot_control_.SetPartitionDevice(
        kPartitionNameRoot, install_plan_.source_slot, source_path);
    fake_boot_control_.SetPartitionDevice(
        kPartitionNameKernel, install_plan_.target_slot, "/dev/null");
    fake_boot_control_.SetPartitionDevice(
        kPartitionNameKernel, install_plan_.source_slot, "/dev/null");

    EXPECT_EQ(expect_success,
              performer_.Write(payload_data.data(), payload_data.size()));
    EXPECT_EQ(0, performer_.Close());

    brillo::Blob partition_data;
    EXPECT_TRUE(utils::ReadFile(new_part.path(), &partition_data));
    return partition_data;
  }

  // Calls delta performer's Write method by pretending to pass in bytes from a
  // delta file whose metadata size is actual_metadata_size and tests if all
  // checks are correctly performed if the install plan contains
  // expected_metadata_size and that the result of the parsing are as per
  // hash_checks_mandatory flag.
  void DoMetadataSizeTest(uint64_t expected_metadata_size,
                          uint64_t actual_metadata_size,
                          bool hash_checks_mandatory) {
    install_plan_.hash_checks_mandatory = hash_checks_mandatory;

    // Set a valid magic string and version number 1.
    EXPECT_TRUE(performer_.Write("CrAU", 4));
    uint64_t version = htobe64(kChromeOSMajorPayloadVersion);
    EXPECT_TRUE(performer_.Write(&version, 8));

    payload_.metadata_size = expected_metadata_size;
    ErrorCode error_code;
    // When filling in size in manifest, exclude the size of the 20-byte header.
    uint64_t size_in_manifest = htobe64(actual_metadata_size - 20);
    bool result = performer_.Write(&size_in_manifest, 8, &error_code);
    if (expected_metadata_size == actual_metadata_size ||
        !hash_checks_mandatory) {
      EXPECT_TRUE(result);
    } else {
      EXPECT_FALSE(result);
      EXPECT_EQ(ErrorCode::kDownloadInvalidMetadataSize, error_code);
    }

    EXPECT_LT(performer_.Close(), 0);
  }

  // Generates a valid delta file but tests the delta performer by suppling
  // different metadata signatures as per metadata_signature_test flag and
  // sees if the result of the parsing are as per hash_checks_mandatory flag.
  void DoMetadataSignatureTest(MetadataSignatureTest metadata_signature_test,
                               bool sign_payload,
                               bool hash_checks_mandatory) {
    // Loads the payload and parses the manifest.
    brillo::Blob payload = GeneratePayload(brillo::Blob(),
                                           vector<AnnotatedOperation>(),
                                           sign_payload,
                                           kChromeOSMajorPayloadVersion,
                                           kFullPayloadMinorVersion);

    LOG(INFO) << "Payload size: " << payload.size();

    install_plan_.hash_checks_mandatory = hash_checks_mandatory;

    MetadataParseResult expected_result, actual_result;
    ErrorCode expected_error, actual_error;

    // Fill up the metadata signature in install plan according to the test.
    switch (metadata_signature_test) {
      case kEmptyMetadataSignature:
        payload_.metadata_signature.clear();
        expected_result = MetadataParseResult::kError;
        expected_error = ErrorCode::kDownloadMetadataSignatureMissingError;
        break;

      case kInvalidMetadataSignature:
        payload_.metadata_signature = kBogusMetadataSignature1;
        expected_result = MetadataParseResult::kError;
        expected_error = ErrorCode::kDownloadMetadataSignatureMismatch;
        break;

      case kValidMetadataSignature:
      default:
        // Set the install plan's metadata size to be the same as the one
        // in the manifest so that we pass the metadata size checks. Only
        // then we can get to manifest signature checks.
        ASSERT_TRUE(PayloadSigner::GetMetadataSignature(
            payload.data(),
            payload_.metadata_size,
            GetBuildArtifactsPath(kUnittestPrivateKeyPath),
            &payload_.metadata_signature));
        EXPECT_FALSE(payload_.metadata_signature.empty());
        expected_result = MetadataParseResult::kSuccess;
        expected_error = ErrorCode::kSuccess;
        break;
    }

    // Ignore the expected result/error if hash checks are not mandatory.
    if (!hash_checks_mandatory) {
      expected_result = MetadataParseResult::kSuccess;
      expected_error = ErrorCode::kSuccess;
    }

    // Use the public key corresponding to the private key used above to
    // sign the metadata.
    string public_key_path = GetBuildArtifactsPath(kUnittestPublicKeyPath);
    EXPECT_TRUE(utils::FileExists(public_key_path.c_str()));
    performer_.set_public_key_path(public_key_path);

    // Init actual_error with an invalid value so that we make sure
    // ParsePayloadMetadata properly populates it in all cases.
    actual_error = ErrorCode::kUmaReportedMax;
    actual_result = performer_.ParsePayloadMetadata(payload, &actual_error);

    EXPECT_EQ(expected_result, actual_result);
    EXPECT_EQ(expected_error, actual_error);

    // Check that the parsed metadata size is what's expected. This test
    // implicitly confirms that the metadata signature is valid, if required.
    EXPECT_EQ(payload_.metadata_size, performer_.metadata_size_);
  }

  // Helper function to pretend that the ECC file descriptor was already opened.
  // Returns a pointer to the created file descriptor.
  FakeFileDescriptor* SetFakeECCFile(size_t size) {
    EXPECT_FALSE(performer_.source_ecc_fd_) << "source_ecc_fd_ already open.";
    FakeFileDescriptor* ret = new FakeFileDescriptor();
    fake_ecc_fd_.reset(ret);
    // Call open to simulate it was already opened.
    ret->Open("", 0);
    ret->SetFileSize(size);
    performer_.source_ecc_fd_ = fake_ecc_fd_;
    return ret;
  }

  uint64_t GetSourceEccRecoveredFailures() const {
    return performer_.source_ecc_recovered_failures_;
  }

  FakePrefs prefs_;
  InstallPlan install_plan_;
  InstallPlan::Payload payload_;
  FakeBootControl fake_boot_control_;
  FakeHardware fake_hardware_;
  MockDownloadActionDelegate mock_delegate_;
  FileDescriptorPtr fake_ecc_fd_;
  DeltaPerformer performer_{&prefs_,
                            &fake_boot_control_,
                            &fake_hardware_,
                            &mock_delegate_,
                            &install_plan_,
                            &payload_,
                            false /* interactive*/};
};

TEST_F(DeltaPerformerTest, FullPayloadWriteTest) {
  payload_.type = InstallPayloadType::kFull;
  brillo::Blob expected_data =
      brillo::Blob(std::begin(kRandomString), std::end(kRandomString));
  expected_data.resize(4096);  // block size
  vector<AnnotatedOperation> aops;
  AnnotatedOperation aop;
  *(aop.op.add_dst_extents()) = ExtentForRange(0, 1);
  aop.op.set_data_offset(0);
  aop.op.set_data_length(expected_data.size());
  aop.op.set_type(InstallOperation::REPLACE);
  aops.push_back(aop);

  brillo::Blob payload_data = GeneratePayload(expected_data,
                                              aops,
                                              false,
                                              kChromeOSMajorPayloadVersion,
                                              kFullPayloadMinorVersion);

  EXPECT_EQ(expected_data, ApplyPayload(payload_data, "/dev/null", true));
}

TEST_F(DeltaPerformerTest, ShouldCancelTest) {
  payload_.type = InstallPayloadType::kFull;
  brillo::Blob expected_data =
      brillo::Blob(std::begin(kRandomString), std::end(kRandomString));
  expected_data.resize(4096);  // block size
  vector<AnnotatedOperation> aops;
  AnnotatedOperation aop;
  *(aop.op.add_dst_extents()) = ExtentForRange(0, 1);
  aop.op.set_data_offset(0);
  aop.op.set_data_length(expected_data.size());
  aop.op.set_type(InstallOperation::REPLACE);
  aops.push_back(aop);

  brillo::Blob payload_data = GeneratePayload(expected_data,
                                              aops,
                                              false,
                                              kChromeOSMajorPayloadVersion,
                                              kFullPayloadMinorVersion);

  testing::Mock::VerifyAndClearExpectations(&mock_delegate_);
  EXPECT_CALL(mock_delegate_, ShouldCancel(_))
      .WillOnce(testing::DoAll(testing::SetArgPointee<0>(ErrorCode::kError),
                               testing::Return(true)));

  ApplyPayload(payload_data, "/dev/null", false);
}

TEST_F(DeltaPerformerTest, ReplaceOperationTest) {
  brillo::Blob expected_data =
      brillo::Blob(std::begin(kRandomString), std::end(kRandomString));
  expected_data.resize(4096);  // block size
  vector<AnnotatedOperation> aops;
  AnnotatedOperation aop;
  *(aop.op.add_dst_extents()) = ExtentForRange(0, 1);
  aop.op.set_data_offset(0);
  aop.op.set_data_length(expected_data.size());
  aop.op.set_type(InstallOperation::REPLACE);
  aops.push_back(aop);

  brillo::Blob payload_data = GeneratePayload(expected_data, aops, false);

  EXPECT_EQ(expected_data, ApplyPayload(payload_data, "/dev/null", true));
}

TEST_F(DeltaPerformerTest, ReplaceBzOperationTest) {
  brillo::Blob expected_data =
      brillo::Blob(std::begin(kRandomString), std::end(kRandomString));
  expected_data.resize(4096);  // block size
  brillo::Blob bz_data;
  EXPECT_TRUE(BzipCompress(expected_data, &bz_data));

  vector<AnnotatedOperation> aops;
  AnnotatedOperation aop;
  *(aop.op.add_dst_extents()) = ExtentForRange(0, 1);
  aop.op.set_data_offset(0);
  aop.op.set_data_length(bz_data.size());
  aop.op.set_type(InstallOperation::REPLACE_BZ);
  aops.push_back(aop);

  brillo::Blob payload_data = GeneratePayload(bz_data, aops, false);

  EXPECT_EQ(expected_data, ApplyPayload(payload_data, "/dev/null", true));
}

TEST_F(DeltaPerformerTest, ReplaceXzOperationTest) {
  brillo::Blob xz_data(std::begin(kXzCompressedData),
                       std::end(kXzCompressedData));
  // The compressed xz data contains a single "a" and padded with zero for the
  // rest of the block.
  brillo::Blob expected_data = brillo::Blob(4096, 0);
  expected_data[0] = 'a';

  AnnotatedOperation aop;
  *(aop.op.add_dst_extents()) = ExtentForRange(0, 1);
  aop.op.set_data_offset(0);
  aop.op.set_data_length(xz_data.size());
  aop.op.set_type(InstallOperation::REPLACE_XZ);
  vector<AnnotatedOperation> aops = {aop};

  brillo::Blob payload_data = GeneratePayload(xz_data, aops, false);

  EXPECT_EQ(expected_data, ApplyPayload(payload_data, "/dev/null", true));
}

TEST_F(DeltaPerformerTest, ZeroOperationTest) {
  brillo::Blob existing_data = brillo::Blob(4096 * 10, 'a');
  brillo::Blob expected_data = existing_data;
  // Blocks 4, 5 and 7 should have zeros instead of 'a' after the operation is
  // applied.
  std::fill(
      expected_data.data() + 4096 * 4, expected_data.data() + 4096 * 6, 0);
  std::fill(
      expected_data.data() + 4096 * 7, expected_data.data() + 4096 * 8, 0);

  AnnotatedOperation aop;
  *(aop.op.add_dst_extents()) = ExtentForRange(4, 2);
  *(aop.op.add_dst_extents()) = ExtentForRange(7, 1);
  aop.op.set_type(InstallOperation::ZERO);
  vector<AnnotatedOperation> aops = {aop};

  brillo::Blob payload_data = GeneratePayload(brillo::Blob(), aops, false);

  EXPECT_EQ(expected_data,
            ApplyPayloadToData(payload_data, "/dev/null", existing_data, true));
}

TEST_F(DeltaPerformerTest, SourceCopyOperationTest) {
  brillo::Blob expected_data(std::begin(kRandomString),
                             std::end(kRandomString));
  expected_data.resize(4096);  // block size
  AnnotatedOperation aop;
  *(aop.op.add_src_extents()) = ExtentForRange(0, 1);
  *(aop.op.add_dst_extents()) = ExtentForRange(0, 1);
  aop.op.set_type(InstallOperation::SOURCE_COPY);
  brillo::Blob src_hash;
  EXPECT_TRUE(HashCalculator::RawHashOfData(expected_data, &src_hash));
  aop.op.set_src_sha256_hash(src_hash.data(), src_hash.size());

  test_utils::ScopedTempFile source("Source-XXXXXX");
  EXPECT_TRUE(test_utils::WriteFileVector(source.path(), expected_data));

  PartitionConfig old_part(kPartitionNameRoot);
  old_part.path = source.path();
  old_part.size = expected_data.size();

  brillo::Blob payload_data =
      GeneratePayload(brillo::Blob(), {aop}, false, &old_part);

  EXPECT_EQ(expected_data, ApplyPayload(payload_data, source.path(), true));
}

TEST_F(DeltaPerformerTest, PuffdiffOperationTest) {
  AnnotatedOperation aop;
  *(aop.op.add_src_extents()) = ExtentForRange(0, 1);
  *(aop.op.add_dst_extents()) = ExtentForRange(0, 1);
  brillo::Blob puffdiff_payload(std::begin(puffdiff_patch),
                                std::end(puffdiff_patch));
  aop.op.set_data_offset(0);
  aop.op.set_data_length(puffdiff_payload.size());
  aop.op.set_type(InstallOperation::PUFFDIFF);
  brillo::Blob src(std::begin(src_deflates), std::end(src_deflates));
  src.resize(4096);  // block size
  brillo::Blob src_hash;
  EXPECT_TRUE(HashCalculator::RawHashOfData(src, &src_hash));
  aop.op.set_src_sha256_hash(src_hash.data(), src_hash.size());

  test_utils::ScopedTempFile source("Source-XXXXXX");
  EXPECT_TRUE(test_utils::WriteFileVector(source.path(), src));

  PartitionConfig old_part(kPartitionNameRoot);
  old_part.path = source.path();
  old_part.size = src.size();

  brillo::Blob payload_data =
      GeneratePayload(puffdiff_payload, {aop}, false, &old_part);

  brillo::Blob dst(std::begin(dst_deflates), std::end(dst_deflates));
  EXPECT_EQ(dst, ApplyPayload(payload_data, source.path(), true));
}

TEST_F(DeltaPerformerTest, SourceHashMismatchTest) {
  brillo::Blob expected_data = {'f', 'o', 'o'};
  brillo::Blob actual_data = {'b', 'a', 'r'};
  expected_data.resize(4096);  // block size
  actual_data.resize(4096);    // block size

  AnnotatedOperation aop;
  *(aop.op.add_src_extents()) = ExtentForRange(0, 1);
  *(aop.op.add_dst_extents()) = ExtentForRange(0, 1);
  aop.op.set_type(InstallOperation::SOURCE_COPY);
  brillo::Blob src_hash;
  EXPECT_TRUE(HashCalculator::RawHashOfData(expected_data, &src_hash));
  aop.op.set_src_sha256_hash(src_hash.data(), src_hash.size());

  test_utils::ScopedTempFile source("Source-XXXXXX");
  EXPECT_TRUE(test_utils::WriteFileVector(source.path(), actual_data));

  PartitionConfig old_part(kPartitionNameRoot);
  old_part.path = source.path();
  old_part.size = actual_data.size();

  brillo::Blob payload_data =
      GeneratePayload(brillo::Blob(), {aop}, false, &old_part);

  EXPECT_EQ(actual_data, ApplyPayload(payload_data, source.path(), false));
}

// Test that the error-corrected file descriptor is used to read the partition
// since the source partition doesn't match the operation hash.
TEST_F(DeltaPerformerTest, ErrorCorrectionSourceCopyFallbackTest) {
  constexpr size_t kCopyOperationSize = 4 * 4096;
  test_utils::ScopedTempFile source("Source-XXXXXX");
  // Write invalid data to the source image, which doesn't match the expected
  // hash.
  brillo::Blob invalid_data(kCopyOperationSize, 0x55);
  EXPECT_TRUE(test_utils::WriteFileVector(source.path(), invalid_data));

  // Setup the fec file descriptor as the fake stream, which matches
  // |expected_data|.
  FakeFileDescriptor* fake_fec = SetFakeECCFile(kCopyOperationSize);
  brillo::Blob expected_data = FakeFileDescriptorData(kCopyOperationSize);

  PartitionConfig old_part(kPartitionNameRoot);
  old_part.path = source.path();
  old_part.size = invalid_data.size();

  brillo::Blob payload_data =
      GenerateSourceCopyPayload(expected_data, true, &old_part);
  EXPECT_EQ(expected_data, ApplyPayload(payload_data, source.path(), true));
  // Verify that the fake_fec was actually used.
  EXPECT_EQ(1U, fake_fec->GetReadOps().size());
  EXPECT_EQ(1U, GetSourceEccRecoveredFailures());
}

// Test that the error-corrected file descriptor is used to read a partition
// when no hash is available for SOURCE_COPY but it falls back to the normal
// file descriptor when the size of the error corrected one is too small.
TEST_F(DeltaPerformerTest, ErrorCorrectionSourceCopyWhenNoHashFallbackTest) {
  constexpr size_t kCopyOperationSize = 4 * 4096;
  test_utils::ScopedTempFile source("Source-XXXXXX");
  // Setup the source path with the right expected data.
  brillo::Blob expected_data = FakeFileDescriptorData(kCopyOperationSize);
  EXPECT_TRUE(test_utils::WriteFileVector(source.path(), expected_data));

  // Setup the fec file descriptor as the fake stream, with smaller data than
  // the expected.
  FakeFileDescriptor* fake_fec = SetFakeECCFile(kCopyOperationSize / 2);

  PartitionConfig old_part(kPartitionNameRoot);
  old_part.path = source.path();
  old_part.size = expected_data.size();

  // The payload operation doesn't include an operation hash.
  brillo::Blob payload_data =
      GenerateSourceCopyPayload(expected_data, false, &old_part);
  EXPECT_EQ(expected_data, ApplyPayload(payload_data, source.path(), true));
  // Verify that the fake_fec was attempted to be used. Since the file
  // descriptor is shorter it can actually do more than one read to realize it
  // reached the EOF.
  EXPECT_LE(1U, fake_fec->GetReadOps().size());
  // This fallback doesn't count as an error-corrected operation since the
  // operation hash was not available.
  EXPECT_EQ(0U, GetSourceEccRecoveredFailures());
}

TEST_F(DeltaPerformerTest, ChooseSourceFDTest) {
  constexpr size_t kSourceSize = 4 * 4096;
  test_utils::ScopedTempFile source("Source-XXXXXX");
  // Write invalid data to the source image, which doesn't match the expected
  // hash.
  brillo::Blob invalid_data(kSourceSize, 0x55);
  EXPECT_TRUE(test_utils::WriteFileVector(source.path(), invalid_data));

  performer_.source_fd_ = std::make_shared<EintrSafeFileDescriptor>();
  performer_.source_fd_->Open(source.path().c_str(), O_RDONLY);
  performer_.block_size_ = 4096;

  // Setup the fec file descriptor as the fake stream, which matches
  // |expected_data|.
  FakeFileDescriptor* fake_fec = SetFakeECCFile(kSourceSize);
  brillo::Blob expected_data = FakeFileDescriptorData(kSourceSize);

  InstallOperation op;
  *(op.add_src_extents()) = ExtentForRange(0, kSourceSize / 4096);
  brillo::Blob src_hash;
  EXPECT_TRUE(HashCalculator::RawHashOfData(expected_data, &src_hash));
  op.set_src_sha256_hash(src_hash.data(), src_hash.size());

  ErrorCode error = ErrorCode::kSuccess;
  EXPECT_EQ(performer_.source_ecc_fd_, performer_.ChooseSourceFD(op, &error));
  EXPECT_EQ(ErrorCode::kSuccess, error);
  // Verify that the fake_fec was actually used.
  EXPECT_EQ(1U, fake_fec->GetReadOps().size());
  EXPECT_EQ(1U, GetSourceEccRecoveredFailures());
}

TEST_F(DeltaPerformerTest, ExtentsToByteStringTest) {
  uint64_t test[] = {1, 1, 4, 2, 0, 1};
  static_assert(arraysize(test) % 2 == 0, "Array size uneven");
  const uint64_t block_size = 4096;
  const uint64_t file_length = 4 * block_size - 13;

  google::protobuf::RepeatedPtrField<Extent> extents;
  for (size_t i = 0; i < arraysize(test); i += 2) {
    *(extents.Add()) = ExtentForRange(test[i], test[i + 1]);
  }

  string expected_output = "4096:4096,16384:8192,0:4083";
  string actual_output;
  EXPECT_TRUE(DeltaPerformer::ExtentsToBsdiffPositionsString(
      extents, block_size, file_length, &actual_output));
  EXPECT_EQ(expected_output, actual_output);
}

TEST_F(DeltaPerformerTest, ValidateManifestFullGoodTest) {
  // The Manifest we are validating.
  DeltaArchiveManifest manifest;
  manifest.mutable_new_kernel_info();
  manifest.mutable_new_rootfs_info();
  manifest.set_minor_version(kFullPayloadMinorVersion);

  RunManifestValidation(manifest,
                        kChromeOSMajorPayloadVersion,
                        InstallPayloadType::kFull,
                        ErrorCode::kSuccess);
}

TEST_F(DeltaPerformerTest, ValidateManifestDeltaGoodTest) {
  // The Manifest we are validating.
  DeltaArchiveManifest manifest;
  manifest.mutable_old_kernel_info();
  manifest.mutable_old_rootfs_info();
  manifest.mutable_new_kernel_info();
  manifest.mutable_new_rootfs_info();
  manifest.set_minor_version(kMaxSupportedMinorPayloadVersion);

  RunManifestValidation(manifest,
                        kChromeOSMajorPayloadVersion,
                        InstallPayloadType::kDelta,
                        ErrorCode::kSuccess);
}

TEST_F(DeltaPerformerTest, ValidateManifestDeltaMinGoodTest) {
  // The Manifest we are validating.
  DeltaArchiveManifest manifest;
  manifest.mutable_old_kernel_info();
  manifest.mutable_old_rootfs_info();
  manifest.mutable_new_kernel_info();
  manifest.mutable_new_rootfs_info();
  manifest.set_minor_version(kMinSupportedMinorPayloadVersion);

  RunManifestValidation(manifest,
                        kChromeOSMajorPayloadVersion,
                        InstallPayloadType::kDelta,
                        ErrorCode::kSuccess);
}

TEST_F(DeltaPerformerTest, ValidateManifestFullUnsetMinorVersion) {
  // The Manifest we are validating.
  DeltaArchiveManifest manifest;

  RunManifestValidation(manifest,
                        kMaxSupportedMajorPayloadVersion,
                        InstallPayloadType::kFull,
                        ErrorCode::kSuccess);
}

TEST_F(DeltaPerformerTest, ValidateManifestDeltaUnsetMinorVersion) {
  // The Manifest we are validating.
  DeltaArchiveManifest manifest;
  // Add an empty old_rootfs_info() to trick the DeltaPerformer into think that
  // this is a delta payload manifest with a missing minor version.
  manifest.mutable_old_rootfs_info();

  RunManifestValidation(manifest,
                        kMaxSupportedMajorPayloadVersion,
                        InstallPayloadType::kDelta,
                        ErrorCode::kUnsupportedMinorPayloadVersion);
}

TEST_F(DeltaPerformerTest, ValidateManifestFullOldKernelTest) {
  // The Manifest we are validating.
  DeltaArchiveManifest manifest;
  manifest.mutable_old_kernel_info();
  manifest.mutable_new_kernel_info();
  manifest.mutable_new_rootfs_info();
  manifest.set_minor_version(kMaxSupportedMinorPayloadVersion);

  RunManifestValidation(manifest,
                        kChromeOSMajorPayloadVersion,
                        InstallPayloadType::kFull,
                        ErrorCode::kPayloadMismatchedType);
}

TEST_F(DeltaPerformerTest, ValidateManifestFullOldRootfsTest) {
  // The Manifest we are validating.
  DeltaArchiveManifest manifest;
  manifest.mutable_old_rootfs_info();
  manifest.mutable_new_kernel_info();
  manifest.mutable_new_rootfs_info();
  manifest.set_minor_version(kMaxSupportedMinorPayloadVersion);

  RunManifestValidation(manifest,
                        kChromeOSMajorPayloadVersion,
                        InstallPayloadType::kFull,
                        ErrorCode::kPayloadMismatchedType);
}

TEST_F(DeltaPerformerTest, ValidateManifestFullPartitionUpdateTest) {
  // The Manifest we are validating.
  DeltaArchiveManifest manifest;
  PartitionUpdate* partition = manifest.add_partitions();
  partition->mutable_old_partition_info();
  partition->mutable_new_partition_info();
  manifest.set_minor_version(kMaxSupportedMinorPayloadVersion);

  RunManifestValidation(manifest,
                        kBrilloMajorPayloadVersion,
                        InstallPayloadType::kFull,
                        ErrorCode::kPayloadMismatchedType);
}

TEST_F(DeltaPerformerTest, ValidateManifestBadMinorVersion) {
  // The Manifest we are validating.
  DeltaArchiveManifest manifest;

  // Generate a bad version number.
  manifest.set_minor_version(kMaxSupportedMinorPayloadVersion + 10000);
  // Mark the manifest as a delta payload by setting old_rootfs_info.
  manifest.mutable_old_rootfs_info();

  RunManifestValidation(manifest,
                        kMaxSupportedMajorPayloadVersion,
                        InstallPayloadType::kDelta,
                        ErrorCode::kUnsupportedMinorPayloadVersion);
}

TEST_F(DeltaPerformerTest, ValidateManifestDowngrade) {
  // The Manifest we are validating.
  DeltaArchiveManifest manifest;

  manifest.set_minor_version(kFullPayloadMinorVersion);
  manifest.set_max_timestamp(1);
  fake_hardware_.SetBuildTimestamp(2);

  RunManifestValidation(manifest,
                        kMaxSupportedMajorPayloadVersion,
                        InstallPayloadType::kFull,
                        ErrorCode::kPayloadTimestampError);
}

TEST_F(DeltaPerformerTest, BrilloMetadataSignatureSizeTest) {
  unsigned int seed = time(nullptr);
  EXPECT_TRUE(performer_.Write(kDeltaMagic, sizeof(kDeltaMagic)));

  uint64_t major_version = htobe64(kBrilloMajorPayloadVersion);
  EXPECT_TRUE(performer_.Write(&major_version, 8));

  uint64_t manifest_size = rand_r(&seed) % 256;
  uint64_t manifest_size_be = htobe64(manifest_size);
  EXPECT_TRUE(performer_.Write(&manifest_size_be, 8));

  uint32_t metadata_signature_size = rand_r(&seed) % 256;
  uint32_t metadata_signature_size_be = htobe32(metadata_signature_size);
  EXPECT_TRUE(performer_.Write(&metadata_signature_size_be, 4));

  EXPECT_LT(performer_.Close(), 0);

  EXPECT_TRUE(performer_.IsHeaderParsed());
  EXPECT_EQ(kBrilloMajorPayloadVersion, performer_.major_payload_version_);
  EXPECT_EQ(24 + manifest_size, performer_.metadata_size_);  // 4 + 8 + 8 + 4
  EXPECT_EQ(metadata_signature_size, performer_.metadata_signature_size_);
}

TEST_F(DeltaPerformerTest, BrilloParsePayloadMetadataTest) {
  brillo::Blob payload_data = GeneratePayload(
      {}, {}, true, kBrilloMajorPayloadVersion, kSourceMinorPayloadVersion);
  install_plan_.hash_checks_mandatory = true;
  performer_.set_public_key_path(GetBuildArtifactsPath(kUnittestPublicKeyPath));
  ErrorCode error;
  EXPECT_EQ(MetadataParseResult::kSuccess,
            performer_.ParsePayloadMetadata(payload_data, &error));
  EXPECT_EQ(ErrorCode::kSuccess, error);
}

TEST_F(DeltaPerformerTest, BadDeltaMagicTest) {
  EXPECT_TRUE(performer_.Write("junk", 4));
  EXPECT_FALSE(performer_.Write("morejunk", 8));
  EXPECT_LT(performer_.Close(), 0);
}

TEST_F(DeltaPerformerTest, MissingMandatoryMetadataSizeTest) {
  DoMetadataSizeTest(0, 75456, true);
}

TEST_F(DeltaPerformerTest, MissingNonMandatoryMetadataSizeTest) {
  DoMetadataSizeTest(0, 123456, false);
}

TEST_F(DeltaPerformerTest, InvalidMandatoryMetadataSizeTest) {
  DoMetadataSizeTest(13000, 140000, true);
}

TEST_F(DeltaPerformerTest, InvalidNonMandatoryMetadataSizeTest) {
  DoMetadataSizeTest(40000, 50000, false);
}

TEST_F(DeltaPerformerTest, ValidMandatoryMetadataSizeTest) {
  DoMetadataSizeTest(85376, 85376, true);
}

TEST_F(DeltaPerformerTest, MandatoryEmptyMetadataSignatureTest) {
  DoMetadataSignatureTest(kEmptyMetadataSignature, true, true);
}

TEST_F(DeltaPerformerTest, NonMandatoryEmptyMetadataSignatureTest) {
  DoMetadataSignatureTest(kEmptyMetadataSignature, true, false);
}

TEST_F(DeltaPerformerTest, MandatoryInvalidMetadataSignatureTest) {
  DoMetadataSignatureTest(kInvalidMetadataSignature, true, true);
}

TEST_F(DeltaPerformerTest, NonMandatoryInvalidMetadataSignatureTest) {
  DoMetadataSignatureTest(kInvalidMetadataSignature, true, false);
}

TEST_F(DeltaPerformerTest, MandatoryValidMetadataSignature1Test) {
  DoMetadataSignatureTest(kValidMetadataSignature, false, true);
}

TEST_F(DeltaPerformerTest, MandatoryValidMetadataSignature2Test) {
  DoMetadataSignatureTest(kValidMetadataSignature, true, true);
}

TEST_F(DeltaPerformerTest, NonMandatoryValidMetadataSignatureTest) {
  DoMetadataSignatureTest(kValidMetadataSignature, true, false);
}

TEST_F(DeltaPerformerTest, UsePublicKeyFromResponse) {
  // The result of the GetPublicKeyResponse() method is based on three things
  //
  //  1. Whether it's an official build; and
  //  2. Whether the Public RSA key to be used is in the root filesystem; and
  //  3. Whether the response has a public key
  //
  // We test all eight combinations to ensure that we only use the
  // public key in the response if
  //
  //  a. it's not an official build; and
  //  b. there is no key in the root filesystem.

  base::ScopedTempDir temp_dir;
  ASSERT_TRUE(temp_dir.CreateUniqueTempDir());
  string non_existing_file = temp_dir.GetPath().Append("non-existing").value();
  string existing_file = temp_dir.GetPath().Append("existing").value();
  constexpr char kExistingKey[] = "Existing";
  ASSERT_TRUE(test_utils::WriteFileString(existing_file, kExistingKey));

  // Non-official build, non-existing public-key, key in response ->
  // kResponseKey
  fake_hardware_.SetIsOfficialBuild(false);
  performer_.public_key_path_ = non_existing_file;
  // This is the result of 'echo -n "Response" | base64' and is not meant to be
  // a valid public key, but it is valid base-64.
  constexpr char kResponseKey[] = "Response";
  constexpr char kBase64ResponseKey[] = "UmVzcG9uc2U=";
  install_plan_.public_key_rsa = kBase64ResponseKey;
  string public_key;
  EXPECT_TRUE(performer_.GetPublicKey(&public_key));
  EXPECT_EQ(public_key, kResponseKey);
  // Same with official build -> no key
  fake_hardware_.SetIsOfficialBuild(true);
  EXPECT_TRUE(performer_.GetPublicKey(&public_key));
  EXPECT_TRUE(public_key.empty());

  // Non-official build, existing public-key, key in response -> kExistingKey
  fake_hardware_.SetIsOfficialBuild(false);
  performer_.public_key_path_ = existing_file;
  install_plan_.public_key_rsa = kBase64ResponseKey;
  EXPECT_TRUE(performer_.GetPublicKey(&public_key));
  EXPECT_EQ(public_key, kExistingKey);
  // Same with official build -> kExistingKey
  fake_hardware_.SetIsOfficialBuild(true);
  EXPECT_TRUE(performer_.GetPublicKey(&public_key));
  EXPECT_EQ(public_key, kExistingKey);

  // Non-official build, non-existing public-key, no key in response -> no key
  fake_hardware_.SetIsOfficialBuild(false);
  performer_.public_key_path_ = non_existing_file;
  install_plan_.public_key_rsa = "";
  EXPECT_TRUE(performer_.GetPublicKey(&public_key));
  EXPECT_TRUE(public_key.empty());
  // Same with official build -> no key
  fake_hardware_.SetIsOfficialBuild(true);
  EXPECT_TRUE(performer_.GetPublicKey(&public_key));
  EXPECT_TRUE(public_key.empty());

  // Non-official build, existing public-key, no key in response -> kExistingKey
  fake_hardware_.SetIsOfficialBuild(false);
  performer_.public_key_path_ = existing_file;
  install_plan_.public_key_rsa = "";
  EXPECT_TRUE(performer_.GetPublicKey(&public_key));
  EXPECT_EQ(public_key, kExistingKey);
  // Same with official build -> kExistingKey
  fake_hardware_.SetIsOfficialBuild(true);
  EXPECT_TRUE(performer_.GetPublicKey(&public_key));
  EXPECT_EQ(public_key, kExistingKey);

  // Non-official build, non-existing public-key, key in response
  // but invalid base64 -> false
  fake_hardware_.SetIsOfficialBuild(false);
  performer_.public_key_path_ = non_existing_file;
  install_plan_.public_key_rsa = "not-valid-base64";
  EXPECT_FALSE(performer_.GetPublicKey(&public_key));
}

TEST_F(DeltaPerformerTest, ConfVersionsMatch) {
  // Test that the versions in update_engine.conf that is installed to the
  // image match the maximum supported delta versions in the update engine.
  uint32_t minor_version;
  brillo::KeyValueStore store;
  EXPECT_TRUE(store.Load(GetBuildArtifactsPath().Append("update_engine.conf")));
  EXPECT_TRUE(utils::GetMinorVersion(store, &minor_version));
  EXPECT_EQ(kMaxSupportedMinorPayloadVersion, minor_version);

  string major_version_str;
  uint64_t major_version;
  EXPECT_TRUE(store.GetString("PAYLOAD_MAJOR_VERSION", &major_version_str));
  EXPECT_TRUE(base::StringToUint64(major_version_str, &major_version));
  EXPECT_EQ(kMaxSupportedMajorPayloadVersion, major_version);
}

}  // namespace chromeos_update_engine
