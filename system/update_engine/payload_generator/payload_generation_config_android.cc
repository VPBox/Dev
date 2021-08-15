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

#include "update_engine/payload_generator/payload_generation_config.h"

#include <base/logging.h>
#include <base/strings/string_number_conversions.h>
#include <base/strings/string_split.h>
#include <brillo/secure_blob.h>
#include <fec/io.h>
#include <libavb/libavb.h>
#include <verity/hash_tree_builder.h>

#include "update_engine/common/utils.h"
#include "update_engine/payload_consumer/verity_writer_android.h"
#include "update_engine/payload_generator/extent_ranges.h"

namespace chromeos_update_engine {

namespace {
bool AvbDescriptorCallback(const AvbDescriptor* descriptor, void* user_data) {
  PartitionConfig* part = static_cast<PartitionConfig*>(user_data);
  AvbDescriptor desc;
  TEST_AND_RETURN_FALSE(
      avb_descriptor_validate_and_byteswap(descriptor, &desc));
  if (desc.tag != AVB_DESCRIPTOR_TAG_HASHTREE)
    return true;

  AvbHashtreeDescriptor hashtree;
  TEST_AND_RETURN_FALSE(avb_hashtree_descriptor_validate_and_byteswap(
      reinterpret_cast<const AvbHashtreeDescriptor*>(descriptor), &hashtree));
  // We only support version 1 right now, will need to introduce a new
  // payload minor version to support new dm verity version.
  TEST_AND_RETURN_FALSE(hashtree.dm_verity_version == 1);
  part->verity.hash_tree_algorithm =
      reinterpret_cast<const char*>(hashtree.hash_algorithm);

  const uint8_t* salt = reinterpret_cast<const uint8_t*>(descriptor) +
                        sizeof(AvbHashtreeDescriptor) +
                        hashtree.partition_name_len;
  part->verity.hash_tree_salt.assign(salt, salt + hashtree.salt_len);

  TEST_AND_RETURN_FALSE(hashtree.data_block_size ==
                        part->fs_interface->GetBlockSize());
  part->verity.hash_tree_data_extent =
      ExtentForBytes(hashtree.data_block_size, 0, hashtree.image_size);

  TEST_AND_RETURN_FALSE(hashtree.hash_block_size ==
                        part->fs_interface->GetBlockSize());
  part->verity.hash_tree_extent = ExtentForBytes(
      hashtree.hash_block_size, hashtree.tree_offset, hashtree.tree_size);

  part->verity.fec_data_extent =
      ExtentForBytes(hashtree.data_block_size, 0, hashtree.fec_offset);
  part->verity.fec_extent = ExtentForBytes(
      hashtree.data_block_size, hashtree.fec_offset, hashtree.fec_size);
  part->verity.fec_roots = hashtree.fec_num_roots;
  return true;
}

// Generate hash tree and FEC based on the verity config and verify that it
// matches the hash tree and FEC stored in the image.
bool VerifyVerityConfig(const PartitionConfig& part) {
  const size_t block_size = part.fs_interface->GetBlockSize();
  if (part.verity.hash_tree_extent.num_blocks() != 0) {
    auto hash_function =
        HashTreeBuilder::HashFunction(part.verity.hash_tree_algorithm);
    TEST_AND_RETURN_FALSE(hash_function != nullptr);
    HashTreeBuilder hash_tree_builder(block_size, hash_function);
    uint64_t data_size =
        part.verity.hash_tree_data_extent.num_blocks() * block_size;
    uint64_t tree_size = hash_tree_builder.CalculateSize(data_size);
    TEST_AND_RETURN_FALSE(
        tree_size == part.verity.hash_tree_extent.num_blocks() * block_size);
    TEST_AND_RETURN_FALSE(
        hash_tree_builder.Initialize(data_size, part.verity.hash_tree_salt));

    brillo::Blob buffer;
    for (uint64_t offset = part.verity.hash_tree_data_extent.start_block() *
                           block_size,
                  data_end = offset + data_size;
         offset < data_end;) {
      constexpr uint64_t kBufferSize = 1024 * 1024;
      size_t bytes_to_read = std::min(kBufferSize, data_end - offset);
      TEST_AND_RETURN_FALSE(
          utils::ReadFileChunk(part.path, offset, bytes_to_read, &buffer));
      TEST_AND_RETURN_FALSE(
          hash_tree_builder.Update(buffer.data(), buffer.size()));
      offset += buffer.size();
      buffer.clear();
    }
    TEST_AND_RETURN_FALSE(hash_tree_builder.BuildHashTree());
    TEST_AND_RETURN_FALSE(utils::ReadFileChunk(
        part.path,
        part.verity.hash_tree_extent.start_block() * block_size,
        tree_size,
        &buffer));
    TEST_AND_RETURN_FALSE(hash_tree_builder.CheckHashTree(buffer));
  }

  if (part.verity.fec_extent.num_blocks() != 0) {
    TEST_AND_RETURN_FALSE(VerityWriterAndroid::EncodeFEC(
        part.path,
        part.verity.fec_data_extent.start_block() * block_size,
        part.verity.fec_data_extent.num_blocks() * block_size,
        part.verity.fec_extent.start_block() * block_size,
        part.verity.fec_extent.num_blocks() * block_size,
        part.verity.fec_roots,
        block_size,
        true /* verify_mode */));
  }
  return true;
}
}  // namespace

bool ImageConfig::LoadVerityConfig() {
  for (PartitionConfig& part : partitions) {
    // Parse AVB devices.
    if (part.size > sizeof(AvbFooter)) {
      uint64_t footer_offset = part.size - sizeof(AvbFooter);
      brillo::Blob buffer;
      TEST_AND_RETURN_FALSE(utils::ReadFileChunk(
          part.path, footer_offset, sizeof(AvbFooter), &buffer));
      if (memcmp(buffer.data(), AVB_FOOTER_MAGIC, AVB_FOOTER_MAGIC_LEN) == 0) {
        LOG(INFO) << "Parsing verity config from AVB footer for " << part.name;
        AvbFooter footer;
        TEST_AND_RETURN_FALSE(avb_footer_validate_and_byteswap(
            reinterpret_cast<const AvbFooter*>(buffer.data()), &footer));
        buffer.clear();

        TEST_AND_RETURN_FALSE(
            footer.vbmeta_offset + sizeof(AvbVBMetaImageHeader) <= part.size);
        TEST_AND_RETURN_FALSE(utils::ReadFileChunk(
            part.path, footer.vbmeta_offset, footer.vbmeta_size, &buffer));
        TEST_AND_RETURN_FALSE(avb_descriptor_foreach(
            buffer.data(), buffer.size(), AvbDescriptorCallback, &part));
      }
    }

    // Parse VB1.0 devices with FEC metadata, devices with hash tree without
    // FEC will be skipped for now.
    if (part.verity.IsEmpty() && part.size > FEC_BLOCKSIZE) {
      brillo::Blob fec_metadata;
      TEST_AND_RETURN_FALSE(utils::ReadFileChunk(part.path,
                                                 part.size - FEC_BLOCKSIZE,
                                                 sizeof(fec_header),
                                                 &fec_metadata));
      const fec_header* header =
          reinterpret_cast<const fec_header*>(fec_metadata.data());
      if (header->magic == FEC_MAGIC) {
        LOG(INFO)
            << "Parsing verity config from Verified Boot 1.0 metadata for "
            << part.name;
        const size_t block_size = part.fs_interface->GetBlockSize();
        // FEC_VERITY_DISABLE skips verifying verity hash tree, because we will
        // verify it ourselves later.
        fec::io fh(part.path, O_RDONLY, FEC_VERITY_DISABLE);
        TEST_AND_RETURN_FALSE(fh);
        fec_verity_metadata verity_data;
        if (fh.get_verity_metadata(verity_data)) {
          auto verity_table = base::SplitString(verity_data.table,
                                                " ",
                                                base::KEEP_WHITESPACE,
                                                base::SPLIT_WANT_ALL);
          TEST_AND_RETURN_FALSE(verity_table.size() == 10);
          size_t data_block_size = 0;
          TEST_AND_RETURN_FALSE(
              base::StringToSizeT(verity_table[3], &data_block_size));
          TEST_AND_RETURN_FALSE(block_size == data_block_size);
          size_t hash_block_size = 0;
          TEST_AND_RETURN_FALSE(
              base::StringToSizeT(verity_table[4], &hash_block_size));
          TEST_AND_RETURN_FALSE(block_size == hash_block_size);
          uint64_t num_data_blocks = 0;
          TEST_AND_RETURN_FALSE(
              base::StringToUint64(verity_table[5], &num_data_blocks));
          part.verity.hash_tree_data_extent =
              ExtentForRange(0, num_data_blocks);
          uint64_t hash_start_block = 0;
          TEST_AND_RETURN_FALSE(
              base::StringToUint64(verity_table[6], &hash_start_block));
          part.verity.hash_tree_algorithm = verity_table[7];
          TEST_AND_RETURN_FALSE(base::HexStringToBytes(
              verity_table[9], &part.verity.hash_tree_salt));
          auto hash_function =
              HashTreeBuilder::HashFunction(part.verity.hash_tree_algorithm);
          TEST_AND_RETURN_FALSE(hash_function != nullptr);
          HashTreeBuilder hash_tree_builder(block_size, hash_function);
          uint64_t tree_size =
              hash_tree_builder.CalculateSize(num_data_blocks * block_size);
          part.verity.hash_tree_extent =
              ExtentForRange(hash_start_block, tree_size / block_size);
        }
        fec_ecc_metadata ecc_data;
        if (fh.get_ecc_metadata(ecc_data) && ecc_data.valid) {
          TEST_AND_RETURN_FALSE(block_size == FEC_BLOCKSIZE);
          part.verity.fec_data_extent = ExtentForRange(0, ecc_data.blocks);
          part.verity.fec_extent =
              ExtentForBytes(block_size, ecc_data.start, header->fec_size);
          part.verity.fec_roots = ecc_data.roots;
        }
      }
    }

    if (!part.verity.IsEmpty()) {
      TEST_AND_RETURN_FALSE(VerifyVerityConfig(part));
    }
  }
  return true;
}

}  // namespace chromeos_update_engine
