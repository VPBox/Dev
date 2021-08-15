//
// Copyright (C) 2015 The Android Open Source Project
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

#include "update_engine/payload_generator/payload_file.h"

#include <endian.h>

#include <algorithm>
#include <map>
#include <utility>

#include <base/strings/stringprintf.h>

#include "update_engine/common/hash_calculator.h"
#include "update_engine/payload_consumer/delta_performer.h"
#include "update_engine/payload_consumer/file_writer.h"
#include "update_engine/payload_consumer/payload_constants.h"
#include "update_engine/payload_generator/annotated_operation.h"
#include "update_engine/payload_generator/delta_diff_utils.h"
#include "update_engine/payload_generator/payload_signer.h"

using std::string;
using std::vector;

namespace chromeos_update_engine {

namespace {

struct DeltaObject {
  DeltaObject(const string& in_name, const int in_type, const off_t in_size)
      : name(in_name), type(in_type), size(in_size) {}
  bool operator<(const DeltaObject& object) const {
    return (size != object.size) ? (size < object.size) : (name < object.name);
  }
  string name;
  int type;
  off_t size;
};

// Writes the uint64_t passed in in host-endian to the file as big-endian.
// Returns true on success.
bool WriteUint64AsBigEndian(FileWriter* writer, const uint64_t value) {
  uint64_t value_be = htobe64(value);
  TEST_AND_RETURN_FALSE(writer->Write(&value_be, sizeof(value_be)));
  return true;
}

}  // namespace

bool PayloadFile::Init(const PayloadGenerationConfig& config) {
  TEST_AND_RETURN_FALSE(config.version.Validate());
  major_version_ = config.version.major;
  manifest_.set_minor_version(config.version.minor);

  if (!config.source.ImageInfoIsEmpty())
    *(manifest_.mutable_old_image_info()) = config.source.image_info;

  if (!config.target.ImageInfoIsEmpty())
    *(manifest_.mutable_new_image_info()) = config.target.image_info;

  manifest_.set_block_size(config.block_size);
  manifest_.set_max_timestamp(config.max_timestamp);

  if (major_version_ == kBrilloMajorPayloadVersion) {
    if (config.target.dynamic_partition_metadata != nullptr)
      *(manifest_.mutable_dynamic_partition_metadata()) =
          *(config.target.dynamic_partition_metadata);
  }

  return true;
}

bool PayloadFile::AddPartition(const PartitionConfig& old_conf,
                               const PartitionConfig& new_conf,
                               const vector<AnnotatedOperation>& aops) {
  // Check partitions order for Chrome OS
  if (major_version_ == kChromeOSMajorPayloadVersion) {
    const vector<const char*> part_order = {kPartitionNameRoot,
                                            kPartitionNameKernel};
    TEST_AND_RETURN_FALSE(part_vec_.size() < part_order.size());
    TEST_AND_RETURN_FALSE(new_conf.name == part_order[part_vec_.size()]);
  }
  Partition part;
  part.name = new_conf.name;
  part.aops = aops;
  part.postinstall = new_conf.postinstall;
  part.verity = new_conf.verity;
  // Initialize the PartitionInfo objects if present.
  if (!old_conf.path.empty())
    TEST_AND_RETURN_FALSE(
        diff_utils::InitializePartitionInfo(old_conf, &part.old_info));
  TEST_AND_RETURN_FALSE(
      diff_utils::InitializePartitionInfo(new_conf, &part.new_info));
  part_vec_.push_back(std::move(part));
  return true;
}

bool PayloadFile::WritePayload(const string& payload_file,
                               const string& data_blobs_path,
                               const string& private_key_path,
                               uint64_t* metadata_size_out) {
  // Reorder the data blobs with the manifest_.
  string ordered_blobs_path;
  TEST_AND_RETURN_FALSE(utils::MakeTempFile(
      "CrAU_temp_data.ordered.XXXXXX", &ordered_blobs_path, nullptr));
  ScopedPathUnlinker ordered_blobs_unlinker(ordered_blobs_path);
  TEST_AND_RETURN_FALSE(ReorderDataBlobs(data_blobs_path, ordered_blobs_path));

  // Check that install op blobs are in order.
  uint64_t next_blob_offset = 0;
  for (const auto& part : part_vec_) {
    for (const auto& aop : part.aops) {
      if (!aop.op.has_data_offset())
        continue;
      if (aop.op.data_offset() != next_blob_offset) {
        LOG(FATAL) << "bad blob offset! " << aop.op.data_offset()
                   << " != " << next_blob_offset;
      }
      next_blob_offset += aop.op.data_length();
    }
  }

  // Copy the operations and partition info from the part_vec_ to the manifest.
  manifest_.clear_install_operations();
  manifest_.clear_kernel_install_operations();
  manifest_.clear_partitions();
  for (const auto& part : part_vec_) {
    if (major_version_ == kBrilloMajorPayloadVersion) {
      PartitionUpdate* partition = manifest_.add_partitions();
      partition->set_partition_name(part.name);
      if (part.postinstall.run) {
        partition->set_run_postinstall(true);
        if (!part.postinstall.path.empty())
          partition->set_postinstall_path(part.postinstall.path);
        if (!part.postinstall.filesystem_type.empty())
          partition->set_filesystem_type(part.postinstall.filesystem_type);
        partition->set_postinstall_optional(part.postinstall.optional);
      }
      if (!part.verity.IsEmpty()) {
        if (part.verity.hash_tree_extent.num_blocks() != 0) {
          *partition->mutable_hash_tree_data_extent() =
              part.verity.hash_tree_data_extent;
          *partition->mutable_hash_tree_extent() = part.verity.hash_tree_extent;
          partition->set_hash_tree_algorithm(part.verity.hash_tree_algorithm);
          if (!part.verity.hash_tree_salt.empty())
            partition->set_hash_tree_salt(part.verity.hash_tree_salt.data(),
                                          part.verity.hash_tree_salt.size());
        }
        if (part.verity.fec_extent.num_blocks() != 0) {
          *partition->mutable_fec_data_extent() = part.verity.fec_data_extent;
          *partition->mutable_fec_extent() = part.verity.fec_extent;
          partition->set_fec_roots(part.verity.fec_roots);
        }
      }
      for (const AnnotatedOperation& aop : part.aops) {
        *partition->add_operations() = aop.op;
      }
      if (part.old_info.has_size() || part.old_info.has_hash())
        *(partition->mutable_old_partition_info()) = part.old_info;
      if (part.new_info.has_size() || part.new_info.has_hash())
        *(partition->mutable_new_partition_info()) = part.new_info;
    } else {
      // major_version_ == kChromeOSMajorPayloadVersion
      if (part.name == kPartitionNameKernel) {
        for (const AnnotatedOperation& aop : part.aops)
          *manifest_.add_kernel_install_operations() = aop.op;
        if (part.old_info.has_size() || part.old_info.has_hash())
          *manifest_.mutable_old_kernel_info() = part.old_info;
        if (part.new_info.has_size() || part.new_info.has_hash())
          *manifest_.mutable_new_kernel_info() = part.new_info;
      } else {
        for (const AnnotatedOperation& aop : part.aops)
          *manifest_.add_install_operations() = aop.op;
        if (part.old_info.has_size() || part.old_info.has_hash())
          *manifest_.mutable_old_rootfs_info() = part.old_info;
        if (part.new_info.has_size() || part.new_info.has_hash())
          *manifest_.mutable_new_rootfs_info() = part.new_info;
      }
    }
  }

  // Signatures appear at the end of the blobs. Note the offset in the
  // manifest_.
  uint64_t signature_blob_length = 0;
  if (!private_key_path.empty()) {
    TEST_AND_RETURN_FALSE(PayloadSigner::SignatureBlobLength(
        {private_key_path}, &signature_blob_length));
    PayloadSigner::AddSignatureToManifest(
        next_blob_offset,
        signature_blob_length,
        major_version_ == kChromeOSMajorPayloadVersion,
        &manifest_);
  }

  // Serialize protobuf
  string serialized_manifest;
  TEST_AND_RETURN_FALSE(manifest_.SerializeToString(&serialized_manifest));

  uint64_t metadata_size =
      sizeof(kDeltaMagic) + 2 * sizeof(uint64_t) + serialized_manifest.size();

  LOG(INFO) << "Writing final delta file header...";
  DirectFileWriter writer;
  TEST_AND_RETURN_FALSE_ERRNO(writer.Open(payload_file.c_str(),
                                          O_WRONLY | O_CREAT | O_TRUNC,
                                          0644) == 0);
  ScopedFileWriterCloser writer_closer(&writer);

  // Write header
  TEST_AND_RETURN_FALSE_ERRNO(writer.Write(kDeltaMagic, sizeof(kDeltaMagic)));

  // Write major version number
  TEST_AND_RETURN_FALSE(WriteUint64AsBigEndian(&writer, major_version_));

  // Write protobuf length
  TEST_AND_RETURN_FALSE(
      WriteUint64AsBigEndian(&writer, serialized_manifest.size()));

  // Write metadata signature size.
  uint32_t metadata_signature_size = 0;
  if (major_version_ == kBrilloMajorPayloadVersion) {
    // Metadata signature has the same size as payload signature, because they
    // are both the same kind of signature for the same kind of hash.
    uint32_t metadata_signature_size = htobe32(signature_blob_length);
    TEST_AND_RETURN_FALSE_ERRNO(writer.Write(&metadata_signature_size,
                                             sizeof(metadata_signature_size)));
    metadata_size += sizeof(metadata_signature_size);
    // Set correct size instead of big endian size.
    metadata_signature_size = signature_blob_length;
  }

  // Write protobuf
  LOG(INFO) << "Writing final delta file protobuf... "
            << serialized_manifest.size();
  TEST_AND_RETURN_FALSE_ERRNO(
      writer.Write(serialized_manifest.data(), serialized_manifest.size()));

  // Write metadata signature blob.
  if (major_version_ == kBrilloMajorPayloadVersion &&
      !private_key_path.empty()) {
    brillo::Blob metadata_hash;
    TEST_AND_RETURN_FALSE(HashCalculator::RawHashOfFile(
        payload_file, metadata_size, &metadata_hash));
    string metadata_signature;
    TEST_AND_RETURN_FALSE(PayloadSigner::SignHashWithKeys(
        metadata_hash, {private_key_path}, &metadata_signature));
    TEST_AND_RETURN_FALSE_ERRNO(
        writer.Write(metadata_signature.data(), metadata_signature.size()));
  }

  // Append the data blobs
  LOG(INFO) << "Writing final delta file data blobs...";
  int blobs_fd = open(ordered_blobs_path.c_str(), O_RDONLY, 0);
  ScopedFdCloser blobs_fd_closer(&blobs_fd);
  TEST_AND_RETURN_FALSE(blobs_fd >= 0);
  for (;;) {
    vector<char> buf(1024 * 1024);
    ssize_t rc = read(blobs_fd, buf.data(), buf.size());
    if (0 == rc) {
      // EOF
      break;
    }
    TEST_AND_RETURN_FALSE_ERRNO(rc > 0);
    TEST_AND_RETURN_FALSE_ERRNO(writer.Write(buf.data(), rc));
  }

  // Write payload signature blob.
  if (!private_key_path.empty()) {
    LOG(INFO) << "Signing the update...";
    string signature;
    TEST_AND_RETURN_FALSE(PayloadSigner::SignPayload(
        payload_file,
        {private_key_path},
        metadata_size,
        metadata_signature_size,
        metadata_size + metadata_signature_size + manifest_.signatures_offset(),
        &signature));
    TEST_AND_RETURN_FALSE_ERRNO(
        writer.Write(signature.data(), signature.size()));
  }

  ReportPayloadUsage(metadata_size);
  *metadata_size_out = metadata_size;
  return true;
}

bool PayloadFile::ReorderDataBlobs(const string& data_blobs_path,
                                   const string& new_data_blobs_path) {
  int in_fd = open(data_blobs_path.c_str(), O_RDONLY, 0);
  TEST_AND_RETURN_FALSE_ERRNO(in_fd >= 0);
  ScopedFdCloser in_fd_closer(&in_fd);

  DirectFileWriter writer;
  int rc = writer.Open(
      new_data_blobs_path.c_str(), O_WRONLY | O_TRUNC | O_CREAT, 0644);
  if (rc != 0) {
    PLOG(ERROR) << "Error creating " << new_data_blobs_path;
    return false;
  }
  ScopedFileWriterCloser writer_closer(&writer);
  uint64_t out_file_size = 0;

  for (auto& part : part_vec_) {
    for (AnnotatedOperation& aop : part.aops) {
      if (!aop.op.has_data_offset())
        continue;
      CHECK(aop.op.has_data_length());
      brillo::Blob buf(aop.op.data_length());
      ssize_t rc = pread(in_fd, buf.data(), buf.size(), aop.op.data_offset());
      TEST_AND_RETURN_FALSE(rc == static_cast<ssize_t>(buf.size()));

      // Add the hash of the data blobs for this operation
      TEST_AND_RETURN_FALSE(AddOperationHash(&aop.op, buf));

      aop.op.set_data_offset(out_file_size);
      TEST_AND_RETURN_FALSE_ERRNO(writer.Write(buf.data(), buf.size()));
      out_file_size += buf.size();
    }
  }
  return true;
}

bool PayloadFile::AddOperationHash(InstallOperation* op,
                                   const brillo::Blob& buf) {
  brillo::Blob hash;
  TEST_AND_RETURN_FALSE(HashCalculator::RawHashOfData(buf, &hash));
  op->set_data_sha256_hash(hash.data(), hash.size());
  return true;
}

void PayloadFile::ReportPayloadUsage(uint64_t metadata_size) const {
  std::map<DeltaObject, int> object_counts;
  off_t total_size = 0;
  int total_op = 0;

  for (const auto& part : part_vec_) {
    string part_prefix = "<" + part.name + ">:";
    for (const AnnotatedOperation& aop : part.aops) {
      DeltaObject delta(
          part_prefix + aop.name, aop.op.type(), aop.op.data_length());
      object_counts[delta]++;
      total_size += aop.op.data_length();
    }
    total_op += part.aops.size();
  }

  object_counts[DeltaObject("<manifest-metadata>", -1, metadata_size)] = 1;
  total_size += metadata_size;

  constexpr char kFormatString[] = "%6.2f%% %10jd %-13s %s %d\n";
  for (const auto& object_count : object_counts) {
    const DeltaObject& object = object_count.first;
    // Use printf() instead of LOG(INFO) because timestamp makes it difficult to
    // compare two reports.
    printf(kFormatString,
           object.size * 100.0 / total_size,
           object.size,
           (object.type >= 0
                ? InstallOperationTypeName(
                      static_cast<InstallOperation::Type>(object.type))
                : "-"),
           object.name.c_str(),
           object_count.second);
  }
  printf(kFormatString, 100.0, total_size, "", "<total>", total_op);
  fflush(stdout);
}

}  // namespace chromeos_update_engine
