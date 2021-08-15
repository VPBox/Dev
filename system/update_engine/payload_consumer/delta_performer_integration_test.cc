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

#include <inttypes.h>
#include <sys/mount.h>

#include <algorithm>
#include <string>
#include <vector>

#include <base/files/file_path.h>
#include <base/files/file_util.h>
#include <base/strings/string_util.h>
#include <base/strings/stringprintf.h>
#include <google/protobuf/repeated_field.h>
#include <gtest/gtest.h>
#include <openssl/pem.h>

#include "update_engine/common/constants.h"
#include "update_engine/common/fake_boot_control.h"
#include "update_engine/common/fake_hardware.h"
#include "update_engine/common/mock_prefs.h"
#include "update_engine/common/test_utils.h"
#include "update_engine/common/utils.h"
#include "update_engine/payload_consumer/mock_download_action.h"
#include "update_engine/payload_consumer/payload_constants.h"
#include "update_engine/payload_consumer/payload_metadata.h"
#include "update_engine/payload_consumer/payload_verifier.h"
#include "update_engine/payload_generator/delta_diff_generator.h"
#include "update_engine/payload_generator/payload_signer.h"
#include "update_engine/update_metadata.pb.h"

namespace chromeos_update_engine {

using std::string;
using std::vector;
using test_utils::GetBuildArtifactsPath;
using test_utils::kRandomString;
using test_utils::ScopedLoopMounter;
using test_utils::System;
using testing::_;
using testing::Return;

extern const char* kUnittestPrivateKeyPath;
extern const char* kUnittestPublicKeyPath;
extern const char* kUnittestPrivateKey2Path;
extern const char* kUnittestPublicKey2Path;

static const uint32_t kDefaultKernelSize = 4096;  // Something small for a test
// clang-format off
static const uint8_t kNewData[] = {'T', 'h', 'i', 's', ' ', 'i', 's', ' ',
                                   'n', 'e', 'w', ' ', 'd', 'a', 't', 'a', '.'};
// clang-format on

namespace {
struct DeltaState {
  string a_img;
  string b_img;
  string result_img;
  size_t image_size;

  string delta_path;
  uint64_t metadata_size;

  string old_kernel;
  brillo::Blob old_kernel_data;

  string new_kernel;
  brillo::Blob new_kernel_data;

  string result_kernel;
  brillo::Blob result_kernel_data;
  size_t kernel_size;

  // The InstallPlan referenced by the DeltaPerformer. This needs to outlive
  // the DeltaPerformer.
  InstallPlan install_plan;

  // The in-memory copy of delta file.
  brillo::Blob delta;

  // Mock and fake instances used by the delta performer.
  FakeBootControl fake_boot_control_;
  FakeHardware fake_hardware_;
  MockDownloadActionDelegate mock_delegate_;
};

enum SignatureTest {
  kSignatureNone,                  // No payload signing.
  kSignatureGenerator,             // Sign the payload at generation time.
  kSignatureGenerated,             // Sign the payload after it's generated.
  kSignatureGeneratedPlaceholder,  // Insert placeholder signatures, then real.
  kSignatureGeneratedPlaceholderMismatch,  // Insert a wrong sized placeholder.
  kSignatureGeneratedShell,  // Sign the generated payload through shell cmds.
  kSignatureGeneratedShellBadKey,     // Sign with a bad key through shell cmds.
  kSignatureGeneratedShellRotateCl1,  // Rotate key, test client v1
  kSignatureGeneratedShellRotateCl2,  // Rotate key, test client v2
};

enum OperationHashTest {
  kInvalidOperationData,
  kValidOperationData,
};

}  // namespace

class DeltaPerformerIntegrationTest : public ::testing::Test {};

static void CompareFilesByBlock(const string& a_file,
                                const string& b_file,
                                size_t image_size) {
  EXPECT_EQ(0U, image_size % kBlockSize);

  brillo::Blob a_data, b_data;
  EXPECT_TRUE(utils::ReadFile(a_file, &a_data)) << "file failed: " << a_file;
  EXPECT_TRUE(utils::ReadFile(b_file, &b_data)) << "file failed: " << b_file;

  EXPECT_GE(a_data.size(), image_size);
  EXPECT_GE(b_data.size(), image_size);
  for (size_t i = 0; i < image_size; i += kBlockSize) {
    EXPECT_EQ(0U, i % kBlockSize);
    brillo::Blob a_sub(&a_data[i], &a_data[i + kBlockSize]);
    brillo::Blob b_sub(&b_data[i], &b_data[i + kBlockSize]);
    EXPECT_TRUE(a_sub == b_sub) << "Block " << (i / kBlockSize) << " differs";
  }
  if (::testing::Test::HasNonfatalFailure()) {
    LOG(INFO) << "Compared filesystems with size " << image_size
              << ", partition A " << a_file << " size: " << a_data.size()
              << ", partition B " << b_file << " size: " << b_data.size();
  }
}

static bool WriteSparseFile(const string& path, off_t size) {
  int fd = open(path.c_str(), O_CREAT | O_TRUNC | O_WRONLY, 0644);
  TEST_AND_RETURN_FALSE_ERRNO(fd >= 0);
  ScopedFdCloser fd_closer(&fd);
  off_t rc = lseek(fd, size + 1, SEEK_SET);
  TEST_AND_RETURN_FALSE_ERRNO(rc != static_cast<off_t>(-1));
  int return_code = ftruncate(fd, size);
  TEST_AND_RETURN_FALSE_ERRNO(return_code == 0);
  return true;
}

static bool WriteByteAtOffset(const string& path, off_t offset) {
  int fd = open(path.c_str(), O_CREAT | O_WRONLY, 0644);
  TEST_AND_RETURN_FALSE_ERRNO(fd >= 0);
  ScopedFdCloser fd_closer(&fd);
  EXPECT_TRUE(utils::PWriteAll(fd, "\0", 1, offset));
  return true;
}

static size_t GetSignatureSize(const string& private_key_path) {
  const brillo::Blob data(1, 'x');
  brillo::Blob hash;
  EXPECT_TRUE(HashCalculator::RawHashOfData(data, &hash));
  brillo::Blob signature;
  EXPECT_TRUE(PayloadSigner::SignHash(hash, private_key_path, &signature));
  return signature.size();
}

static bool InsertSignaturePlaceholder(int signature_size,
                                       const string& payload_path,
                                       uint64_t* out_metadata_size) {
  vector<brillo::Blob> signatures;
  signatures.push_back(brillo::Blob(signature_size, 0));

  return PayloadSigner::AddSignatureToPayload(
      payload_path, signatures, {}, payload_path, out_metadata_size);
}

static void SignGeneratedPayload(const string& payload_path,
                                 uint64_t* out_metadata_size) {
  string private_key_path = GetBuildArtifactsPath(kUnittestPrivateKeyPath);
  int signature_size = GetSignatureSize(private_key_path);
  brillo::Blob hash;
  ASSERT_TRUE(PayloadSigner::HashPayloadForSigning(
      payload_path, {signature_size}, &hash, nullptr));
  brillo::Blob signature;
  ASSERT_TRUE(PayloadSigner::SignHash(hash, private_key_path, &signature));
  ASSERT_TRUE(PayloadSigner::AddSignatureToPayload(
      payload_path, {signature}, {}, payload_path, out_metadata_size));
  EXPECT_TRUE(PayloadSigner::VerifySignedPayload(
      payload_path, GetBuildArtifactsPath(kUnittestPublicKeyPath)));
}

static void SignGeneratedShellPayload(SignatureTest signature_test,
                                      const string& payload_path) {
  string private_key_path = GetBuildArtifactsPath(kUnittestPrivateKeyPath);
  if (signature_test == kSignatureGeneratedShellBadKey) {
    ASSERT_TRUE(utils::MakeTempFile("key.XXXXXX", &private_key_path, nullptr));
  } else {
    ASSERT_TRUE(signature_test == kSignatureGeneratedShell ||
                signature_test == kSignatureGeneratedShellRotateCl1 ||
                signature_test == kSignatureGeneratedShellRotateCl2);
  }
  ScopedPathUnlinker key_unlinker(private_key_path);
  key_unlinker.set_should_remove(signature_test ==
                                 kSignatureGeneratedShellBadKey);
  // Generates a new private key that will not match the public key.
  if (signature_test == kSignatureGeneratedShellBadKey) {
    LOG(INFO) << "Generating a mismatched private key.";
    // The code below executes the equivalent of:
    // openssl genrsa -out <private_key_path> 2048
    RSA* rsa = RSA_new();
    BIGNUM* e = BN_new();
    EXPECT_EQ(1, BN_set_word(e, RSA_F4));
    EXPECT_EQ(1, RSA_generate_key_ex(rsa, 2048, e, nullptr));
    BN_free(e);
    FILE* fprikey = fopen(private_key_path.c_str(), "w");
    EXPECT_NE(nullptr, fprikey);
    EXPECT_EQ(1,
              PEM_write_RSAPrivateKey(
                  fprikey, rsa, nullptr, nullptr, 0, nullptr, nullptr));
    fclose(fprikey);
    RSA_free(rsa);
  }
  int signature_size = GetSignatureSize(private_key_path);
  test_utils::ScopedTempFile hash_file("hash.XXXXXX");
  string signature_size_string;
  if (signature_test == kSignatureGeneratedShellRotateCl1 ||
      signature_test == kSignatureGeneratedShellRotateCl2)
    signature_size_string =
        base::StringPrintf("%d:%d", signature_size, signature_size);
  else
    signature_size_string = base::StringPrintf("%d", signature_size);
  string delta_generator_path = GetBuildArtifactsPath("delta_generator");
  ASSERT_EQ(0,
            System(base::StringPrintf(
                "%s -in_file=%s -signature_size=%s -out_hash_file=%s",
                delta_generator_path.c_str(),
                payload_path.c_str(),
                signature_size_string.c_str(),
                hash_file.path().c_str())));

  // Sign the hash
  brillo::Blob hash, signature;
  ASSERT_TRUE(utils::ReadFile(hash_file.path(), &hash));
  ASSERT_TRUE(PayloadSigner::SignHash(hash, private_key_path, &signature));

  test_utils::ScopedTempFile sig_file("signature.XXXXXX");
  ASSERT_TRUE(test_utils::WriteFileVector(sig_file.path(), signature));
  string sig_files = sig_file.path();

  test_utils::ScopedTempFile sig_file2("signature.XXXXXX");
  if (signature_test == kSignatureGeneratedShellRotateCl1 ||
      signature_test == kSignatureGeneratedShellRotateCl2) {
    ASSERT_TRUE(PayloadSigner::SignHash(
        hash, GetBuildArtifactsPath(kUnittestPrivateKey2Path), &signature));
    ASSERT_TRUE(test_utils::WriteFileVector(sig_file2.path(), signature));
    // Append second sig file to first path
    sig_files += ":" + sig_file2.path();
  }

  ASSERT_EQ(0,
            System(base::StringPrintf(
                "%s -in_file=%s -payload_signature_file=%s -out_file=%s",
                delta_generator_path.c_str(),
                payload_path.c_str(),
                sig_files.c_str(),
                payload_path.c_str())));
  int verify_result = System(base::StringPrintf(
      "%s -in_file=%s -public_key=%s -public_key_version=%d",
      delta_generator_path.c_str(),
      payload_path.c_str(),
      (signature_test == kSignatureGeneratedShellRotateCl2
           ? GetBuildArtifactsPath(kUnittestPublicKey2Path)
           : GetBuildArtifactsPath(kUnittestPublicKeyPath))
          .c_str(),
      signature_test == kSignatureGeneratedShellRotateCl2 ? 2 : 1));
  if (signature_test == kSignatureGeneratedShellBadKey) {
    ASSERT_NE(0, verify_result);
  } else {
    ASSERT_EQ(0, verify_result);
  }
}

static void GenerateDeltaFile(bool full_kernel,
                              bool full_rootfs,
                              bool noop,
                              ssize_t chunk_size,
                              SignatureTest signature_test,
                              DeltaState* state,
                              uint32_t minor_version) {
  EXPECT_TRUE(utils::MakeTempFile("a_img.XXXXXX", &state->a_img, nullptr));
  EXPECT_TRUE(utils::MakeTempFile("b_img.XXXXXX", &state->b_img, nullptr));

  // result_img is used in minor version 2. Instead of applying the update
  // in-place on A, we apply it to a new image, result_img.
  EXPECT_TRUE(
      utils::MakeTempFile("result_img.XXXXXX", &state->result_img, nullptr));

  EXPECT_TRUE(
      base::CopyFile(GetBuildArtifactsPath().Append("gen/disk_ext2_4k.img"),
                     base::FilePath(state->a_img)));

  state->image_size = utils::FileSize(state->a_img);

  // Create ImageInfo A & B
  ImageInfo old_image_info;
  ImageInfo new_image_info;

  if (!full_rootfs) {
    old_image_info.set_channel("src-channel");
    old_image_info.set_board("src-board");
    old_image_info.set_version("src-version");
    old_image_info.set_key("src-key");
    old_image_info.set_build_channel("src-build-channel");
    old_image_info.set_build_version("src-build-version");
  }

  new_image_info.set_channel("test-channel");
  new_image_info.set_board("test-board");
  new_image_info.set_version("test-version");
  new_image_info.set_key("test-key");
  new_image_info.set_build_channel("test-build-channel");
  new_image_info.set_build_version("test-build-version");

  // Make some changes to the A image.
  {
    string a_mnt;
    ScopedLoopMounter b_mounter(state->a_img, &a_mnt, 0);

    brillo::Blob hardtocompress;
    while (hardtocompress.size() < 3 * kBlockSize) {
      hardtocompress.insert(hardtocompress.end(),
                            std::begin(kRandomString),
                            std::end(kRandomString));
    }
    EXPECT_TRUE(utils::WriteFile(
        base::StringPrintf("%s/hardtocompress", a_mnt.c_str()).c_str(),
        hardtocompress.data(),
        hardtocompress.size()));

    brillo::Blob zeros(16 * 1024, 0);
    EXPECT_EQ(static_cast<int>(zeros.size()),
              base::WriteFile(base::FilePath(base::StringPrintf(
                                  "%s/move-to-sparse", a_mnt.c_str())),
                              reinterpret_cast<const char*>(zeros.data()),
                              zeros.size()));

    EXPECT_TRUE(WriteSparseFile(
        base::StringPrintf("%s/move-from-sparse", a_mnt.c_str()), 16 * 1024));

    EXPECT_TRUE(WriteByteAtOffset(
        base::StringPrintf("%s/move-semi-sparse", a_mnt.c_str()), 4096));

    // Write 1 MiB of 0xff to try to catch the case where writing a bsdiff
    // patch fails to zero out the final block.
    brillo::Blob ones(1024 * 1024, 0xff);
    EXPECT_TRUE(
        utils::WriteFile(base::StringPrintf("%s/ones", a_mnt.c_str()).c_str(),
                         ones.data(),
                         ones.size()));
  }

  if (noop) {
    EXPECT_TRUE(base::CopyFile(base::FilePath(state->a_img),
                               base::FilePath(state->b_img)));
    old_image_info = new_image_info;
  } else {
    if (minor_version == kSourceMinorPayloadVersion) {
      // Create a result image with image_size bytes of garbage.
      brillo::Blob ones(state->image_size, 0xff);
      EXPECT_TRUE(utils::WriteFile(
          state->result_img.c_str(), ones.data(), ones.size()));
      EXPECT_EQ(utils::FileSize(state->a_img),
                utils::FileSize(state->result_img));
    }

    EXPECT_TRUE(
        base::CopyFile(GetBuildArtifactsPath().Append("gen/disk_ext2_4k.img"),
                       base::FilePath(state->b_img)));

    // Make some changes to the B image.
    string b_mnt;
    ScopedLoopMounter b_mounter(state->b_img, &b_mnt, 0);
    base::FilePath mnt_path(b_mnt);

    EXPECT_TRUE(base::CopyFile(mnt_path.Append("regular-small"),
                               mnt_path.Append("regular-small2")));
    EXPECT_TRUE(base::DeleteFile(mnt_path.Append("regular-small"), false));
    EXPECT_TRUE(base::Move(mnt_path.Append("regular-small2"),
                           mnt_path.Append("regular-small")));
    EXPECT_TRUE(
        test_utils::WriteFileString(mnt_path.Append("foo").value(), "foo"));
    EXPECT_EQ(0, base::WriteFile(mnt_path.Append("emptyfile"), "", 0));

    EXPECT_TRUE(
        WriteSparseFile(mnt_path.Append("fullsparse").value(), 1024 * 1024));
    EXPECT_TRUE(
        WriteSparseFile(mnt_path.Append("move-to-sparse").value(), 16 * 1024));

    brillo::Blob zeros(16 * 1024, 0);
    EXPECT_EQ(static_cast<int>(zeros.size()),
              base::WriteFile(mnt_path.Append("move-from-sparse"),
                              reinterpret_cast<const char*>(zeros.data()),
                              zeros.size()));

    EXPECT_TRUE(
        WriteByteAtOffset(mnt_path.Append("move-semi-sparse").value(), 4096));
    EXPECT_TRUE(WriteByteAtOffset(mnt_path.Append("partsparse").value(), 4096));

    EXPECT_TRUE(
        base::CopyFile(mnt_path.Append("regular-16k"), mnt_path.Append("tmp")));
    EXPECT_TRUE(base::Move(mnt_path.Append("tmp"),
                           mnt_path.Append("link-hard-regular-16k")));

    EXPECT_TRUE(base::DeleteFile(mnt_path.Append("link-short_symlink"), false));
    EXPECT_TRUE(test_utils::WriteFileString(
        mnt_path.Append("link-short_symlink").value(), "foobar"));

    brillo::Blob hardtocompress;
    while (hardtocompress.size() < 3 * kBlockSize) {
      hardtocompress.insert(hardtocompress.end(),
                            std::begin(kRandomString),
                            std::end(kRandomString));
    }
    EXPECT_TRUE(utils::WriteFile(
        base::StringPrintf("%s/hardtocompress", b_mnt.c_str()).c_str(),
        hardtocompress.data(),
        hardtocompress.size()));
  }

  string old_kernel;
  EXPECT_TRUE(
      utils::MakeTempFile("old_kernel.XXXXXX", &state->old_kernel, nullptr));

  string new_kernel;
  EXPECT_TRUE(
      utils::MakeTempFile("new_kernel.XXXXXX", &state->new_kernel, nullptr));

  string result_kernel;
  EXPECT_TRUE(utils::MakeTempFile(
      "result_kernel.XXXXXX", &state->result_kernel, nullptr));

  state->kernel_size = kDefaultKernelSize;
  state->old_kernel_data.resize(kDefaultKernelSize);
  state->new_kernel_data.resize(state->old_kernel_data.size());
  state->result_kernel_data.resize(state->old_kernel_data.size());
  test_utils::FillWithData(&state->old_kernel_data);
  test_utils::FillWithData(&state->new_kernel_data);
  test_utils::FillWithData(&state->result_kernel_data);

  // change the new kernel data
  std::copy(
      std::begin(kNewData), std::end(kNewData), state->new_kernel_data.begin());

  if (noop) {
    state->old_kernel_data = state->new_kernel_data;
  }

  // Write kernels to disk
  EXPECT_TRUE(utils::WriteFile(state->old_kernel.c_str(),
                               state->old_kernel_data.data(),
                               state->old_kernel_data.size()));
  EXPECT_TRUE(utils::WriteFile(state->new_kernel.c_str(),
                               state->new_kernel_data.data(),
                               state->new_kernel_data.size()));
  EXPECT_TRUE(utils::WriteFile(state->result_kernel.c_str(),
                               state->result_kernel_data.data(),
                               state->result_kernel_data.size()));

  EXPECT_TRUE(utils::MakeTempFile("delta.XXXXXX", &state->delta_path, nullptr));
  LOG(INFO) << "delta path: " << state->delta_path;
  {
    const string private_key =
        signature_test == kSignatureGenerator
            ? GetBuildArtifactsPath(kUnittestPrivateKeyPath)
            : "";

    PayloadGenerationConfig payload_config;
    payload_config.is_delta = !full_rootfs;
    payload_config.hard_chunk_size = chunk_size;
    payload_config.rootfs_partition_size = kRootFSPartitionSize;
    payload_config.version.major = kChromeOSMajorPayloadVersion;
    payload_config.version.minor = minor_version;
    if (!full_rootfs) {
      payload_config.source.partitions.emplace_back(kPartitionNameRoot);
      payload_config.source.partitions.emplace_back(kPartitionNameKernel);
      payload_config.source.partitions.front().path = state->a_img;
      if (!full_kernel)
        payload_config.source.partitions.back().path = state->old_kernel;
      payload_config.source.image_info = old_image_info;
      EXPECT_TRUE(payload_config.source.LoadImageSize());
      for (PartitionConfig& part : payload_config.source.partitions)
        EXPECT_TRUE(part.OpenFilesystem());
    } else {
      if (payload_config.hard_chunk_size == -1)
        // Use 1 MiB chunk size for the full unittests.
        payload_config.hard_chunk_size = 1024 * 1024;
    }
    payload_config.target.partitions.emplace_back(kPartitionNameRoot);
    payload_config.target.partitions.back().path = state->b_img;
    payload_config.target.partitions.emplace_back(kPartitionNameKernel);
    payload_config.target.partitions.back().path = state->new_kernel;
    payload_config.target.image_info = new_image_info;
    EXPECT_TRUE(payload_config.target.LoadImageSize());
    for (PartitionConfig& part : payload_config.target.partitions)
      EXPECT_TRUE(part.OpenFilesystem());

    EXPECT_TRUE(payload_config.Validate());
    EXPECT_TRUE(GenerateUpdatePayloadFile(
        payload_config, state->delta_path, private_key, &state->metadata_size));
  }
  // Extend the "partitions" holding the file system a bit.
  EXPECT_EQ(0,
            HANDLE_EINTR(truncate(state->a_img.c_str(),
                                  state->image_size + 1024 * 1024)));
  EXPECT_EQ(static_cast<off_t>(state->image_size + 1024 * 1024),
            utils::FileSize(state->a_img));
  EXPECT_EQ(0,
            HANDLE_EINTR(truncate(state->b_img.c_str(),
                                  state->image_size + 1024 * 1024)));
  EXPECT_EQ(static_cast<off_t>(state->image_size + 1024 * 1024),
            utils::FileSize(state->b_img));

  if (signature_test == kSignatureGeneratedPlaceholder ||
      signature_test == kSignatureGeneratedPlaceholderMismatch) {
    int signature_size =
        GetSignatureSize(GetBuildArtifactsPath(kUnittestPrivateKeyPath));
    LOG(INFO) << "Inserting placeholder signature.";
    ASSERT_TRUE(InsertSignaturePlaceholder(
        signature_size, state->delta_path, &state->metadata_size));

    if (signature_test == kSignatureGeneratedPlaceholderMismatch) {
      signature_size -= 1;
      LOG(INFO) << "Inserting mismatched placeholder signature.";
      ASSERT_FALSE(InsertSignaturePlaceholder(
          signature_size, state->delta_path, &state->metadata_size));
      return;
    }
  }

  if (signature_test == kSignatureGenerated ||
      signature_test == kSignatureGeneratedPlaceholder ||
      signature_test == kSignatureGeneratedPlaceholderMismatch) {
    // Generate the signed payload and update the metadata size in state to
    // reflect the new size after adding the signature operation to the
    // manifest.
    LOG(INFO) << "Signing payload.";
    SignGeneratedPayload(state->delta_path, &state->metadata_size);
  } else if (signature_test == kSignatureGeneratedShell ||
             signature_test == kSignatureGeneratedShellBadKey ||
             signature_test == kSignatureGeneratedShellRotateCl1 ||
             signature_test == kSignatureGeneratedShellRotateCl2) {
    SignGeneratedShellPayload(signature_test, state->delta_path);
  }
}

static void ApplyDeltaFile(bool full_kernel,
                           bool full_rootfs,
                           bool noop,
                           SignatureTest signature_test,
                           DeltaState* state,
                           bool hash_checks_mandatory,
                           OperationHashTest op_hash_test,
                           DeltaPerformer** performer,
                           uint32_t minor_version) {
  // Check the metadata.
  {
    EXPECT_TRUE(utils::ReadFile(state->delta_path, &state->delta));
    PayloadMetadata payload_metadata;
    EXPECT_TRUE(payload_metadata.ParsePayloadHeader(state->delta));
    state->metadata_size = payload_metadata.GetMetadataSize();
    LOG(INFO) << "Metadata size: " << state->metadata_size;

    DeltaArchiveManifest manifest;
    EXPECT_TRUE(payload_metadata.GetManifest(state->delta, &manifest));
    if (signature_test == kSignatureNone) {
      EXPECT_FALSE(manifest.has_signatures_offset());
      EXPECT_FALSE(manifest.has_signatures_size());
    } else {
      EXPECT_TRUE(manifest.has_signatures_offset());
      EXPECT_TRUE(manifest.has_signatures_size());
      Signatures sigs_message;
      EXPECT_TRUE(sigs_message.ParseFromArray(
          &state->delta[state->metadata_size + manifest.signatures_offset()],
          manifest.signatures_size()));
      if (signature_test == kSignatureGeneratedShellRotateCl1 ||
          signature_test == kSignatureGeneratedShellRotateCl2)
        EXPECT_EQ(2, sigs_message.signatures_size());
      else
        EXPECT_EQ(1, sigs_message.signatures_size());
      const Signatures::Signature& signature = sigs_message.signatures(0);
      EXPECT_EQ(1U, signature.version());

      uint64_t expected_sig_data_length = 0;
      vector<string> key_paths{GetBuildArtifactsPath(kUnittestPrivateKeyPath)};
      if (signature_test == kSignatureGeneratedShellRotateCl1 ||
          signature_test == kSignatureGeneratedShellRotateCl2) {
        key_paths.push_back(GetBuildArtifactsPath(kUnittestPrivateKey2Path));
      }
      EXPECT_TRUE(PayloadSigner::SignatureBlobLength(
          key_paths, &expected_sig_data_length));
      EXPECT_EQ(expected_sig_data_length, manifest.signatures_size());
      EXPECT_FALSE(signature.data().empty());
    }

    if (noop) {
      EXPECT_EQ(0, manifest.install_operations_size());
      EXPECT_EQ(1, manifest.kernel_install_operations_size());
    }

    if (full_kernel) {
      EXPECT_FALSE(manifest.has_old_kernel_info());
    } else {
      EXPECT_EQ(state->old_kernel_data.size(),
                manifest.old_kernel_info().size());
      EXPECT_FALSE(manifest.old_kernel_info().hash().empty());
    }

    EXPECT_EQ(manifest.new_image_info().channel(), "test-channel");
    EXPECT_EQ(manifest.new_image_info().board(), "test-board");
    EXPECT_EQ(manifest.new_image_info().version(), "test-version");
    EXPECT_EQ(manifest.new_image_info().key(), "test-key");
    EXPECT_EQ(manifest.new_image_info().build_channel(), "test-build-channel");
    EXPECT_EQ(manifest.new_image_info().build_version(), "test-build-version");

    if (!full_rootfs) {
      if (noop) {
        EXPECT_EQ(manifest.old_image_info().channel(), "test-channel");
        EXPECT_EQ(manifest.old_image_info().board(), "test-board");
        EXPECT_EQ(manifest.old_image_info().version(), "test-version");
        EXPECT_EQ(manifest.old_image_info().key(), "test-key");
        EXPECT_EQ(manifest.old_image_info().build_channel(),
                  "test-build-channel");
        EXPECT_EQ(manifest.old_image_info().build_version(),
                  "test-build-version");
      } else {
        EXPECT_EQ(manifest.old_image_info().channel(), "src-channel");
        EXPECT_EQ(manifest.old_image_info().board(), "src-board");
        EXPECT_EQ(manifest.old_image_info().version(), "src-version");
        EXPECT_EQ(manifest.old_image_info().key(), "src-key");
        EXPECT_EQ(manifest.old_image_info().build_channel(),
                  "src-build-channel");
        EXPECT_EQ(manifest.old_image_info().build_version(),
                  "src-build-version");
      }
    }

    if (full_rootfs) {
      EXPECT_FALSE(manifest.has_old_rootfs_info());
      EXPECT_FALSE(manifest.has_old_image_info());
      EXPECT_TRUE(manifest.has_new_image_info());
    } else {
      EXPECT_EQ(state->image_size, manifest.old_rootfs_info().size());
      EXPECT_FALSE(manifest.old_rootfs_info().hash().empty());
    }

    EXPECT_EQ(state->new_kernel_data.size(), manifest.new_kernel_info().size());
    EXPECT_EQ(state->image_size, manifest.new_rootfs_info().size());

    EXPECT_FALSE(manifest.new_kernel_info().hash().empty());
    EXPECT_FALSE(manifest.new_rootfs_info().hash().empty());
  }

  MockPrefs prefs;
  EXPECT_CALL(prefs, SetInt64(kPrefsManifestMetadataSize, state->metadata_size))
      .WillOnce(Return(true));
  EXPECT_CALL(prefs, SetInt64(kPrefsManifestSignatureSize, 0))
      .WillOnce(Return(true));
  EXPECT_CALL(prefs, SetInt64(kPrefsUpdateStateNextOperation, _))
      .WillRepeatedly(Return(true));
  EXPECT_CALL(prefs, GetInt64(kPrefsUpdateStateNextOperation, _))
      .WillOnce(Return(false));
  EXPECT_CALL(prefs, SetInt64(kPrefsUpdateStateNextDataOffset, _))
      .WillRepeatedly(Return(true));
  EXPECT_CALL(prefs, SetInt64(kPrefsUpdateStateNextDataLength, _))
      .WillRepeatedly(Return(true));
  EXPECT_CALL(prefs, SetString(kPrefsUpdateStateSHA256Context, _))
      .WillRepeatedly(Return(true));
  EXPECT_CALL(prefs, SetString(kPrefsUpdateStateSignedSHA256Context, _))
      .WillRepeatedly(Return(true));
  EXPECT_CALL(prefs, SetBoolean(kPrefsDynamicPartitionMetadataUpdated, _))
      .WillRepeatedly(Return(true));
  if (op_hash_test == kValidOperationData && signature_test != kSignatureNone) {
    EXPECT_CALL(prefs, SetString(kPrefsUpdateStateSignatureBlob, _))
        .WillOnce(Return(true));
  }

  EXPECT_CALL(state->mock_delegate_, ShouldCancel(_))
      .WillRepeatedly(Return(false));

  // Update the A image in place.
  InstallPlan* install_plan = &state->install_plan;
  install_plan->hash_checks_mandatory = hash_checks_mandatory;
  install_plan->payloads = {{.metadata_size = state->metadata_size,
                             .type = (full_kernel && full_rootfs)
                                         ? InstallPayloadType::kFull
                                         : InstallPayloadType::kDelta}};
  install_plan->source_slot = 0;
  install_plan->target_slot = 1;

  InstallPlan::Partition root_part;
  root_part.name = kPartitionNameRoot;

  InstallPlan::Partition kernel_part;
  kernel_part.name = kPartitionNameKernel;

  LOG(INFO) << "Setting payload metadata size in Omaha  = "
            << state->metadata_size;
  ASSERT_TRUE(PayloadSigner::GetMetadataSignature(
      state->delta.data(),
      state->metadata_size,
      GetBuildArtifactsPath(kUnittestPrivateKeyPath),
      &install_plan->payloads[0].metadata_signature));
  EXPECT_FALSE(install_plan->payloads[0].metadata_signature.empty());

  *performer = new DeltaPerformer(&prefs,
                                  &state->fake_boot_control_,
                                  &state->fake_hardware_,
                                  &state->mock_delegate_,
                                  install_plan,
                                  &install_plan->payloads[0],
                                  false /* interactive */);
  string public_key_path = GetBuildArtifactsPath(kUnittestPublicKeyPath);
  EXPECT_TRUE(utils::FileExists(public_key_path.c_str()));
  (*performer)->set_public_key_path(public_key_path);

  EXPECT_EQ(static_cast<off_t>(state->image_size),
            HashCalculator::RawHashOfFile(
                state->a_img, state->image_size, &root_part.source_hash));
  EXPECT_TRUE(HashCalculator::RawHashOfData(state->old_kernel_data,
                                            &kernel_part.source_hash));

  // The partitions should be empty before DeltaPerformer.
  install_plan->partitions.clear();

  // With minor version 2, we want the target to be the new image, result_img,
  // but with version 1, we want to update A in place.
  string target_root, target_kernel;
  if (minor_version == kSourceMinorPayloadVersion) {
    target_root = state->result_img;
    target_kernel = state->result_kernel;
  } else {
    target_root = state->a_img;
    target_kernel = state->old_kernel;
  }

  state->fake_boot_control_.SetPartitionDevice(
      kPartitionNameRoot, install_plan->source_slot, state->a_img);
  state->fake_boot_control_.SetPartitionDevice(
      kPartitionNameKernel, install_plan->source_slot, state->old_kernel);
  state->fake_boot_control_.SetPartitionDevice(
      kPartitionNameRoot, install_plan->target_slot, target_root);
  state->fake_boot_control_.SetPartitionDevice(
      kPartitionNameKernel, install_plan->target_slot, target_kernel);

  ErrorCode expected_error, actual_error;
  bool continue_writing;
  switch (op_hash_test) {
    case kInvalidOperationData: {
      // Muck with some random offset post the metadata size so that
      // some operation hash will result in a mismatch.
      int some_offset = state->metadata_size + 300;
      LOG(INFO) << "Tampered value at offset: " << some_offset;
      state->delta[some_offset]++;
      expected_error = ErrorCode::kDownloadOperationHashMismatch;
      continue_writing = false;
      break;
    }

    case kValidOperationData:
    default:
      // no change.
      expected_error = ErrorCode::kSuccess;
      continue_writing = true;
      break;
  }

  // Write at some number of bytes per operation. Arbitrarily chose 5.
  const size_t kBytesPerWrite = 5;
  for (size_t i = 0; i < state->delta.size(); i += kBytesPerWrite) {
    size_t count = std::min(state->delta.size() - i, kBytesPerWrite);
    bool write_succeeded =
        ((*performer)->Write(&state->delta[i], count, &actual_error));
    // Normally write_succeeded should be true every time and
    // actual_error should be ErrorCode::kSuccess. If so, continue the loop.
    // But if we seeded an operation hash error above, then write_succeeded
    // will be false. The failure may happen at any operation n. So, all
    // Writes until n-1 should succeed and the nth operation will fail with
    // actual_error. In this case, we should bail out of the loop because
    // we cannot proceed applying the delta.
    if (!write_succeeded) {
      LOG(INFO) << "Write failed. Checking if it failed with expected error";
      EXPECT_EQ(expected_error, actual_error);
      if (!continue_writing) {
        LOG(INFO) << "Cannot continue writing. Bailing out.";
        break;
      }
    }

    EXPECT_EQ(ErrorCode::kSuccess, actual_error);
  }

  // If we had continued all the way through, Close should succeed.
  // Otherwise, it should fail. Check appropriately.
  bool close_result = (*performer)->Close();
  if (continue_writing)
    EXPECT_EQ(0, close_result);
  else
    EXPECT_LE(0, close_result);
}

void VerifyPayloadResult(DeltaPerformer* performer,
                         DeltaState* state,
                         ErrorCode expected_result,
                         uint32_t minor_version) {
  if (!performer) {
    EXPECT_TRUE(!"Skipping payload verification since performer is null.");
    return;
  }

  LOG(INFO) << "Verifying payload for expected result " << expected_result;
  brillo::Blob expected_hash;
  HashCalculator::RawHashOfData(state->delta, &expected_hash);
  EXPECT_EQ(expected_result,
            performer->VerifyPayload(expected_hash, state->delta.size()));
  LOG(INFO) << "Verified payload.";

  if (expected_result != ErrorCode::kSuccess) {
    // no need to verify new partition if VerifyPayload failed.
    return;
  }

  brillo::Blob updated_kernel_partition;
  if (minor_version == kSourceMinorPayloadVersion) {
    CompareFilesByBlock(
        state->result_kernel, state->new_kernel, state->kernel_size);
    CompareFilesByBlock(state->result_img, state->b_img, state->image_size);
    EXPECT_TRUE(
        utils::ReadFile(state->result_kernel, &updated_kernel_partition));
  } else {
    CompareFilesByBlock(
        state->old_kernel, state->new_kernel, state->kernel_size);
    CompareFilesByBlock(state->a_img, state->b_img, state->image_size);
    EXPECT_TRUE(utils::ReadFile(state->old_kernel, &updated_kernel_partition));
  }

  ASSERT_GE(updated_kernel_partition.size(), arraysize(kNewData));
  EXPECT_TRUE(std::equal(std::begin(kNewData),
                         std::end(kNewData),
                         updated_kernel_partition.begin()));

  const auto& partitions = state->install_plan.partitions;
  EXPECT_EQ(2U, partitions.size());
  EXPECT_EQ(kPartitionNameRoot, partitions[0].name);
  EXPECT_EQ(kPartitionNameKernel, partitions[1].name);

  EXPECT_EQ(kDefaultKernelSize, partitions[1].target_size);
  brillo::Blob expected_new_kernel_hash;
  EXPECT_TRUE(HashCalculator::RawHashOfData(state->new_kernel_data,
                                            &expected_new_kernel_hash));
  EXPECT_EQ(expected_new_kernel_hash, partitions[1].target_hash);

  EXPECT_EQ(state->image_size, partitions[0].target_size);
  brillo::Blob expected_new_rootfs_hash;
  EXPECT_EQ(static_cast<off_t>(state->image_size),
            HashCalculator::RawHashOfFile(
                state->b_img, state->image_size, &expected_new_rootfs_hash));
  EXPECT_EQ(expected_new_rootfs_hash, partitions[0].target_hash);
}

void VerifyPayload(DeltaPerformer* performer,
                   DeltaState* state,
                   SignatureTest signature_test,
                   uint32_t minor_version) {
  ErrorCode expected_result = ErrorCode::kSuccess;
  switch (signature_test) {
    case kSignatureNone:
      expected_result = ErrorCode::kSignedDeltaPayloadExpectedError;
      break;
    case kSignatureGeneratedShellBadKey:
      expected_result = ErrorCode::kDownloadPayloadPubKeyVerificationError;
      break;
    default:
      break;  // appease gcc
  }

  VerifyPayloadResult(performer, state, expected_result, minor_version);
}

void DoSmallImageTest(bool full_kernel,
                      bool full_rootfs,
                      bool noop,
                      ssize_t chunk_size,
                      SignatureTest signature_test,
                      bool hash_checks_mandatory,
                      uint32_t minor_version) {
  DeltaState state;
  DeltaPerformer* performer = nullptr;
  GenerateDeltaFile(full_kernel,
                    full_rootfs,
                    noop,
                    chunk_size,
                    signature_test,
                    &state,
                    minor_version);

  ScopedPathUnlinker a_img_unlinker(state.a_img);
  ScopedPathUnlinker b_img_unlinker(state.b_img);
  ScopedPathUnlinker new_img_unlinker(state.result_img);
  ScopedPathUnlinker delta_unlinker(state.delta_path);
  ScopedPathUnlinker old_kernel_unlinker(state.old_kernel);
  ScopedPathUnlinker new_kernel_unlinker(state.new_kernel);
  ScopedPathUnlinker result_kernel_unlinker(state.result_kernel);
  ApplyDeltaFile(full_kernel,
                 full_rootfs,
                 noop,
                 signature_test,
                 &state,
                 hash_checks_mandatory,
                 kValidOperationData,
                 &performer,
                 minor_version);
  VerifyPayload(performer, &state, signature_test, minor_version);
  delete performer;
}

void DoOperationHashMismatchTest(OperationHashTest op_hash_test,
                                 bool hash_checks_mandatory) {
  DeltaState state;
  uint64_t minor_version = kFullPayloadMinorVersion;
  GenerateDeltaFile(
      true, true, false, -1, kSignatureGenerated, &state, minor_version);
  ScopedPathUnlinker a_img_unlinker(state.a_img);
  ScopedPathUnlinker b_img_unlinker(state.b_img);
  ScopedPathUnlinker delta_unlinker(state.delta_path);
  ScopedPathUnlinker old_kernel_unlinker(state.old_kernel);
  ScopedPathUnlinker new_kernel_unlinker(state.new_kernel);
  DeltaPerformer* performer = nullptr;
  ApplyDeltaFile(true,
                 true,
                 false,
                 kSignatureGenerated,
                 &state,
                 hash_checks_mandatory,
                 op_hash_test,
                 &performer,
                 minor_version);
  delete performer;
}

TEST(DeltaPerformerIntegrationTest, RunAsRootSmallImageTest) {
  DoSmallImageTest(false,
                   false,
                   false,
                   -1,
                   kSignatureGenerator,
                   false,
                   kInPlaceMinorPayloadVersion);
}

TEST(DeltaPerformerIntegrationTest,
     RunAsRootSmallImageSignaturePlaceholderTest) {
  DoSmallImageTest(false,
                   false,
                   false,
                   -1,
                   kSignatureGeneratedPlaceholder,
                   false,
                   kInPlaceMinorPayloadVersion);
}

TEST(DeltaPerformerIntegrationTest,
     RunAsRootSmallImageSignaturePlaceholderMismatchTest) {
  DeltaState state;
  GenerateDeltaFile(false,
                    false,
                    false,
                    -1,
                    kSignatureGeneratedPlaceholderMismatch,
                    &state,
                    kInPlaceMinorPayloadVersion);
}

TEST(DeltaPerformerIntegrationTest, RunAsRootSmallImageChunksTest) {
  DoSmallImageTest(false,
                   false,
                   false,
                   kBlockSize,
                   kSignatureGenerator,
                   false,
                   kInPlaceMinorPayloadVersion);
}

TEST(DeltaPerformerIntegrationTest, RunAsRootFullKernelSmallImageTest) {
  DoSmallImageTest(true,
                   false,
                   false,
                   -1,
                   kSignatureGenerator,
                   false,
                   kInPlaceMinorPayloadVersion);
}

TEST(DeltaPerformerIntegrationTest, RunAsRootFullSmallImageTest) {
  DoSmallImageTest(true,
                   true,
                   false,
                   -1,
                   kSignatureGenerator,
                   true,
                   kFullPayloadMinorVersion);
}

TEST(DeltaPerformerIntegrationTest, RunAsRootNoopSmallImageTest) {
  DoSmallImageTest(false,
                   false,
                   true,
                   -1,
                   kSignatureGenerator,
                   false,
                   kInPlaceMinorPayloadVersion);
}

TEST(DeltaPerformerIntegrationTest, RunAsRootSmallImageSignNoneTest) {
  DoSmallImageTest(false,
                   false,
                   false,
                   -1,
                   kSignatureNone,
                   false,
                   kInPlaceMinorPayloadVersion);
}

TEST(DeltaPerformerIntegrationTest, RunAsRootSmallImageSignGeneratedTest) {
  DoSmallImageTest(false,
                   false,
                   false,
                   -1,
                   kSignatureGenerated,
                   true,
                   kInPlaceMinorPayloadVersion);
}

TEST(DeltaPerformerIntegrationTest, RunAsRootSmallImageSignGeneratedShellTest) {
  DoSmallImageTest(false,
                   false,
                   false,
                   -1,
                   kSignatureGeneratedShell,
                   false,
                   kInPlaceMinorPayloadVersion);
}

TEST(DeltaPerformerIntegrationTest,
     RunAsRootSmallImageSignGeneratedShellBadKeyTest) {
  DoSmallImageTest(false,
                   false,
                   false,
                   -1,
                   kSignatureGeneratedShellBadKey,
                   false,
                   kInPlaceMinorPayloadVersion);
}

TEST(DeltaPerformerIntegrationTest,
     RunAsRootSmallImageSignGeneratedShellRotateCl1Test) {
  DoSmallImageTest(false,
                   false,
                   false,
                   -1,
                   kSignatureGeneratedShellRotateCl1,
                   false,
                   kInPlaceMinorPayloadVersion);
}

TEST(DeltaPerformerIntegrationTest,
     RunAsRootSmallImageSignGeneratedShellRotateCl2Test) {
  DoSmallImageTest(false,
                   false,
                   false,
                   -1,
                   kSignatureGeneratedShellRotateCl2,
                   false,
                   kInPlaceMinorPayloadVersion);
}

TEST(DeltaPerformerIntegrationTest, RunAsRootSmallImageSourceOpsTest) {
  DoSmallImageTest(false,
                   false,
                   false,
                   -1,
                   kSignatureGenerator,
                   false,
                   kSourceMinorPayloadVersion);
}

TEST(DeltaPerformerIntegrationTest,
     RunAsRootMandatoryOperationHashMismatchTest) {
  DoOperationHashMismatchTest(kInvalidOperationData, true);
}

}  // namespace chromeos_update_engine
